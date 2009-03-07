#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned int i, c_read;
    unsigned char c;
    FILE *f;
    unsigned char c_key[16] = {
        0xc1, 0xce, 0x00, 0x08,
        0x74, 0xaf, 0xb9, 0xce,
        0x62, 0xf3, 0xef, 0x6f,
        0x59, 0xab, 0x80, 0xdd
    };
    unsigned char u_key[16];
    
    printf("Veuillez entrer la cle de decryptage (42) : ");
    scanf("%u", &c_read);
    
    c = (unsigned char)(c_read & 0x000000ff); /*Pour avoir une cle de la taille d'un unsigned int*/
    
    printf("%d\n", c);
    
    for (i=0; i<16; i++) {
        u_key[i] = c_key[i] ^ c;
    }
    
    if ((f = fopen("key.bin", "wb")) == NULL) {
        perror("key.bin");
        exit(EXIT_FAILURE);
    }
    
    if ((fwrite(u_key, sizeof u_key, 1, f)) != 1) {
        perror("key.bin");
        exit(EXIT_FAILURE);
    }     
    
    fclose(f);
    
    printf("Le fichier \"key.bin\" a ete cree avec success.\n");
    
    return EXIT_SUCCESS;
}
