#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pcre.h>

void write_pid(const char *pid_file) {
    FILE *f_pid;

    f_pid = fopen(pid_file, "w");
    if (f_pid == NULL) {
        perror(pid_file);
        exit(EXIT_FAILURE);
    }

    fprintf(f_pid, "%d", getpid());
    fclose(f_pid);
}

/*
 * Check if application is running at pid specified in file
 * Returns : 0 if application not running, -1 if pid file exists but can't be read
 *           pid of running app in other cases.
 */
int already_running(const char *pid_file, const char *app_name) {
    FILE *f;
    pcre *regex_app;
    const char *error;
    int erroffset;
    char s_pid[9], cmd_file[1024], cmd_line[1024];
    int pid;

    f = fopen(pid_file, "r");
    if (f == NULL) {
        if (errno == EACCES) {
            /* Permission denied, assume running */
            return -1;
        } else if (errno == ENOENT) {
            /* File not found, assume not running */
            return 0;
        }

        /* Other error, assume not running */
        return 0;
    }

    fgets(s_pid, 8, f);
    fclose(f);
    s_pid[8] = '\0';

    pid = atoi(s_pid);
    if (pid == 0) {
        /* No pid found in file */
        return 0;
    }

    regex_app = pcre_compile(app_name, 0, &error, &erroffset, NULL);
    if (regex_app == NULL) {
        fprintf(stderr, "%s at %d\n", error, erroffset);
        exit(EXIT_FAILURE);
    }

    sprintf(cmd_file, "/proc/%d/cmdline", pid);
    f = fopen(cmd_file, "r");
    if (f == NULL) {
        if (errno == ENOENT) {
            /* file / pid does not exist */
            return 0;
        }
        /* TODO: Remove or better check */
        /* Nothing found at pid */
        return 0;
    }

    fgets(cmd_line, 1023, f);
    fclose(f);

    if (pcre_exec(regex_app, NULL, cmd_line, strlen(cmd_line), 0, 0, NULL, 0) == 0) {
        return pid;
    }

    return 0;
}
