#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>

#include "patterns.h"

void gotoxy(int x, int y) {
    char essq[100]; /* String variable to hold the escape sequence */
    char xstr[100]; /* Strings to hold the x and y coordinates */
    char ystr[100]; /* Escape sequences must be built with characters */

    /* Convert the screen coordinates to strings */
    sprintf(xstr, "%d", x);
    sprintf(ystr, "%d", y);

    /* Build the escape sequence (vertical move) */
    essq[0] = '\0';
    strcat(essq, "\033[");
    strcat(essq, ystr);

    /* Described in man terminfo as vpa=\E[%p1%dd
       Vertical position absolute */
    strcat(essq, "d");

    /* Horizontal move
       Horizontal position absolute */
    strcat(essq, "\033[");
    strcat(essq, xstr);

    /* Described in man terminfo as hpa=\E[%p1%dG */
    strcat(essq, "G");

    /* Execute the escape sequence
       This will move the cursor to x, y */
    printf("%s", essq);
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

void clrscr() {
    int i;

    for (i=0; i<100; i++)
        putchar('\n');
}

int main(int argc, char **argv) {
    char grid_old[25][80], grid_new[25][80], tmp[25][80];
    int x, y, cnt;
    int h = 25;
    int w = 80;
    int delay = 50;

    srand(time(NULL));
    clrscr();

    for (x=0; x<h; x++) {
        for (y=0; y<w; y++) {
            grid_old[x][y] = !(rand()%4);
        }
    }

    memcpy(grid_old, glider_gun, sizeof(char) * h * w);

    while (1) {
        for (x=0; x<h; x++) {
            for (y=0; y<w; y++) {
                cnt = 0;

                /* Up */
                if (x-1 >= 0 && grid_old[x-1][y])
                    cnt++;
                /* Down */
                if (x+1 < h && grid_old[x+1][y])
                    cnt++;
                /* Left */
                if (y-1 >= 0 && grid_old[x][y-1])
                    cnt++;
                /* Right */
                if (y+1 < w && grid_old[x][y+1])
                    cnt++;
                /* Up-Left */
                if (x-1 >= 0 && y-1 >= 0 && grid_old[x-1][y-1])
                    cnt++;
                /* Up-Right */
                if (x-1 >= 0 && y+1 < w && grid_old[x-1][y+1])
                    cnt++;
                /* Down-Left */
                if (x+1 < h && y-1 >= 0 && grid_old[x+1][y-1])
                    cnt++;
                /* Down-Right */
                if (x+1 < h && y+1 < w && grid_old[x+1][y+1])
                    cnt++;

                if ((cnt < 2 || cnt > 3) && grid_old[x][y])
                    grid_new[x][y] = 0;
                else if ((cnt == 2 || cnt == 3) && grid_old[x][y])
                    grid_new[x][y] = 1;
                else if (cnt == 3 && !grid_old[x][y])
                    grid_new[x][y] = 1;
                else
                    grid_new[x][y] = 0;
            }
        }

        /* print */
        gotoxy(0, 0);
        for (x=0; x<h; x++) {
            for (y=0; y<w; y++) {
                printf("%c", (grid_new[x][y] ? 'x' : ' '));
            }
            printf("\n");
        }

        memcpy(tmp, grid_old, sizeof(char) * h * w);
        memcpy(grid_old, grid_new, sizeof(char) * h * w);
        memcpy(grid_new, tmp, sizeof(char) * h * w);
        msleep(delay);
    }

    return EXIT_SUCCESS;
}
