#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/ioctl.h>
#endif

#include <crc32.h>
#include "sfv.h"

int check_sfv_file(char *fn) {
    FILE *f;
    char line[10240]; /* Should be more than enough */
    char file[10240];
    uint32_t crc32a, crc32b;
    unsigned int i, len, nberr=0;

    f = fopen(fn, "r");
    if (f == NULL) {
        perror(fn);
        return -1;
    }

    print_head_foot(fn);

    while (fgets(line, 10240, f) != NULL) {
        if (*line == ';' || *line == '\n' || *line == '\r') /*Comment char & whitespace*/
            continue;

        len = strlen(line);
        if (len == 0)
            continue;

        while (line[len-1] == '\n' || line[len-1] == '\r') {
            line[len-1] = '\0';
            len--;
        }

        i = len - 1;
        while (isxdigit(line[i]) && i >= 0)
            i--;

        /*Filename*/
        strncpy(file, line, i);
        file[i] = '\0';

        /*CRC32 from sfv*/
        sscanf(line+i+1, "%x", &crc32a);

        /*Calculated crc32*/
        if (crc32_hash_file(file, &crc32b) != 0) {
            fprintf(stderr, "ERR: %s\n", file);
            nberr++;
            continue;
        }

        if (crc32a == crc32b) {
            fprintf(stdout, "OK:  %s\n", file);
        } else {
            fprintf(stderr, "BAD: %s\n", file);
            nberr++;
        }
    }

    if (nberr == 0)
        puts("\nEverything is OK!");
    else if (nberr == 1)
        fprintf(stderr, "\nThere was %u error.\n", nberr);
    else
        fprintf(stderr, "\nThere were %u errors.\n", nberr);

    fclose(f);

    print_head_foot(NULL);
    return 0;
}

void print_head_foot(char *fn) {
    char *buffer, *file = NULL;
    unsigned short width;

    /*Buffer initialisation*/
    width = get_console_width();
    buffer = malloc(width + 1);
    memset(buffer, '*', width);
    buffer[width] = '\0';

    if (fn != NULL) {
        file = calloc(1, width - 3);
        strncpy(file, "Verification de ", strlen("Verification de "));
        strncat(file, fn, width - 4 - strlen("Verification de "));

        strncpy(buffer + ((width - strlen(file)) / 2), file, strlen(file));

        /*Space before and after the string*/
        *(buffer + ((width - strlen(file)) / 2) - 1) = ' ';
        *(buffer + ((width + strlen(file)) / 2)) = ' ';

        if (strlen(fn) > width - 4 - strlen("Verification de "))
            strncpy(buffer + (width - 5), "...", 3);
    }

    printf("%s\n", buffer);
    free(buffer);
    free(file);
}

int get_console_width() {
#ifdef __linux__
    /*struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);
    return ts.ts_cols;*/ /*ts_lines*/
    struct winsize ws;
    unsigned short height, width;

    if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 &&
            ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == -1 &&
            ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) ||
            ws.ws_col == 0) {
        height = 25;
        width = 80;
    } else {
        height = ws.ws_row;
        width = ws.ws_col;
    }

    return width;
#elif defined WIN32
    return 80;
#else
    return 80;
#endif
}
