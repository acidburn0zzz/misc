#include "crc32.h"
#include <stdio.h>

/*
void crc32_generate_table() {
   unsigned int crc;
   int i, j;
    
    for (i = 0; i < 256; i++) {
        crc = i;
        for (j = 8; j > 0; j--) {
            if (crc & 1){
                crc = (crc >> 1) ^ 0xedb88320;
            } else {
                crc >>= 1;
            }
        }
        crc_table[i] = crc;
   }
}
*/

void crc32_begin(unsigned int *sum) {
    *sum = 0xffffffff;
}

void crc32_hash(const unsigned char *data, unsigned long len, unsigned int *sum) {
    unsigned long i;
    
    for (i=0; i<len; i++) {
        *sum = (*sum >> 8) ^ crc_table[(*sum ^ data[i]) & 0xff];
    }
}

void crc32_end(unsigned int *sum) {
    *sum = *sum ^ 0xffffffff;
}

int crc32_test() {
    unsigned int foo;
    crc32_begin(&foo);
    crc32_hash((unsigned char*)"Sample string", 13, &foo);
    crc32_end(&foo);
    
    if (foo == 0xa7579504)
        return 1;
    else
        return 0;
}
