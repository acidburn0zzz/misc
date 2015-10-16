#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILE_SIZE 0x10000
#define SAVE_SIZE 0x0A00

uint16_t get_checksum(const uint8_t *data, uint32_t size) {
    uint16_t sum;
    int i;

    sum = 0x0000;

    for (i=0; i<size-2; i++) {
        sum += *(data + i);
    }

    return sum;
}

int main(int argc, char **argv) {
    FILE *f;
    int offset;
    uint8_t data[SAVE_SIZE];
    uint16_t cksum;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s sram_file offset\n", argv[0]);
        return 1;
    }

    offset = atoi(argv[2]);

    if ((f = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "error: unable to open SRAM file '%s'\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);

    if (ftell(f) != FILE_SIZE) {
        fprintf(stderr, "error: invalid SRAM file size.\n");
        return 1;
    }

    fseek(f, offset, SEEK_SET);

    if (fread(data, SAVE_SIZE, 1, f) != 1) {
        fprintf(stderr, "error: unable to read SRAM file.\n");
        return 1;
    }

    fclose(f);

    cksum = get_checksum(data, SAVE_SIZE);

    if ((f = fopen(argv[1], "r+")) == NULL) {
        fprintf(stderr, "error: unable to open SRAM file '%s'\n", argv[1]);
        return -1;
    }

    fseek(f, offset + SAVE_SIZE - 2, SEEK_SET);

    if (fwrite(&cksum, sizeof(cksum), 1, f) != 1) {
        fprintf(stderr, "error: unable to write SRAM data to file.\n");
        return -1;
    }

    fclose(f);

    return 0;
}
