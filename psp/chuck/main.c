#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspkernel.h>
#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("CHUCK_PRX", 0x800, 1, 1);

int count_lines(FILE *f) {
    char ch, prev = '\n';
    int lines = 0;
    
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n')
            lines++;
        prev = ch;
    }
    
    /*Adding a line if file doesn't finish with a new line*/
    if (prev != '\n')
        lines++;
    
    return lines;
}

char* get_line_number(int n, FILE *f) {
    static char fact[1024];
    char ch;
    
    /*We return to the beginning of the file*/
    fseek(f, 0, SEEK_SET);
    while (n) {
        ch = fgetc(f);
        if (ch == '\n')
            n--;
    }
    
    fgets(fact, 1024, f);
    
    return fact;
}

void print_fact(const char* fact, int nb) {
    pspDebugScreenInit();
    pspDebugScreenSetBackColor(0x00000000);
    pspDebugScreenSetXY(0, 0);
    
    printf("**********************\n");
    printf("* Chuck Norris Facts *\n");
    printf("**********************\n\n");
    
    printf("Fact #%i\n", nb+1);
    printf("%s\n", fact);
    
    printf("\nPress X to return to XMB");
}

int main(int argc, char **argv) {
    FILE *f;
    int lines, nb;
    char *fact;
    
    srand(time(NULL));
    
    f = fopen("facts", "r");
    if (f == NULL) {
        perror("chuck: facts");
        exit(EXIT_FAILURE);
    }
    
    lines = count_lines(f);


    SceCtrlData pad;
    while (1) { /*Boucle principale du plugin*/
        sceCtrlPeekBufferPositive(&pad, 1);
        
        if (pad.Buttons & PSP_CTRL_SQUARE) {
            /*Get a random fact*/
            nb = rand()%lines;
            fact = get_line_number(nb, f);
            print_fact(fact, nb);

            while (1) { /*On attend la touche X pour quitter*/
                sceCtrlPeekBufferPositive(&pad, 1);
                if (pad.Buttons & PSP_CTRL_CROSS)
                    break;
            }
        }
        sceKernelDelayThread(1000);
    }
    
    fclose(f);
    
	return 0;
}

/* Exported function returns the address of module_info */
void* getModuleInfo() {
	return (void*) &module_info;
}
