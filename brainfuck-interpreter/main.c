#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

const unsigned int PTR_SIZE = 0x8000;
unsigned char *ptr, *buf, *_ptr, *_buf;

int fsize(FILE *f);
void interpret();

int main(int argc, char *argv[]) {
    /*unsigned char c, i;*/
    FILE *file;
    
    if (argc != 2) {
        printf("Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File %s not found\n", argv[1]);
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
    
    interpret(buf, ptr, _ptr);
    
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

void interpret() {
    unsigned char c, i;
    c = *buf;
    while (c != 0x00) {
        switch (c) {
            case '>':
                ++ptr;
                if (ptr >= _ptr + PTR_SIZE) {
                    puts("Pointer can't go over 0x8000");
                    exit(1);
                }
                break;
            case '<':
                --ptr;
                if (ptr < _ptr) {
                    puts("Pointer can't go under 0");
                    exit(1);
                }
                break;
            case '+':
                ++(*ptr);
                break;
            case '-':
                --(*ptr);
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',':
                *ptr = getchar();
                break;
            case '[':
                if (*ptr == 0) {
                    i=1;
                    while (c != ']' && i != 0) {
                        c = *(++buf);
                        if (c == '[')
                            interpret(buf, ptr, _ptr);
                    }
                }
                break;
            case ']':
                if (*ptr != 0) {
                    i=1;
                    while (c != '[' && i != 0) {
                        c = *(--buf);
                        if (c == '[')
                            interpret(buf, ptr, _ptr);
                    }
                } else {
                    return;
                }
                break;
            case '#': /*Ajout de moi : # = commentaire de ligne*/
                while (c != 0x0a && c != 0x0d && c != 0x00)
                    c = *(++buf);
        }
        c = *(++buf);
    }
    return;
}
