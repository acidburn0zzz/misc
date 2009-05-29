#include <malloc.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__
#include <sys/ioctl.h>
#endif

#include <crc32.h>
#include "sfv.h"

void check_sfv_file(char *fn) {
    print_head_foot(fn);
    print_head_foot(0);
}
    
void print_head_foot(char *fn) {
    char *buffer, *file = NULL;
    
    /*Initialisation du buffer*/
    buffer = malloc(getConsoleWidth() + 1);
    memset(buffer, '-', 80);
    buffer[80] = '\0';
    
    if (fn != NULL) {
        file = calloc(1, 77);
        strncpy(file, "Verification de ", strlen("Verification de "));
        strncat(file, fn, 76 - strlen("Verification de "));
            
        strncpy(buffer + ((getConsoleWidth() - strlen(file)) / 2), file, strlen(file));
        
        if (strlen(fn) > 76 - strlen("Verification de "))
            strncpy(buffer + (getConsoleWidth() - 5), "...", 3);
    }
    
    printf("%s\n", buffer);
    free(buffer);
    free(file);
}

int getConsoleWidth() {
#ifdef __linux__
    struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);
    return ts.ts_cols; /*ts_lines*/
#elif defined WIN32
    return 80;
#else
    return 80;
#endif
}
