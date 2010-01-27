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

    /* RAM init */
    for (i=0; i<1000; i++) {
        RAM[i] = NULL;
    }

    /* RAM check */
    for (i=0; i<1000; i++) {
        if (RAM[i] != NULL)
            panic("Ram damaged at address 0x%.4x", i);
    }

    HDD = fopen("hdd", "w+b");
    if (HDD == NULL)
        panic("AwesomeLinux is unable to format hard drive");

    /* HDD init */
    for (i=0; i<10000; i++) {
        fputc(0, HDD);
    }
    fflush(HDD);

    /* HDD check */
    fseek(HDD, 0, SEEK_SET);
    for (i=0; i<10000; i++) {
        if (fgetc(HDD) != 0)
            panic("AwesomeLinux is unable to format hard drive");
    }

    printf("Welcome to AwesomeLinux\n");
}

void unboot() {
    if (fclose(HDD) != 0)
        panic("AwesomeLinux is unable to close hard drive");

    printf("See ya!\n");
}

int main(int argc, char **argv) {
    myboot(RAM, HDD);
    unboot();

    return 0;
}
