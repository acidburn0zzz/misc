#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hexdump.h"

/* Comment to remove line comment */
#define LINE_COMMENT 1

/* Un-comment to add debug output */
/* #define DEBUG 1 */

/* Should be at least 0x7530 (30 000) */
#define ARRAY_SIZE 0x8000

#define TRUE 1
#define FALSE 0

/* Prototypes */
void debug_print(const char *fmt, ...);
void run(void);

/* Globals */
unsigned char *ptr, *buf, *_ptr, *_buf;
unsigned int break_pos;
size_t buf_size;

#ifdef DEBUG
    void debug_print(const char *fmt, ...) {
        char msg[1024];
        va_list va;

        va_start(va, fmt);
        vsnprintf(msg, sizeof msg, fmt, va);
        fprintf(stderr, "%s", msg);
    }
#else
    inline void debug_print(const char *fmt, ...) { }
#endif

void run() {
    int i;
    char c;

    while (buf - _buf < buf_size) {
        c = *buf;

        switch (c) {
        case '>':
            ++ptr;
            if (ptr >= _ptr + ARRAY_SIZE) {
                fprintf(stderr, "Error: Pointer can't go over 0x%x\n", ARRAY_SIZE);
                exit(EXIT_FAILURE);
            }
            debug_print("(%.3ld): %c | Pos. is %ld\n", buf - _buf, c, ptr - _ptr);
            break;
        case '<':
            --ptr;
            if (ptr < _ptr) {
                fprintf(stderr, "Error: Pointer can't go under 0\n");
                exit(EXIT_FAILURE);
            }
            debug_print("(%.3ld): %c | Pos. is %ld\n", buf - _buf, c, ptr - _ptr);
            break;
        case '+':
            ++(*ptr);
            debug_print("(%.3ld): %c | v[%ld]=%d\n", buf - _buf, c, ptr - _ptr, *ptr);
            break;
        case '-':
            --(*ptr);
            debug_print("(%.3ld): %c | v[%ld]=%d\n", buf - _buf, c, ptr - _ptr, *ptr);
            break;
        case '.':
            putchar(*ptr);
            debug_print("(%.3ld): %c | output '%d' %c\n", buf - _buf, c, *ptr, *ptr);
            break;
        case ',':
            *ptr = getchar();
            debug_print("(%.3ld): %c | read '%d' %c\n", buf - _buf, c, *ptr, *ptr);
            break;
        case '[':
            if (*ptr == 0) {
                i = 0;
                c = *(++buf);
                while (c != ']' || i != 0) {
                    if (c == '[')
                        i++;
                    if (c == ']')
                        i--;
                    c = *(++buf);
                }
            }
            break;
        case ']':
            if (*ptr != 0) {
                i = 0;
                c = *(--buf);
                while (c != '[' || i != 0) {
                    if (c == '[')
                        i--;
                    if (c == ']')
                        i++;
                    c = *(--buf);
                }
            }
            break;
#ifdef LINE_COMMENT
        case '#':
        case ';':
            while (c != 0x0a && c != 0x0d && c != 0x00)
                c = *(++buf);
            break;
#endif
        }

        buf++;
    }
}

int main(int argc, char *argv[]) {
    FILE *file;

    if (argc < 2) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }

    file = fopen(argv[argc - 1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: %s: File not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    buf_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    ptr = _ptr = (unsigned char*)calloc(1, ARRAY_SIZE);
    buf = _buf = (unsigned char*)malloc(buf_size);

    if (fread(buf, buf_size, sizeof(char), file) != sizeof(char)) {
        fprintf(stderr, "Error: Unable to read from file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    fclose(file);

    run();

    free(_buf);
    free(_ptr);
    return EXIT_SUCCESS;
}
