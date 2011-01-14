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

    unsigned char dsi_c_key[16] = {
        0x85, 0x31, 0xdf, 0x3c,
        0x82, 0x2d, 0xf8, 0x30,
        0xc0, 0x6f, 0xb2, 0x65,
        0x2e, 0x5e, 0x02, 0x4b
    };

    unsigned char wii_u_key[16];
    unsigned char dsi_u_key[16];

    printf("Answer to the Ultimate Question of Life, the Universe and Everything: ");
    if (scanf("%u", &key_input) != 1) {
        fprintf(stderr, "Error reading from stdin\n");
        exit(1);
    }


    key = (unsigned char)(key_input & 0x000000ff); /*Pour avoir une cle de la taille d'un unsigned int*/

    for (i=0; i<16; i++) {
        wii_u_key[i] = wii_c_key[i] ^ key;
    }

    if ((f = fopen("wiikey.bin", "wb")) == NULL) {
        perror("wiikey.bin");
        exit(EXIT_FAILURE);
    }

    if ((fwrite(wii_u_key, sizeof wii_u_key, 1, f)) != 1) {
        perror("wiikey.bin");
        exit(EXIT_FAILURE);
    }

    fclose(f);

    for (i=0; i<20; i++) {
        dsi_u_key[i] = dsi_c_key[i] ^ key;
    }

    if ((f = fopen("dsikey.bin", "wb")) == NULL) {
        perror("dsikey.bin");
        exit(EXIT_FAILURE);
    }

    if ((fwrite(dsi_u_key, sizeof dsi_u_key, 1, f)) != 1) {
        perror("dsikey.bin");
        exit(EXIT_FAILURE);
    }

    fclose(f);

    printf("Files \"wiikey.bin\" and \"dsikey.bin\" created successfully.\n");

    return EXIT_SUCCESS;
}
