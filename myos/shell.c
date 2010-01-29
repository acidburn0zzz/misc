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

void setVar(char *str) {
    char *var;
    int pos = 0;

    if (strchr(str, ' ') == NULL) {
        puts("Invalid VAR");
        return;
    }

    while (pos < 1000 && RAM[pos] != NULL)
        pos++;

    if (pos == 1000)
        puts("RAM full");

    var = malloc(strlen(str) + 1);
    strcpy(var, str);

    RAM[pos] = var;
}

void echoVar(char *varName) {
    int i, pos;

    for (i=0; i<1000; i++) {
        if (RAM[i] == NULL)
            continue;

        pos = strcspn(RAM[i], " ");
        if (strncmp(varName, RAM[i], pos) == 0)
            break;
    }

    if (i == 1000)
        printf("%s: VAR not found\n", varName);
    else
        printf("%s=%s\n", varName, RAM[i]+pos+1);
}

void foo() {
    /* Dummy test function */
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
            return;
        } else if (strcmp(cmd, "VER") == 0) {
            version();
        } else if (strcmp(cmd, "CLR") == 0) {
            system("clear");
        } else if (strncmp(cmd, "SET ", (strlen(cmd) >= 4 ? 4 : 3)) == 0) {
            setVar(cmd+4);
        } else if (strncmp(cmd, "ECHO ", (strlen(cmd) >= 5 ? 5 : 4)) == 0) {
            echoVar(cmd+5);
        } else if (strcmp(cmd, "foo") == 0) {
            foo();
        } else {
            /* printf("%s: Unknown command\n", cmd); */
            system(cmd);
        }
    }
}
