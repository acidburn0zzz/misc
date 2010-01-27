#include <stdio.h>
#include <string.h>

#include "kernel.h"
#include "shell.h"

void read_command(char *cmd, int size) {
    int i;

    fgets(cmd, size, stdin);
    for (i=0; i<size; i++) {
        if (cmd[i] == '\n' || cmd[i] == '\r') {
            cmd[i] = '\0';
            break;
        }
    }

    /* flush the rest */
}

void version() {
    puts("MyOS v0.001 by The Chancelor");
}

void myshell(char verbose) {
    char cmd[256];
    puts("Welcome to AwesomeShell\n");

    while (1) {
        printf("prompt> ");
        read_command(cmd, 256);

        if (strlen(cmd) == 0)
            continue;

        if (strcmp(cmd, "exit") == 0) {
            return;
        } else if (strcmp(cmd, "ver") == 0) {
            version();
        } else {
            printf("%s: Unknown command\n", cmd);
        }
    }
}
