#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void write_pid() {
    FILE *f_pid;

    f_pid = fopen("app.pid", "w");
    if (f_pid == NULL) {
        perror("app.pid");
        exit(EXIT_FAILURE);
    }

    fprintf(f_pid, "%d", getpid());
    fclose(f_pid);
}

int already_running() {
    FILE *f;
    char s_pid[9], cmd_file[1024], cmd_line[1024];
    int pid;

    f = fopen("app.pid", "r");
    if (f == NULL) {
        perror("app.pid");

        if (errno == EACCES) {
            /* Permission denied, assume running */
            return 1;
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

    sprintf(cmd_file, "/proc/%d/cmdline", pid);
    f = fopen(cmd_file, "r");
    if (f == NULL) {
        /* Check error for FileNotFound */
        /* Nothing found at pid */
        return 0;
    }

    fgets(cmd_line, 1023, f);
    fclose(f);

    printf("%s: %s\n", cmd_file, cmd_line);

    return 1;
}

int main(int argc, char **argv) {
    if (already_running()) {
        fprintf(stderr, "Application is already running\n");
        return EXIT_FAILURE;
    }
    write_pid();
    getchar();
    return 0;
}
