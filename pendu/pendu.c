#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <ncurses.h>

#include "anim.h"
#include "fichiers.h"
#include "modifications.h"
#include "pendu.h"

int play(void);

WINDOW *create_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW *win);

int main(int argc, char **argv) {
    char ch;
    int end;

    initscr();
    /* raw(); Line buffering disabled */
    keypad(stdscr, TRUE);
    noecho();
    start_color();

    end = 0;
    while (!end) {
        clear();
        move(7, 25);
        printw("Veuillez choisir une option :");
        move(8, 25);
        printw("1. Jouer au bonhomme pendu");
        move(9, 25);
        printw("2. Afficher/modifier les mots");
        move(10, 25);
        printw("3. Afficher/supprimer les scores");
        move(11, 25);
        printw("4. Quitter");
        refresh();
        move(7, 55);
        ch = getch();

        switch (ch) {
        case '1':
            play();
            break;
        case '2':
            edit_words();
            break;
        case '3':
            edit_scores();
            break;
        case '4':
            end = 1;
        }
    }

    endwin();

    return 0;
}

int play(void) {
    const int NB_MOT = 30;

    char cLesNomComplets[20][30];
    char cLesMots[30][25];
    char cCode;

    int iLesPointages[20];
    int iNbJoueur, finished = 0;

    char cNomJoueur[30], cLettresChoisies[27], cMot[25], cMessage[75], cNomTemp[30];
    char cLettre;
    int iVies, iNbLettreTrouve, iEssai, iTemp, i, j;
    int iErreur, iLettreTrouvee, iDejaDansListe=0;

    srand(time(NULL));
    clear();

    get_words(cLesMots);
    get_high_scores(&iNbJoueur, cLesNomComplets, iLesPointages);

    init_pair(COL_DEF_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COL_DEF_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COL_DEF_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COL_DEF_BLUE, COLOR_BLUE, COLOR_BLACK);

    cCode = showIntro(100);

    clear();
    echo();
    move(12, 27);
    printw("Veuillez entrer votre nom : ");
    move(13, 27);
    scanw("%29s", cNomJoueur);

    while (strlen(cNomJoueur) == 0) {
        move(12, 27);
        printw("Veuillez entrer un nom valide : ");
        move(13, 27);
        scanw("%29s", cNomJoueur);
    }
    noecho();

    while (!finished) {
        /* Choix du mot */
        strcpy(cMot, cLesMots[rand()%NB_MOT]);

        iVies = 6;
        iEssai = 0;
        iNbLettreTrouve=0;
        strncpy(cLettresChoisies, "", 27);
        /*
        strcpy n'effacait que le premier caractere tandis que
        strncpy copie le premier caractere et remplit le reste
        de caracteres nuls, d'ou le 27
        http://www.linux-kheops.com/doc/man/manfr/man-html-0.9/man3/strcpy.3.html
        */

        clear();
        printw("Bonjour %s", cNomJoueur);
        if (cCode == '!') {
            printw("\n%s", cMot);
        }

        /* dessin de la potence */
        move(3, 20);
        addch(ACS_ULCORNER);
        addch(ACS_HLINE);
        addch(ACS_HLINE);
        addch(ACS_HLINE);
        addch(ACS_URCORNER);
        move(4, 20);
        addch(ACS_VLINE);
        move(5, 20);
        addch(ACS_VLINE);
        move(6, 20);
        addch(ACS_VLINE);
        move(7, 20);
        addch(ACS_VLINE);
        move(8, 20);
        addch(ACS_VLINE);
        move(9, 20);
        addch(ACS_VLINE);
        move(10, 18);
        addch(ACS_HLINE);
        addch(ACS_HLINE);
        addch(ACS_BTEE);
        addch(ACS_HLINE);
        addch(ACS_HLINE);
        /* fin du dessin */

        /* Nombre de lettres du mot et affichage des lignes */
        move(13, 15);
        printw("Votre mot contient %d lettres", strlen(cMot));
        for (i=0; i<strlen(cMot); i++) {
            move(15, 15+2*i);
            printw("_");
        }
        move(18, 15);
        printw("Veuillez entrer une lettre : ");

        /* Boucle principale du jeu */
        while (iVies > 0 && (unsigned)iNbLettreTrouve < strlen(cMot)) {
            iErreur = 0;
            iLettreTrouvee = 0;
            move(22, 15);
            printw("Lettres choisies : %s", cLettresChoisies);

            move(18, 44);
            cLettre = tolower(getch());
            while ((cLettre < 'a' || cLettre > 'z') && cLettre != '!' ) {
                /* verifie si le caractere entre est une valide
                   les caracteres sont les lettres, ! pour la vache */
                message_box("Veuillez entrer une lettre", MB_OK);
                cLettre = tolower(getch());
            }

            if (cLettre == '!') {
                showCow();
            } else {
                for (i=0; i<strlen(cLettresChoisies); i++) { /* verifie si le caractere a deja ete entree */
                    if (cLettresChoisies[i] == cLettre) {
                        iErreur = 1;
                        message_box("Lettre deja choisie", MB_OK);
                    }
                }

                if (iErreur == 0) {                       /* verifie si la lettre entree est dans le mot */
                    cLettresChoisies[iEssai] = cLettre;   /* et affiche la lettre aux bons endroits */
                    iEssai++;
                    for (i=0; i<strlen(cMot); i++) {
                        if (tolower(cMot[i]) == cLettre) {
                            move(15, 15+2*i);
                            printw("%c", cMot[i]);
                            iNbLettreTrouve++;
                            iLettreTrouvee=1;
                        }
                    }
                    if (iLettreTrouvee == 0) {  /* enleve une vie et dessine le bonhomme */
                        iVies--;                /* si la lettre n'etait pas dans le mot */
                        switch (iVies) {
                        case 5:
                            attron(COLOR_PAIR(COL_DEF_GREEN));
                            move(4, 24);
                            addch('O');
                            attroff(COLOR_PAIR(COL_DEF_GREEN));
                            break;
                        case 4:
                            attron(COLOR_PAIR(COL_DEF_GREEN));
                            move(5, 24);
                            addch(ACS_BTEE);
                            move(6, 24);
                            addch(ACS_VLINE);
                            attroff(COLOR_PAIR(COL_DEF_GREEN));
                            break;
                        case 3:
                            attron(COLOR_PAIR(COL_DEF_YELLOW));
                            move(4, 24);
                            addch('O');
                            move(5, 24);
                            addch(ACS_BTEE);
                            move(6, 24);
                            addch(ACS_VLINE);
                            move(5, 23);
                            addch(ACS_ULCORNER);
                            attroff(COLOR_PAIR(COL_DEF_YELLOW));
                            break;
                        case 2:
                            attron(COLOR_PAIR(COL_DEF_YELLOW));
                            move(5, 25);
                            addch(ACS_URCORNER);
                            attroff(COLOR_PAIR(COL_DEF_YELLOW));
                            break;
                        case 1:
                            attron(COLOR_PAIR(COL_DEF_RED));
                            move(4, 24);
                            addch('O');
                            move(5, 24);
                            addch(ACS_BTEE);
                            move(6, 24);
                            addch(ACS_VLINE);
                            move(5, 23);
                            addch(ACS_ULCORNER);
                            move(5, 25);
                            addch(ACS_URCORNER);
                            move(7, 23);
                            addch('/');
                            attroff(COLOR_PAIR(COL_DEF_RED));
                            break;
                        case 0:
                            attron(COLOR_PAIR(COL_DEF_RED));
                            move(7, 25);
                            addch('\\');
                            attroff(COLOR_PAIR(COL_DEF_RED));
                            move(18, 44);
                        }
                    }
                }
            }
        }

        if (iVies == 0) {
            strcpy(cMessage, "Vous avez perdu, le mot etait : ");
            strcat(cMessage, cMot);
            message_box(cMessage, MB_OK);
        } else {
            message_box("Vous avez trouve le mot", MB_OK);
            /* on verifie si le nom est deja dans la liste */
            for (i=0; i<iNbJoueur; i++) {
                if (strcmp(cNomJoueur, cLesNomComplets[i]) == 0) {
                    iLesPointages[i]++;
                    iDejaDansListe = 1;
                    break;
                }
            }
            if (iDejaDansListe == 0 && iNbJoueur<20) {
                strcpy(cLesNomComplets[iNbJoueur], cNomJoueur);
                iLesPointages[iNbJoueur] = 1;
                iNbJoueur++;
            } else if (iDejaDansListe == 0 && iNbJoueur == 20) {
                message_box("La liste de scores est pleine", MB_OK);
            }
        }

        for (i=0; i<iNbJoueur-1; i++) {
            for (j=i+1; j<iNbJoueur; j++) {
                if (iLesPointages[i] < iLesPointages[j]) {
                    iTemp = iLesPointages[i];
                    iLesPointages[i] = iLesPointages[j];
                    iLesPointages[j] = iTemp;
                    strcpy(cNomTemp, cLesNomComplets[i]);
                    strcpy(cLesNomComplets[i], cLesNomComplets[j]);
                    strcpy(cLesNomComplets[j], cNomTemp);
                }
            }
        }

        /* Affichage des scores */
        clear();
        printw("%-3s%-30s%s\n\n","#","Nom","Score");
        for (i=0; i<iNbJoueur; i++) {
            printw("%-3d%-30s%d\n", i+1, cLesNomComplets[i], iLesPointages[i]);

        }

        if (message_box("Voulez-vous rejouer (o/n)?", MB_YESNO) == ANS_NO)
            finished = 1;
    }

    save_high_scores(iNbJoueur, cLesNomComplets, iLesPointages);

    return 0;
}

int message_box(char *message, int type) {
    int h, w, y, x, ret, cur_x, cur_y;
    char ch;
    WINDOW *win;

    refresh();
    getyx(stdscr, cur_x, cur_y);
    curs_set(0);

    h = 3;
    w = strlen(message) + 4;
    y = (LINES - h) / 2;
    x = (COLS - w) / 2;
    win = create_win(h, w, y, x);

    mvwprintw(win, 1, 2, message);

    if (type == MB_OK) {
        wgetch(win);
        ret = ANS_NONE;
    } else {
        while (tolower(ch = wgetch(win)) != 'y' && ch != 'o' && ch != 'n');

        if (ch == 'y' || ch == 'o')
            ret = ANS_YES;
        else
            ret = ANS_NO;
    }

    destroy_win(win);
    move(cur_x, cur_y);
    curs_set(1);

    return ret;
}

WINDOW *create_win(int height, int width, int starty, int startx) {
    WINDOW *win;

    win = newwin(height, width, starty, startx);
    box(win, 0 , 0);    /*
                         * 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines
                         */
    wrefresh(win);  /* Show that box */

    return win;
}

void destroy_win(WINDOW *win) {
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void msleep(int ms) {
    struct timespec req;
    time_t sec = (ms/1000);
    ms = ms - (sec*1000);

    req.tv_sec=sec;
    req.tv_nsec=ms*1000000L;

    while (nanosleep(&req, &req) == -1)
        continue;
}

void quit(int ret, char *errmsg) {
    endwin();

    if (errmsg)
        fprintf(stderr, "%s\n", errmsg);

    exit(ret);
}
