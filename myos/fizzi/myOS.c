#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "myOS.h"
#include "mySH.h"
#include "myKernel.h"

char *RAM[RAMSIZE];
FILE *HDD;

/*  Boot up OS
    Error Codes:
    1: RAM Issue
    2: Hard Drive issue */
int boot(){
    int i;

    // Set up and test RAM
    for(i = 0; i < 1000; i++){
        RAM[i] = NULL;
    }
    for(i = 0; i < 1000; i++){
        if(RAM[i] != NULL) {
            printf("RAM damaged at address %x\n", i);
            return 1;
        }
    }

    // Format HDD and ensure it was done properly
    HDD = fopen("hdd", "w+b");
    for (i=0; i<10000; i++) {
        fputc(0, HDD);
    }

    rewind(HDD);
    for (i=0; i<10000; i++) {
        if(fgetc(HDD) != 0){
            puts("Hard disk could not be formatted");
            return 2;
        }
    }
    fflush(HDD);

    return 0;
}

int main(int argc, char **argv){
	int i;

	if (boot() > 0) return 0; //End if error was encountered
	puts("Welcome to Mios - Linux for feline companions!");

	kernel(argc, argv);

	puts("Shutting down... Meow");
	return 0;
}
