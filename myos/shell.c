#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "envvar.h"
#include "kernel.h"
#include "panic.h"
#include "shell.h"

item_t *vars = NULL;

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
    delete_list(vars);
}

void foo(item_t *list) {
    /* Dummy test function */

    while (list != NULL) {
        printf("%s=%s\n", list->key, (char *) list->val);
        list = list->next;
    }
}

void add_env_var(char *cmd_line) {
	char *key, *val, *token;

	token = strtok(cmd_line, " ");
	key = malloc(strlen(token) + 1);
	memcpy(key, token, strlen(token) + 1);

	token = strtok(NULL, " ");
	val = malloc(strlen(token) + 1);
	memcpy(val, token, strlen(token) + 1);

	vars = add_item(vars, key, val);
}

void myshell(char verbose) {
    char input_line[256], *cmd;

    puts("Welcome to AwesomeShell");

    while (1) {
        printf("prompt> ");
        read_command(input_line, 256);

        if (strlen(input_line) == 0)
            continue;

        cmd = strtok(input_line, " ");
        if (strcmp(cmd, "EXIT") == 0) {
            cleanup();
            return;
        } else if (strcmp(cmd, "VER") == 0) {
            version();
        } else if (strcmp(cmd, "CLR") == 0) {
            system("clear");
        } else if (strcmp(cmd, "SET") == 0) {
            add_env_var(input_line + strlen(cmd) + 1);
        } else if (strcmp(cmd, "UNSET") == 0) {
            char *key;
            key = strtok(NULL, " ");
            vars = del_item(vars, key);
        } else if (strcmp(cmd, "ECHO") == 0) {
            char *key, *value;
            key = strtok(NULL, " ");
            value = (char *) get_item(vars, key);
            puts(value != NULL ? value : "");
        } else if (strcmp(cmd, "foo") == 0) {
            foo(vars);
        } else {
            /* printf("%s: Unknown command\n", cmd); */
            system(input_line);
        }
    }
}
