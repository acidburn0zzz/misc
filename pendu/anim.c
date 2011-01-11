#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "pendu.h"

char showIntro(int delay) {
    char title[] = "LE BONHOMME PENDU\0";
    char author[] = "PAR MATHIEU LEMAY\0";
    char ch;
    int i;

    attron(COLOR_PAIR(COL_DEF_RED));
    move(10, 35); addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_BTEE); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    move(9, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(8, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(7, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(6, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(5, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(4, 37); addch(ACS_VLINE); refresh(); msleep(delay);
    move(3, 37); addch(ACS_ULCORNER); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_HLINE); refresh(); msleep(delay);
    addch(ACS_URCORNER); refresh(); msleep(delay);
    move(4, 41); addch('O'); refresh(); msleep(delay);
    move(5, 41); addch(ACS_BTEE); refresh(); msleep(delay);
    move(6, 41); addch(ACS_VLINE); refresh(); msleep(delay);
    move(5, 40); addch(ACS_ULCORNER); refresh(); msleep(delay);
    move(5, 42); addch(ACS_URCORNER); refresh(); msleep(delay);
    move(7, 40); addch('/'); refresh(); msleep(delay);
    move(7, 42); addch('\\');
    attroff(COLOR_PAIR(COL_DEF_RED));

    attron(COLOR_PAIR(COL_DEF_BLUE));
    move(16, 23);
    for (i=0; i<strlen(title); i++) {
        refresh();
        msleep(delay);
        printw("%c ", title[i]);
    }
    move(18, 23);
    for (i=0; i<strlen(author); i++) {
        refresh();
        msleep(delay);
        printw("%c ", author[i]);
    }
    attroff(COLOR_PAIR(COL_DEF_BLUE));

    move(22, 19);
    printw("Veuillez appuyer une touche pour continuer");
    refresh();

    ch = getch();

    return ch;
}

void showCow(void) {
    init_pair(COL_DEF_GREEN, COLOR_GREEN, COLOR_BLACK);

    move(4, 55);
    printw("         (__)");
    move(5, 55);
    printw("         (oo)");
    move(6, 55);
    printw("   /------\\/");
    move(7, 55);
    printw("  / |    ||");
    move(8, 55);
    printw(" *  /\\---/\\");
    move(9, 55);
    printw("    ~~   ~~");
    move(10, 55);
    attron(COLOR_PAIR(1));
    printw("~~~~~~~~~~~~~~");
    move(12, 55);
    attroff(COLOR_PAIR(1));
    printw("YOU'VE BEEN MOOOOOED!!!");
    refresh();
}
