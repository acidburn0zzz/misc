/***************************************************************************
 * Copyright (C) 2011 Lemay, Mathieu                                       *
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

 /*
  * AcidIPS 1.0 by Mathieu Lemay
  * This is a simple app to apply ips patches.
  * IPS files specs : http://zerosoft.zophar.net/ips.php
  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define BYTE3_TO_UINT(bp) \
    (((uint32_t)(bp)[0] << 16) & 0x00FF0000) | \
    (((uint32_t)(bp)[1] << 8) & 0x0000FF00) | \
    ((uint32_t)(bp)[2] & 0x000000FF)

#define BYTE2_TO_UINT(bp) \
    (((uint32_t)(bp)[0] << 8) & 0x0000FF00) | \
    ((uint32_t) (bp)[1] & 0x000000FF)

void log_header(FILE *log_file) {
    fprintf(log_file, "Acid IPS Version 1.00\n");
    fprintf(log_file, "Apply IPS Patch Log\n\n");
    fprintf(log_file, "Offset    Size    RLE    IPS Range            Patch Size\n");
}

void log_ips(FILE *log_file, uint32_t offset, uint32_t size, uint32_t rle, uint32_t patch_range_b,
             uint32_t patch_range_e) {
    fprintf(log_file, "%.6X    %4X    %-3s    %.8X-%.8X    %5X\n", offset, rle ? rle : size, rle ? "Yes" : "No",
            patch_range_b, patch_range_e - 1, patch_range_e - patch_range_b);
}

void log_footer(FILE *log_file, uint32_t nb_patches) {
    fprintf(log_file, "\nTotal Patches: %u\n", nb_patches);
}

void fail(const char *fmt, ...) {
    char msg[1024];
    va_list va;

    va_start(va, fmt);
    vsnprintf(msg, sizeof msg, fmt, va);
    fprintf(stderr, "%s\n", msg);

    exit(1);
}

FILE *open_file(char *path, char *mode) {
    FILE *f = fopen(path, mode);

    if (f == NULL) {
        perror(path);
        exit(1);
    }

    return f;
}

void usage(char *cmd) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\tApply patch: %s a <original file> <patch file> <output file>\n", cmd);
    /* fprintf(stderr, "\tCreate patch: %s c <original file> <modified file> <output file>\n", cmd); */
}

int apply_patch(char *s_original_file, char *s_ips_file, char *s_output_file) {
    FILE *f_original_file, *f_ips_file, *f_output_file;
    unsigned char *buffer;

    int eof = 0, count = 0, pos, orig_file_size, i;
    unsigned char offset_c[3], size_c[2];
    uint32_t offset, size, rle, patch_range_a, patch_range_b;

    f_original_file = open_file(s_original_file, "rb");
    f_ips_file = open_file(s_ips_file, "rb");
    f_output_file = open_file(s_output_file, "wb");

    fseek(f_original_file, 0, SEEK_END);
    orig_file_size = ftell(f_original_file);
    fseek(f_original_file, 0, SEEK_SET);

    /* Checking if s_ips_file is really an ips */
    buffer = malloc(5);
    if (fread(buffer, 5, 1, f_ips_file) != 1)
        fail("Unable to read from: %s", s_ips_file);

    if (strncmp((const char *) buffer, "PATCH", 5) != 0)
        fail("%s: Not an IPS patch", s_ips_file);

    log_header(stdout);

    while (!eof) {
        patch_range_a = ftell(f_ips_file);
        if (fread(offset_c, 3, 1, f_ips_file) != 1) {
            fail("Unable to read from: %s", s_ips_file);
        }

        if (strncmp((const char *) offset_c, "EOF", 3) == 0) {
            eof = 1;
            continue;
        }

        offset = BYTE3_TO_UINT(offset_c);

        if (fread(size_c, 2, 1, f_ips_file) != 1) {
            fail("Unable to read from: %s", s_ips_file);
        }

        size = BYTE2_TO_UINT(size_c);

        if (size == 0) {
            if (fread(size_c, 2, 1, f_ips_file) != 1) {
                fail("Unable to read from: %s", s_ips_file);
            }

            size = 1;
            rle = BYTE2_TO_UINT(size_c);
        } else {
            rle = 0;
        }

        /* Read original file from last pos to offset */
        pos = ftell(f_original_file);
        if (pos < offset && offset <= orig_file_size) {
            buffer = realloc(buffer, offset - pos);
            if (fread(buffer, offset - pos, 1, f_original_file) != 1)
                fail("Unable to read from: %s", s_original_file);

            if (fwrite(buffer, offset - pos, 1, f_output_file) != 1)
                fail("Unable to write to: %s", s_output_file);
        }

        /* Read the actual patch */
        buffer = realloc(buffer, size);
        if (fread(buffer, size, 1, f_ips_file) != 1)
            fail("Unable to read from: %s", s_ips_file);

        fseek(f_output_file, offset, SEEK_SET);
        if (rle) {
            for (i=0; i<rle; i++)
                if (fwrite(buffer, size, 1, f_output_file) != 1)
                    fail("Unable to write to: %s", s_output_file);
        } else {
            if (fwrite(buffer, size, 1, f_output_file) != 1)
                fail("Unable to write to: %s", s_output_file);
        }

        /* Put the pos indicator from f_original_file to the same pos as f_output_file if possible */
        pos = ftell(f_output_file);
        fseek(f_original_file, pos, SEEK_SET);

        patch_range_b = ftell(f_ips_file);
        log_ips(stdout, offset, size, rle, patch_range_a, patch_range_b);
        count++;
    }

    /* Fill end of the original file */
    pos = ftell(f_original_file);
    if (pos < orig_file_size) {
        buffer = realloc(buffer, orig_file_size - pos);
        if (fread(buffer, orig_file_size - pos, 1, f_original_file) != 1)
            fail("Unable to read from: %s", s_original_file);

        if (fwrite(buffer, orig_file_size - pos, 1, f_output_file) != 1)
            fail("Unable to write to: %s", s_output_file);
    }

    log_footer(stdout, count);

    free(buffer);
    fclose(f_ips_file);
    fclose(f_output_file);

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 5 || strlen(argv[1]) > 1) {
        usage(argv[0]);
        return 1;
    }

    if (tolower(argv[1][0]) == 'a') {
        return apply_patch(argv[2], argv[3], argv[4]);
    } else if (tolower(argv[1][0]) == 'c') {
        /* Create */
        /* return create_patch(argv[2], argv[3], argv[4]); */
    } else {
        usage(argv[0]);
        return 1;
    }
    return 0;
}
