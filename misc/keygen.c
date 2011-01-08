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

    unsigned char wii_u_key[16];

    printf("Please enter key (42) : ");
    if (scanf("%u", &key_input) != 1) {
        fprintf(stderr, "Error reading from stdin\n");
        exit(1);
    }

    key = (unsigned char)(key_input & 0x000000ff); /*Pour avoir une cle de la taille d'un unsigned int*/

    for (i=0; i<16; i++) {
        wii_u_key[i] = wii_c_key[i] ^ key;
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

    printf("File \"wii_key.bin\" created successfully.\n");

    return EXIT_SUCCESS;
}
