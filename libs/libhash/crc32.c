/***************************************************************************
 * Copyright (C) 2009 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/
#include <stdio.h>
#include <string.h>

#include "crc32.h"

#define BUFFER_SIZE 16384

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

void crc32_begin(uint32_t *sum) {
    *sum = 0xffffffff;
}

void crc32_hash(const unsigned char *data, unsigned int len, uint32_t *sum) {
    while (len--)
        *sum = (*sum >> 8) ^ crc_table[(*sum ^ *data++) & 0xff];
}

void crc32_end(uint32_t *sum) {
    *sum = *sum ^ 0xffffffff;
}

unsigned int crc32_hash_string(unsigned char *str) {
    uint32_t crc32;

    crc32_begin(&crc32);
    crc32_hash(str, strlen((char*)str), &crc32);
    crc32_end(&crc32);

    return crc32;
}

int crc32_hash_file(char *fn, uint32_t *sum) {
    FILE *f;
    unsigned int size;
    unsigned char buffer[BUFFER_SIZE];

    if (!(f = fopen(fn, "rb"))) {
        perror(fn);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    crc32_begin(sum);

    while (size > 0) {
        if (size < BUFFER_SIZE) {
            if (fread(buffer, size, 1, f) != 1) {
                perror(fn);
                return -1;
            }
            crc32_hash(buffer, size, sum);
            size = 0;
        } else {
            if (fread(buffer, BUFFER_SIZE, 1, f) != 1) {
                perror(fn);
                return -1;
            }
            crc32_hash(buffer, BUFFER_SIZE, sum);
            size -= BUFFER_SIZE;
        }
    }

    fclose(f);

    crc32_end(sum);

    return 0;
}

int crc32_test() {
    uint32_t foo;
    crc32_begin(&foo);
    crc32_hash((unsigned char*)"Sample string", 13, &foo);
    crc32_end(&foo);

    if (foo == 0xa7579504)
        return 0;
    else
        return -1;
}
