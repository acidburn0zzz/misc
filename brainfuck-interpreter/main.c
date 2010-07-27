#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int fsize(FILE *f);

int main(int argc, char *argv[]) {
    unsigned char *ptr, *buf, *_ptr, *_buf, c;
    int i, dbg = 0;
    const unsigned int PTR_SIZE = 0x8000;
    FILE *file;

    if (argc < 2) {
        printf("Usage: %s [-d] file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp("-d", argv[1]) == 0)
        dbg = 1;

    file = fopen(argv[argc-1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: %s: File not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    _ptr = calloc(1, PTR_SIZE);
    _buf = malloc(fsize(file));

    ptr = _ptr;
    buf = _buf;

    if (fread(buf, fsize(file), sizeof(char), file) != sizeof(char)) {
        fprintf(stderr, "Error: Unable to read from file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    c = *buf;
    while (c != 0x00) {
        switch (c) {
            case '>':
                ++ptr;
                if (ptr >= _ptr + PTR_SIZE) {
                    puts("\nError: Pointer can't go over 0x8000");
                    exit(EXIT_FAILURE);
                }
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | Pos. is %ld\n", buf - _buf, c, ptr - _ptr);
                c = *(++buf);
                break;
            case '<':
                --ptr;
                if (ptr < _ptr) {
                    puts("\nError: Pointer can't go under 0");
                    exit(EXIT_FAILURE);
                }
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | Pos. is %ld\n", buf - _buf, c, ptr - _ptr);
                c = *(++buf);
                break;
            case '+':
                ++(*ptr);
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | v[%ld]=%d\n", buf - _buf, c, ptr - _ptr, *ptr);
                c = *(++buf);
                break;
            case '-':
                --(*ptr);
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | v[%ld]=%d\n", buf - _buf, c, ptr - _ptr, *ptr);
                c = *(++buf);
                break;
            case '.':
                putchar(*ptr);
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | output '%d' %c\n", buf - _buf, c, *ptr, *ptr);
                c = *(++buf);
                break;
            case ',':
                *ptr = getchar();
                if (dbg)
                    fprintf(stderr, "(%.3ld): %c | read '%d' %c\n", buf - _buf, c, *ptr, *ptr);
                c = *(++buf);
                break;
            case '[':
                if (*ptr == 0) {
                    i=0;
                    c = *(++buf);
                    while (c != ']' || i != 0) {
                        if (c == '[')
                            i++;
                        if (c == ']')
                            i--;
                        c = *(++buf);
                    }
                } else {
                    c = *(++buf);
                }
                break;
            case ']':
                if (*ptr != 0) {
                    i=0;
                    c = *(--buf);
                    while (c != '[' || i != 0) {
                        if (c == '[')
                            i--;
                        if (c == ']')
                            i++;
                        c = *(--buf);
                    }
                } else {
                    c = *(++buf);
                }
                break;
            #ifdef LINE_COMMENT
            case '#': /*Ajout de moi : # = commentaire de ligne*/
            case ';': /*Ajout de moi : ; = commentaire de ligne*/
                while (c != 0x0a && c != 0x0d && c != 0x00)
                    c = *(++buf);
                break;
            #endif
            default:
                c = *(++buf);
        }
    }

    free(_buf);
    free(_ptr);
    return EXIT_SUCCESS;
}

int fsize(FILE *f) {
    int size;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}
