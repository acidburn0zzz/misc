#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "panic.h"
#include "shell.h"

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

void foo() {
    /* Dummy test function */
}

void myshell(char verbose) {
    char cmd[256], c;
    puts("Welcome to AwesomeShell");

    while (1) {
        printf("prompt> ");
        read_command(cmd, 256);

        if (strlen(cmd) == 0)
            continue;

        if (strcmp(cmd, "EXIT") == 0) {
            return;
        } else if (strcmp(cmd, "VER") == 0) {
            version();
        } else if (strcmp(cmd, "CLR") == 0) {
            system("clear");
        } else if (strcmp(cmd, "foo") == 0) {
            foo();
        } else {
            /* printf("%s: Unknown command\n", cmd); */
            system(cmd);
        }
    }
}
