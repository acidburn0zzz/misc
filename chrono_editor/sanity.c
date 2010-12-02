/*
 * Secret of Evermore Sanity Fixer
 * Copyright (C) 2006 emuWorks
 * http://games.technoplaza.net/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdint.h>

// define this at compile time on big endian systems (i.e. PPC Mac's)
#ifdef BYTE_ORDER_BIGENDIAN
    #define SWAP_ON_BIGENDIAN(w) ((w << 8) | (w >> 8))
#else
    #define SWAP_ON_BIGENDIAN(w) (w)
#endif

static void fix_game(unsigned char *sram, int g) {
    uint32_t checksum;
    uint16_t *tmp, *offset, over = 0;
    int i;

    checksum = 0x00000000;
    offset = (uint16_t *)(sram + 0x1ff0 + 2 * g);

    for (i=0; i<0x0A00; i+=2) {
        tmp = (uint16_t *)(sram + i + g * 0x0A00);
        checksum += *tmp;
        checksum += over;

        over = 0;

        if (checksum > 0xffff) {
            over = 1;
            checksum &= 0xffff;
        }
    }

    *offset = (uint16_t)checksum;

    printf("fixed checksum for game %d: %.4x\n", (g + 1), (uint16_t)checksum);
}

int main(int argc, char **argv) {
    FILE *f;
    int i;
    char sram[0x2000];

    if (argc != 2) {
        fprintf(stderr, "syntax: %s sram-file.srm\n", argv[0]);
        return -1;
    }

    if ((f = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "error: unable to open SRAM file '%s'\n", argv[1]);
        return -1;
    }

    fseek(f, 0, SEEK_END);

    if (ftell(f) != 0x2000) {
        fprintf(stderr, "error: invalid SRAM file size.\n");
        return -1;
    }

    fseek(f, 0, SEEK_SET);

    if (fread(sram, 0x2000, 1, f) != 1) {
        fprintf(stderr, "error: unable to read SRAM file.\n");
        return -1;
    }

    fclose(f);

    for (i = 0; i < 3; ++i) {
        fix_game(sram, i);
    }

    if ((f = fopen(argv[1], "wb")) == NULL) {
        fprintf(stderr, "error: unable to open SRAM file '%s'\n", argv[1]);
        return -1;
    }

    if (fwrite(sram, 0x2000, 1, f) != 1) {
        fprintf(stderr, "error: unable to write SRAM data to file.\n");
        return -1;
    }

    fclose(f);

    return 0;
}
