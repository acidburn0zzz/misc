#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
    char magic[4];
    char version[4];
    int  offsets[8];
} s_header;

/* Fichiers dans le PBP */
char *filename[8] = {
   "PARAM.SFO",
   "ICON0.PNG",
   "ICON1.PMF",
   "PIC0.PNG",
   "PIC1.PNG",
   "SND0.AT3",
   "DATA.PSP",
   "DATA.PSAR"
};

char good_magic[4] = {'\0', 'P', 'B', 'P'};

int main(int argc, char **argv) {
    FILE *infile, *outfile;
    s_header header;
    int size;
    int MAX_BUFFER = 16*1024*1024;

    int i, dummy;

    /* Verif si le nom du fichier a extraire est specifie */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Ouverture du fichier */
    infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        fprintf(stderr, "Fichier introuvable: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    fseek(infile, 0, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    /* Taille du fichier PBP */
    if (size < 0) {
        fprintf(stderr, "Impossible d'avoir la taille du fichier: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Recuperation de l'en-tete */
    if ((dummy = fread(&header, sizeof(s_header), 1, infile)) < 0) {
        fprintf(stderr, "Impossible de lire l'en-tete du fichier: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Verification du magic number */
    for (i=0; i<sizeof(good_magic); i++) {
        if (header.magic[i] != good_magic[i]) {
            fprintf(stderr, "Le fichier n'est pas un PBP: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    for (i=0; i<8; i++) {
        int reste;
        unsigned char *buffer;
        if (i == 7)
            reste = size - header.offsets[i];
        else
            reste = header.offsets[i+1] - header.offsets[i];

        /* Information sur le fichier */
        printf("[%i] %-12s | %11i\n", i, filename[i], reste);

        /* Ignorer les fichiers vides */
        if (!reste)
            continue;

        /* Ouverture du ficher de sortie */
        outfile = fopen(filename[i], "wb");
        if (outfile == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier pour ecriture: %s\n", filename[i]);
            return EXIT_FAILURE;
        }

        /* On se rend au bon offset */
        if (fseek(infile, header.offsets[i], SEEK_SET) != 0) {
            printf("Impossible de se deplacer dans le fichier: %s\n", argv[1]);
            return EXIT_FAILURE;
        }

        /* Extraction du fichier */
        while (reste > 0) {
            int taille_buffer;

            if (reste < MAX_BUFFER) {
                taille_buffer = reste;
            } else {
                taille_buffer = MAX_BUFFER;
            }

            reste -= taille_buffer;

            /* Verification du buffer */
            buffer = malloc(taille_buffer);
            if (buffer == NULL) {
                fprintf(stderr, "Impossible d'initialiser le buffer\n");
                return EXIT_FAILURE;
            }

            /* Lecture dans le buffer */
            if ((dummy = fread(buffer, taille_buffer, 1, infile)) != 1) {
                fprintf(stderr, "Erreur de lecture dans le fichier: %s\n", argv[1]);
                return EXIT_FAILURE;
            }

            /* Ecriture du buffer */
            if ((dummy = fwrite(buffer, taille_buffer, 1, outfile)) != 1) {
                fprintf(stderr, "Erreur d'ecriture dans le fichier: %s\n", filename[i]);
                return EXIT_FAILURE;
            }

            free(buffer);
        }

        /* Fermeture du fichier */
        fclose(outfile);
    }

    fclose(infile);

    return EXIT_SUCCESS;
}
