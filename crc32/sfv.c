#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/ioctl.h>
#endif

#include <crc32.h>
#include "sfv.h"

int check_sfv_file(char *fn) {
    FILE *f;
    char line[10241]; /* Should be more than enough */
    char file[10241];
    unsigned int crc32a, crc32b;
    unsigned int i, len, nberr=0;
    
    print_head_foot(fn);
    
    f = fopen(fn, "r");
    if (f == NULL) {
        perror(fn);
        return -1;
    }
    
    while (fgets(line, 10240, f) != NULL) {
        if (*line == ';' || *line == '\n' || *line == '\r') /*Comment char & whitespace*/
            continue;
        
        len = strlen(line);
        if (line == 0)
            continue;
        
        if (line[len-1] == '\n' || line[len-1] == '\r') {
            line[len-1] = '\0';
            len--;
        }
        
        i = len - 1;
        while (is_hex(line[i]) && i >= 0)
            i--;
        
        /*Filename*/
        strncpy(file, line, i);
        file[i] = '\0';
        
        /*CRC32 from sfv*/
        sscanf(line+i+1, "%x", &crc32a);
        
        /*Calculated crc32*/
        if (crc32_hash_file(file, &crc32b) != 0) {
            fprintf(stderr, "ERR: %s\n", file);
            nberr++;
            continue;
        }
        
        if (crc32a == crc32b) {
            fprintf(stdout, "OK:  %s\n", file);
        } else {
            fprintf(stderr, "BAD: %s\n", file);
            nberr++;
        }
        
        /* printf("%s %.8x %.8x\n", file, crc32a, crc32b); */
    }
    
    if (nberr == 0)
        puts("\nEverything is OK!");
    else if (nberr == 1)
        fprintf(stderr, "\nThere was %u error.\n", nberr);
    else
        fprintf(stderr, "\nThere were %u errors.\n", nberr);
    
    fclose(f);
    
    print_head_foot(NULL);
    return 0;
}
    
void print_head_foot(char *fn) {
    char *buffer, *file = NULL;
    
    /*Buffer initialisation*/
    buffer = malloc(getConsoleWidth() + 1);
    memset(buffer, '*', 80);
    buffer[80] = '\0';
    
    if (fn != NULL) {
        file = calloc(1, 77);
        strncpy(file, "Verification de ", strlen("Verification de "));
        strncat(file, fn, 76 - strlen("Verification de "));
            
        strncpy(buffer + ((getConsoleWidth() - strlen(file)) / 2), file, strlen(file));
        
        /*Space before and after the string*/
        *(buffer + ((getConsoleWidth() - strlen(file)) / 2) - 1) = ' ';
        *(buffer + ((getConsoleWidth() + strlen(file)) / 2)) = ' ';
        
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

int is_hex(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    else if (c >= 'A' && c <= 'F')
        return 1;
    else if (c >= 'a' && c <= 'f')
        return 1;
    else
        return 0;
}
