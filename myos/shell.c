#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "envvar.h"
#include "kernel.h"
#include "panic.h"
#include "shell.h"

struct envvar_s *vars = NULL;

void read_command(char *cmd, int size) {
    int i;
    char c;

    fgets(cmd, size, stdin);
    for (i=0; i<size; i++) {
        if (cmd[i] == '\n' || cmd[i] == '\r') {
            cmd[i] = '\0';
            break;
        }
    }

    /* Flushing the rest of stdin */
    /*
    while ((c = getc(stdin)) != EOF && c != '\n' && c != '\r') {
        continue;
    }
    */
}

void version() {
    puts("MyOS v0.001 by The Chancelor");
}

void cleanup() {
    cleanEnv(vars);
}

void foo(struct envvar_s *list) {
    /* Dummy test function */

    while (list != NULL) {
        printf("%s=%s\n", list->key, list->val);
        list = list->next;
    }
}

void myshell(char verbose) {
    char cmd[256];

    puts("Welcome to AwesomeShell");

    while (1) {
        printf("prompt> ");
        read_command(cmd, 256);

        if (strlen(cmd) == 0)
            continue;

        if (strcmp(cmd, "EXIT") == 0) {
            cleanup();
            return;
        } else if (strcmp(cmd, "VER") == 0) {
            version();
        } else if (strcmp(cmd, "CLR") == 0) {
            system("clear");
        } else if (strncmp(cmd, "SET ", (strlen(cmd) >= 4 ? 4 : 3)) == 0) {
            vars = addEnv(cmd+4, vars);
        } else if (strncmp(cmd, "ECHO ", (strlen(cmd) >= 5 ? 5 : 4)) == 0) {
            char *tmp = getEnv(cmd+5, vars);
            puts(tmp != NULL ? tmp : "");
        } else if (strcmp(cmd, "foo") == 0) {
            foo(vars);
        } else {
            /* printf("%s: Unknown command\n", cmd); */
            system(cmd);
        }
    }
}
