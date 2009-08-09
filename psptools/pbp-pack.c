#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct {
    char magic[4];
    char version[4];
    int  offsets[8];
} header = {
    {'\0', 'P', 'B', 'P'},
    {0x00, 0x00, 0x01, 0x00},
    {40, 0, 0, 0, 0, 0, 0, 0}
};

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

int main(int argc, char **argv) {
    FILE *infile, *outfile;
    int size;
    int MAX_BUFFER = 16*1024*1024, taille_buffer;
    unsigned char *buffer;

    int i;

    if (argc != 9) {
        fprintf(stderr, "Usage: %s <%s> <%s> <%s> <%s> <%s> <%s> <%s> <%s>\n", argv[0], filename[0],
            filename[1], filename[2], filename[3], filename[4], filename[5], filename[6], filename[7]);
        return EXIT_FAILURE;
    }

    /* Ouverture du fichier EBOOT.PBP */
    outfile = fopen("EBOOT.PBP", "wb");
    if (outfile == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier pour ecriture : EBOOT.PBP\n");
        return EXIT_FAILURE;
    }

    /* On parcourt les fichiers en argument */
    for (i=1; i<=8; i++) {
        if (strcmp(argv[i], "NULL") == 0 || strcmp(argv[i], "-") == 0) {
            size = 0;
			infile = NULL;
        } else {
            /* Ouverture du fichier a placer dans le EBOOT */
            infile = fopen(argv[i], "rb");
            if (infile == NULL) {
                fprintf(stderr, "Impossible d'ouvrir le fichier pour lecture : %s\n", argv[i]);
                return EXIT_FAILURE;
            }

            /* Taille du fichier */
            fseek(infile, 0, SEEK_END);
            size = ftell(infile);
            fseek(infile, 0, SEEK_SET);
            if (size < 0) {
                fprintf(stderr, "Impossible de savoir la taille du fichier : %s\n", argv[i]);
                return EXIT_FAILURE;
            }
        }

        /* On ajoute l'offset pour le fichier suivant */
        /* Offset de ce fichier plus taille de ce fichier */
        if (i != 8) {
            header.offsets[i] = header.offsets[i-1] + size;
        }

        /* On passe au suivant si le fichier est vide */
        if (size == 0)
            continue;

        /* On s'assure d'etre a la bonne place pour l'ecriture */
        if (fseek(outfile, header.offsets[i-1], SEEK_SET) != 0) {
            printf("Impossible de se deplacer dans le fichier: EBOOT.PBP\n");
            return EXIT_FAILURE;
        }

        /* Boucle d'ecriture du fichier */
        while (size > 0) {
            if (size < MAX_BUFFER) {
                taille_buffer = size;
            } else {
                taille_buffer = MAX_BUFFER;
            }
            size -= taille_buffer;

            /* Allocation de la memoire */
            buffer = malloc(taille_buffer);
            if (buffer == NULL) {
                fprintf(stderr, "Impossible d'initialiser le buffer\n");
                return EXIT_FAILURE;
            }

            /* Lecture dans le buffer */
            if (fread(buffer, taille_buffer, 1, infile) != 1) {
                fprintf(stderr, "Erreur de lecture dans le fichier: %s\n", argv[i]);
                return EXIT_FAILURE;
            }

            /* Ecriture du buffer */
            if (fwrite(buffer, taille_buffer, 1, outfile) != 1) {
                fprintf(stderr, "Erreur d'ecriture dans le fichier: %s\n", filename[i]);
                return EXIT_FAILURE;
            }
        }

		fclose(infile);
    }

	/* On retourne au debut du EBOOT */
	if (fseek(outfile, 0, SEEK_SET) != 0) {
		printf("Impossible de se deplacer dans le fichier: EBOOT.PBP\n");
		return EXIT_FAILURE;
	}

	/* Ecriture du header */
	if (fwrite((char*)&header, sizeof(header), 1, outfile) != 1) {
		fprintf(stderr, "Erreur d'ecriture dans le fichier: EBOOT.PBP\n");
		return EXIT_FAILURE;
	}

	fclose(outfile);

    return EXIT_SUCCESS;
}
