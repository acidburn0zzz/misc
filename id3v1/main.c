#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

void showHelp() {
    printf("HELP !!!\n");
}

int main(int argc, char *argv[]) {
    FILE *mp3;
    struct ID3 id3;

    int b_id3=0, opt, f_show=0, f_verbose=0, index;
    unsigned char *title=NULL, *artist=NULL, *album=NULL, *year=NULL, *comment=NULL, track='\0', *genre=NULL, *filename=NULL;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s options mp3_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*Options*/
    while ((opt = getopt(argc, argv, "a:c:g:hl:r:st:vy:")) != -1) {
        switch (opt) {
            case 'a':
                artist = optarg;
                break;
            case 'c':
                comment = optarg;
                break;
            case 'g':
                genre = optarg;
                break;
            case 'h':
                showHelp();
                exit(EXIT_SUCCESS);
            case 'l':
                album = optarg;
                break;
            case 'r':
                track = optarg[0] - '0';
                break;
            case 's':
                f_show = 1;
                break;
            case 't':
                title = optarg;
                break;
            case 'v':
                f_verbose = 1;
                break;
            case 'y':
                year = optarg;
                break;
        }
    }

    if (optind != argc-1) {
        fprintf(stderr, "No file specified.\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[argc-1];


    /*Lire les 128 derniers bits du fichier*/
    if ((mp3 = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "File %s not found.\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(mp3, -128, SEEK_END);

    if (fread(&id3, sizeof(id3), 1, mp3) == 0) {
        fprintf(stderr, "Impossible to read from %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fclose(mp3);

    /*Verification si le tag existe*/
    if (id3.tag[0] == 'T' && id3.tag[1] == 'A' && id3.tag[2] == 'G') {
        b_id3 = 1;
    }

    /*Afficher les infos si "-s" et quitter*/
    if (f_show) {
        if (b_id3) {
            fprintf(stdout, "File : %s\n", filename);
            fprintf(stdout, "Title : %.30s\n", id3.title);
            fprintf(stdout, "Artist : %.30s\n", id3.artist);
            fprintf(stdout, "Album : %.30s\n", id3.album);
            fprintf(stdout, "Year : %.4s\n", id3.year);
            fprintf(stdout, "Comment : %.28s\n", id3.comment);
            fprintf(stdout, "Track : %d\n", id3.track);
            fprintf(stdout, "Genre : %d\n", id3.genre);
        } else {
            fprintf(stdout, "File %s doesn't contain ID3v1.1 tag.\n", argv[1]);
        }
        exit(EXIT_SUCCESS);
    }

    return 0;
}
