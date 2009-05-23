#include <malloc.h>
#include <sha2.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int sha256 = 1, i;
    
    sha256_ctx ctx256;
    sha512_ctx ctx512;
    
    unsigned char *hval;
    char s[1024], passwd_hash[65];
    
    /* Ajouter condition pour sha512 */
    
    if (argc == 2) {
        strncpy(s, argv[1], 1023);
        s[1023] = '\0';
    } else {
        printf("Veuillez entrer le mot de passe a crypter: ");
        fgets(s, 1024, stdin);
        for (i=0; i<1024; i++) {
            if (s[i] == '\n') {
                s[i] = '\0';
                break;
            }
        }
    }
    
    if (sha256) {
        hval = malloc(SHA256_DIGEST_SIZE);
        sha256_begin(&ctx256);
        sha256_hash(s, strlen(s), &ctx256);
        sha256_end(hval, &ctx256);
        for (i=0; i<SHA256_DIGEST_SIZE; i++)
            printf("%.2x", hval[i]);
    } else {
        hval = malloc(SHA512_DIGEST_SIZE);
        sha512_begin(&ctx512);
        sha512_hash(s, strlen(s), &ctx512);
        sha512_end(hval, &ctx512);
        for (i=0; i<SHA512_DIGEST_SIZE; i++)
            printf("%.2x", hval[i]);
    }
    
    free(hval);
    printf("\n");
    return 0;
}