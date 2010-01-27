#include <stdio.h>
#include <malloc.h>

#include "kernel.h"
#include "panic.h"

void myboot() {
    int i;

    puts("AwesomeLinux Booting...");

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
    puts("Done\n");
}

void unboot() {
    int i;

    for (i=0; i<1000; i++) {
        if (RAM[i] != NULL)
            free(RAM[i]);

    }

    if (fclose(HDD) != 0)
        panic("AwesomeLinux is unable to close hard drive");

    printf("See ya!\n");
}
