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
}

void myshell(char verbose) {
    char cmd[256];
    puts("Welcome to AwesomeShell\n");

    while (1) {
        printf("prompt> ");
        read_command(cmd, 256);

        if (strcmp(cmd, "EXIT") == 0) {
            return;
        } else {
            printf("%s: Unknown command\n", cmd);
        }
    }
}
