#include <stdio.h>
#include <stdlib.h>

int main(void) {
    unsigned int i, key_input;
    unsigned char key;
    FILE *f;
    unsigned char wii_c_key[16] = {
        0xc1, 0xce, 0x00, 0x08,
        0x74, 0xaf, 0xb9, 0xce,
        0x62, 0xf3, 0xef, 0x6f,
        0x59, 0xab, 0x80, 0xdd
    };

    unsigned char unk_c_key[20] = {
        0xb4, 0x36, 0x1a, 0x2c,
        0xfe, 0xe2, 0xdf, 0x34,
        0xd7, 0xd6, 0x1c, 0x9a,
        0x6b, 0x05, 0xd1, 0x77,
        0x38, 0xec, 0xaf, 0x68
    };

    unsigned char wii_u_key[16];
    unsigned char unk_u_key[20];

    printf("Please enter key (42) : ");
    scanf("%u", &key_input);

    key = (unsigned char)(key_input & 0x000000ff); /*Pour avoir une cle de la taille d'un unsigned int*/

    for (i=0; i<16; i++) {
        wii_u_key[i] = wii_c_key[i] ^ key;
    }

    for (i=0; i<20; i++) {
        unk_u_key[i] = unk_c_key[i] ^ key;
    }

    if ((f = fopen("wii_key.bin", "wb")) == NULL) {
        perror("wii_key.bin");
        exit(EXIT_FAILURE);
    }

    if ((fwrite(wii_u_key, sizeof wii_u_key, 1, f)) != 1) {
        perror("wii_key.bin");
        exit(EXIT_FAILURE);
    }

    fclose(f);

    if ((f = fopen("unk_key.bin", "wb")) == NULL) {
        perror("unk_key.bin");
        exit(EXIT_FAILURE);
    }

    if ((fwrite(unk_u_key, sizeof unk_u_key, 1, f)) != 1) {
        perror("unk_key.bin");
        exit(EXIT_FAILURE);
    }

    fclose(f);

    printf("Files \"wii_key.bin\" and \"unk_key.bin\" created successfully.\n");

    return EXIT_SUCCESS;
}
