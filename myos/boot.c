#include <stdio.h>
#include <stdarg.h>

char *RAM[1000];
FILE *HDD;

void panic(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    printf("Kernel panic: ");
    vprintf(fmt, args);
    printf("\n"); /* Won't show anything if not there */
    va_end(args);

    for (;;);
}

void myboot() {
    int i;
    panic("Unable to initialize hard drive");

    /* RAM init */
    for (i=0; i<1000; i++) {
        RAM[i] = NULL;
    }

    /* RAM check */
    for (i=0; i<1000; i++) {
        if (RAM[i] != NULL)
            panic("Ram corrupted at offset 0x%.4x", i);
    }

    HDD = fopen("hdd", "w+b");
    if (HDD == NULL)
        panic("Unable to initialize hard drive");

    /* HDD init */
    for (i=0; i<10000; i++) {
        fputc(0, HDD);
    }
    fflush(HDD);

    /* HDD check */
    fseek(HDD, 0, SEEK_SET);
    for (i=0; i<10000; i++) {
        if (fgetc(HDD) != 0)
            panic("Unable to initialize hard drive");
    }
}

int main(int argc, char **argv) {
    myboot(RAM, HDD);

    fclose(HDD);

    return 0;
}
