#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <crc32.h>
#include "sfv.h"

#define __PROG_VERSION__ "1.0 "

void usage(char *prog) {
    fprintf(stderr, "AcidCRC32 %s\n", __PROG_VERSION__);
    fprintf(stderr, "Usage :\n");
    fprintf(stderr, "\t%s [-u] file(s)\n", prog);
    fprintf(stderr, "OR\n");
    fprintf(stderr, "\t%s [-u] -c sfv_file\n", prog);
    exit(EXIT_FAILURE);
}

void print_header() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[128];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (strftime(buffer, 128, "le %d/%m/%Y a %X", timeinfo) == 0)
        buffer[0] = 0;

    printf("; Genere par AcidCRC32 %s %s\n;\n", __PROG_VERSION__, buffer);
}

int main(int argc, char **argv) {
    uint32_t crc32;
    char opt, *sfv_file = NULL;
    int sfv = 0;
    int upper_case = 0;

    if (argc < 2) {
        usage(argv[0]);
        exit (EXIT_FAILURE);
    }

    /* Traitement des arguments */
    while ((opt = getopt(argc, argv, "c:hu")) != -1) {
        switch (opt) {
        case 'c':
            sfv_file = malloc(strlen(optarg) + 1);
            strncpy(sfv_file, optarg, strlen(optarg) + 1);
            sfv = 1;
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'u':
            upper_case = 1;
        }
    }

    argc -= optind;
    argv += optind;

    if (sfv) {
        check_sfv_file(sfv_file);
        free(sfv_file);
        exit(EXIT_SUCCESS);
    }

    print_header();

    while (argc--) {
        if (crc32_hash_file(*argv, &crc32) != 0)
            exit (EXIT_FAILURE);
        if (upper_case)
            printf("%s %.8X\n", *argv, crc32);
        else
            printf("%s %.8x\n", *argv, crc32);

        argv++;
    }

    return EXIT_SUCCESS;
}
