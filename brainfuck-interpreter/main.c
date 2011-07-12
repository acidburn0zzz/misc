#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>

/* Comment to remove line comment */
#define LINE_COMMENT 1

/* Un-comment to add debug output */
#define DEBUG 1

/* Should be at least 0x7530 (30 000) */
#define ARRAY_SIZE 0x8000

unsigned char *ptr, *buf, *_ptr, *_buf, c;
void hexdump(const char *buf, int offset, int len);

#ifdef DEBUG
#define BUFFER_MAX_LENGTH 256

static char *debug_cmd;
static char input_buffer[BUFFER_MAX_LENGTH];
static int buffer_chars;
static char user_input = '\0';

void debug_print(const char *fmt, ...) {
    char msg[1024];
    va_list va;

    va_start(va, fmt);
    vsnprintf(msg, sizeof msg, fmt, va);
    fprintf(stderr, "%s", msg);
}

void prompt() {
    printf("bfint> ");
}

void get_user_command() {
    buffer_chars = 0;
    while (user_input != '\n' && buffer_chars < BUFFER_MAX_LENGTH) {
        input_buffer[buffer_chars++] = user_input;
        user_input = getchar();
    }
    input_buffer[buffer_chars] = '\0';

    debug_cmd = strtok(input_buffer, " ");
}

int execute_debug_cmd() {
    if (strcmp("exit", debug_cmd) == 0) {
        exit(EXIT_SUCCESS);
    }
    if (strcmp("cont", debug_cmd) == 0 || strcmp("continue", debug_cmd) == 0) {
       return 0;
    }
    if (strcmp("dump", debug_cmd) == 0) {
        hexdump((const char*)_ptr, 0, 256);
        return 1;
    }

    printf("%s: Invalid command\n", debug_cmd);
    return 1;
}

void shell() {
    prompt();
    while (1) {
        user_input = getchar();
        if (user_input == '\n') {
            prompt();
            continue;
        }

        get_user_command();
        if (execute_debug_cmd() == 0)
            break;
        prompt();
    }
}
            
#else
void debug_print(const char *fmt, ...) { }
void shell() { }
#endif

void hexdump(const char *buf, int offset, int len) {
    int i;
    char *begin, *end, c;

    begin = buf + offset;
    end = begin + len;

    printf("\n");

    while (begin < end) {
        for (i=0; i<16; i++) {
            if (i != 0 && i % 4 == 0)
                printf(" ");

            printf("%.2x ", (int) *(begin+i));
        }
        for (i=0; i<16; i++) {
            if (i != 0 && i % 8 == 0)
                printf(" ");

            c = *(begin+i);
            printf("%c", (c >= 0x20 && c <= 0x7e) ? c : '.');
        }
        printf("\n");
        begin += 16;
    }
}

void interpret_next_command() {
    int i;
    static unsigned char c;

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

int main(int argc, char *argv[]) {
    int f_size;
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
    f_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    ptr = _ptr = calloc(1, ARRAY_SIZE);
    buf = _buf = malloc(f_size);

    if (fread(buf, f_size, sizeof(char), file) != sizeof(char)) {
        fprintf(stderr, "Error: Unable to read from file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    fclose(file);

    shell();
    while (buf - _buf < f_size) {
        interpret_next_command();
        shell();
    }

    hexdump((const char*)_ptr, 0, 256);
    printf("Done\n");

    free(_buf);
    free(_ptr);
    return EXIT_SUCCESS;
}
