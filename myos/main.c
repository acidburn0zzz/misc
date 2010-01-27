#include <stdio.h>
#include <string.h>

#include "boot.h"
#include "shell.h"

void help() {
    puts("Bla bla bla...");
}

int main(int argc, char **argv) {
    char verbose = 0;
    int i;

    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-H") == 0) {
            help();
            return 0;
        } else if (strcmp(argv[i], "-V") == 0) {
            verbose = 1;
        } else {
            printf("Incorrect parameter: %s\n", argv[i]);
            help();
            return 1;
        }
    }

    myboot();
    myshell(verbose);
    unboot();

    return 0;
}
