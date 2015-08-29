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
unsigned char *data_ptr, *ip, *data_array, *instr_array;
unsigned int break_pos;
size_t program_size;

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

    while (ip - instr_array < program_size) {
        c = *ip;

        switch (c) {
        case '>':
            ++data_ptr;
            if (data_ptr >= data_array + ARRAY_SIZE) {
                fprintf(stderr, "Error: Pointer can't go over 0x%x\n", ARRAY_SIZE);
                exit(EXIT_FAILURE);
            }
            debug_print("(%.3ld): %c | Pos. is %ld\n", ip - instr_array, c, data_ptr - data_array);
            break;
        case '<':
            --data_ptr;
            if (data_ptr < data_array) {
                fprintf(stderr, "Error: Pointer can't go under 0\n");
                exit(EXIT_FAILURE);
            }
            debug_print("(%.3ld): %c | Pos. is %ld\n", ip - instr_array, c, data_ptr - data_array);
            break;
        case '+':
            ++(*data_ptr);
            debug_print("(%.3ld): %c | v[%ld]=%d\n", ip - instr_array, c, data_ptr - data_array, *data_ptr);
            break;
        case '-':
            --(*data_ptr);
            debug_print("(%.3ld): %c | v[%ld]=%d\n", ip - instr_array, c, data_ptr - data_array, *data_ptr);
            break;
        case '.':
            putchar(*data_ptr);
            debug_print("(%.3ld): %c | output '%d' %c\n", ip - instr_array, c, *data_ptr, *data_ptr);
            break;
        case ',':
            *data_ptr = getchar();
            debug_print("(%.3ld): %c | read '%d' %c\n", ip - instr_array, c, *data_ptr, *data_ptr);
            break;
        case '[':
            if (*data_ptr == 0) {
                i = 0;
                c = *(++ip);
                while (c != ']' || i != 0) {
                    if (c == '[')
                        i++;
                    if (c == ']')
                        i--;
                    c = *(++ip);
                }
            }
            break;
        case ']':
            if (*data_ptr != 0) {
                i = 0;
                c = *(--ip);
                while (c != '[' || i != 0) {
                    if (c == '[')
                        i--;
                    if (c == ']')
                        i++;
                    c = *(--ip);
                }
            }
            break;
#ifdef LINE_COMMENT
        case '#':
        case ';':
            while (c != 0x0a && c != 0x0d && c != 0x00)
                c = *(++ip);
            break;
#endif
        }

        ip++;
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
    program_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data_ptr = data_array = (unsigned char*)calloc(1, ARRAY_SIZE);
    ip = instr_array = (unsigned char*)malloc(program_size);

    if (fread(ip, program_size, sizeof(char), file) != sizeof(char)) {
        fprintf(stderr, "Error: Unable to read from file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    fclose(file);

    run();

    free(instr_array);
    free(data_array);
    return EXIT_SUCCESS;
}
