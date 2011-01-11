#include <stdio.h>
#include <stdlib.h>

#include "fichiers.h"
#include "pendu.h"

void get_high_scores(int *nb_players, char names[20][30], int scores[20]) {
    FILE *f;
    int i;

    f = fopen("scores.bin", "rb");
    if (f == NULL) {
        quit(EXIT_FAILURE, "Impossible d'ouvrir le fichier scores.bin pour lecture");
    }

    if (fread((void*) nb_players, sizeof(*nb_players), 1, f) != 1) {
        message_box("Fichier de scores corrompu", MB_OK);
        *nb_players = 0;
        fclose(f);
        return;
    }

    for (i=0; i<*nb_players; i++) {
        if (fread(&names[i], sizeof(names[i]), 1, f) != 1) {
            message_box("Fichier de scores corrompu", MB_OK);
            *nb_players = 0;
            fclose(f);
            return;
        }
        if (fread(&scores[i], sizeof(scores[i]), 1, f) != 1) {
            message_box("Fichier de scores corrompu", MB_OK);
            *nb_players = 0;
            fclose(f);
            return;
        }
    }

    fclose(f);
}

void save_high_scores(int nb_players, char names[20][30], int scores[20]) {
    FILE *f;
    int i;

    f = fopen("scores.bin", "wb");
    if (f == NULL) {
        quit(EXIT_FAILURE, "Impossible d'ouvrir le fichier scores.bin pour ecriture");
    }

    fwrite(&nb_players, sizeof(nb_players), 1, f);

    for (i=0; i<nb_players; i++) {
        fwrite(&names[i], sizeof(names[i]), 1, f);
        fwrite(&scores[i], sizeof(scores[i]), 1, f);
    }

    fclose(f);
}

void get_words(char words[30][25]) {
    FILE *f;
    int i;

    f = fopen("words.bin", "rb");
    if (f == NULL) {
        quit(EXIT_FAILURE, "Impossible d'ouvrir le fichier words.bin");
    }

    for (i=0; i<30; i++)
        if (fread(&words[i], sizeof(words[i]), 1, f) != 1)
            quit(EXIT_FAILURE, "Fichier de mots corrompu");

    fclose(f);
}

void save_words(char words[30][25]) {
    FILE *f;
    int i;

    f = fopen("words.bin", "wb");
    if (f == NULL) {
        quit(EXIT_FAILURE, "Impossible d'ouvrir le fichier words.bin");
    }

    for (i=0; i<30; i++)
        fwrite(&words[i], sizeof(words[i]), 1, f);

    fclose(f);
}
