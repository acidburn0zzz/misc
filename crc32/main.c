#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <crc32.h>

#define __PROG_VERSION__ "1.0"

#define BUFFER_SIZE 512

unsigned long size, remain;
char *cur_file;

void msleep(int ms);
void usage(char *app_name);
int get_console_width(void);
void print_header(void);
void print_head_foot(char *fn);
void pct(void);
int hash_file(char *fn, uint32_t *sum);
int check_sfv_file(char *fn);

void msleep(int ms) {
    struct timespec req;
    time_t sec = (ms/1000);
    ms = ms - (sec*1000);

    req.tv_sec=sec;
    req.tv_nsec=ms*1000000L;

    while (nanosleep(&req, &req) == -1)
        continue;
}

void usage(char *app_name) {
    fprintf(stderr, "AcidCRC32 %s\n", __PROG_VERSION__);
    fprintf(stderr, "Usage :\n");
    fprintf(stderr, "\t%s [-u] file(s)\n", app_name);
    fprintf(stderr, "OR\n");
    fprintf(stderr, "\t%s [-u] -c sfv_file\n", app_name);
}

int get_console_width() {
    struct winsize ws;

    if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 &&
            ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == -1 &&
            ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) ||
            ws.ws_col == 0) {
        return 80;
    } else {
        return ws.ws_col;
    }
}

void print_header() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[128];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (strftime(buffer, 128, "le %Y-%m-%d a %X", timeinfo) == 0)
        buffer[0] = 0;

    printf("; Genere par AcidCRC32 %s %s\n;\n", __PROG_VERSION__, buffer);
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

void pct() {
    while (remain > 0) {
        fprintf(stderr, "\r%s: %5.2f%%", cur_file, 100.0 * (size - remain) / size);
        msleep(50);
    }

    fprintf(stderr, "\r\033[2K");

    return;
}

int hash_file(char *fn, uint32_t *sum) {
    FILE *f;
    unsigned int rsize;
    unsigned char buffer[BUFFER_SIZE];

    pthread_t pct_thread;

    if (!(f = fopen(fn, "rb"))) {
        perror(fn);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    remain = size;
    cur_file = fn;
    pthread_create(&pct_thread, NULL, (void * (*)(void *)) pct, NULL);

    crc32_begin(sum);

    while (remain > 0) {
        rsize = (BUFFER_SIZE < remain ? BUFFER_SIZE : remain);

        if (fread(buffer, rsize, 1, f) != 1) {
            perror(fn);
            return -1;
        }

        crc32_hash(buffer, rsize, sum);
        remain -= rsize;
    }

    fclose(f);

    crc32_end(sum);
    pthread_join(pct_thread, NULL);

    return 0;
}

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
        if (hash_file(file, &crc32b) != 0) {
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

int main(int argc, char **argv) {
    uint32_t crc32;
    int8_t opt;
    char *sfv_file = NULL;
    int sfv = 0;
    int upper_case = 0;

    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
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
        if (hash_file(*argv, &crc32) != 0)
            exit(EXIT_FAILURE);

        if (upper_case)
            printf("%s %.8X\n", *argv, crc32);
        else
            printf("%s %.8x\n", *argv, crc32);

        argv++;
    }

    return EXIT_SUCCESS;
}
