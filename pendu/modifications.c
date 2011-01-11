#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "fichiers.h"
#include "pendu.h"

void edit_words() {
    char words[30][25], nword[25];
    char input[3];
    int i, ch=0;

    get_words(words);
    echo();

    while (ch != -1) {
        clear();

        printw("%-4c%s\n\n",'#',"Mot");
        for (i=0; i<15; i++) {
            printw("%-4d%-26s%-4d%-26s\n", i+1, words[i], i+16, words[i+15]);
        }
        printw("\nVeuillez taper le numero du mot a modifier (\"q\" pour quitter) ");

        scanw("%2s", input);

        if (strcmp("q", input) == 0) {
            ch = -1;
            continue;
        }

        ch = atoi(input);
        if (ch > 0 && ch <= 30) {
            printw("\nVeuillez entrer le nouveau mot : ");
            scanw("%24s", nword);
            while (strlen(nword) == 0) {
                move(21, 34);
                scanw("%24s", nword);
            }
            strcpy(words[ch - 1], nword);
        }
    }

    save_words(words);
    noecho();
}

void edit_scores() {
    char names[20][30];
    char input[3];
    int scores[20];
    int i, ch=0, nb_players;

    get_high_scores(&nb_players, names, scores);
    echo();

    while (ch != -1) {
        clear();

        printw("%-4c%-30s%s\n\n",'#',"Nom","Score");
        for (i=0; i<nb_players; i++) {
            printw("%-4d%-30s%d\n", i+1, names[i], scores[i]);

        }
        printw("\nVeuillez taper le numero du joueur a supprimer (\"q\" pour quitter) ");

        scanw("%2s", input);

        if (strcmp("q", input) == 0) {
            ch = -1;
            continue;
        }

        ch = atoi(input);
        if (ch > 0 && ch <= nb_players) {
            for (i=ch-1; i<nb_players-1; i++) {
                strcpy(names[i], names[i+1]);
                scores[i] = scores[i+1];
            }
            /*
            strcpy(names[ch-1],"");
            scores[iNbJoueur-1] = 0;
            */
            nb_players--;
        }
    }

    save_high_scores(nb_players, names, scores);
    noecho();
}
