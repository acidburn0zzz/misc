#include <stdio.h>

struct uint24_s {
    unsigned int val : 24;
} __attribute__((packed));
typedef struct uint24_s u24;

int main(void) {
    printf("%d\n", sizeof(u24));
    return 0;
}
