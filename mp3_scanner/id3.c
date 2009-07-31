#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "id3.h"

id3_file_s *id3_file_open(char *fn) {
    id3_file_s *id3f = malloc(sizeof(id3_file_s));
    if (id3f == NULL) {
        printf("Impossible d'allouer la memoire\n");
        exit(EXIT_FAILURE);
    }

    id3f->file = fopen(fn, "r");
    if (id3f->file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", fn);
        exit(EXIT_FAILURE);
    }

    return id3f;
}

void id3_file_close(id3_file_s *id3f) {
    fclose(id3f->file);
    free(id3f);
}

id3v1_s *id3_file_read(id3_file_s *id3f) {
    id3v1_s *id3 = malloc(sizeof(id3v1_s));
    fseek(id3f->file, sizeof(id3v1_s) * -1, SEEK_END);

    if (fread(id3, sizeof(id3v1_s), 1, id3f->file) != 1) {
        printf("Impossible de lire dans le fichier\n");
        exit(EXIT_FAILURE);
    }

    if (strncmp(id3->tag, "TAG", 3) != 0) {
        id3f->isID3 = 0;
    } else {
        id3f->isID3 = 1;
    }

    return id3;
}
