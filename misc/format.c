#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void usage(char *prog) {
    fprintf(stderr, "Usage :\n");
    fprintf(stderr, "\t%s [-l lines] file(s)\n", prog);
}

/**
 * FIX: Will go in an infinite loop if a word is longer than len
 */
void format_file(char *fn, int len) {
    FILE *f;
    char *line, *buff;
    size_t ls = 0;

    if (!(f = fopen(fn, "r"))) {
        perror(fn);
        return;
    }

    while ((ls = getline(&line, &ls, f)) != -1) {
            buff = line;
            while (strlen(buff) > len+3) {
                buff += len;
                while (*buff != ' ' && buff > line)
                    buff--;

                *buff = '\n';
                buff++;
            }

            printf("%s", line);
    }

    free(line);
    fclose(f);
}

int main(int argc, char **argv) {
    char opt;
    int len = 79, i;

    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "l:")) != -1) {
        switch (opt) {
        case 'l':
            len = atoi(optarg);
            if (len < 1) {
                printf("Invalid length: %s\n", optarg);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    argc -= optind;
    argv += optind;

    while (argc--) {
        format_file(*argv, len);
        argv++;
    }

    return EXIT_SUCCESS;
}
