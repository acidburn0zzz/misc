#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <pthread.h>

#include "patterns.h"

#define WIDTH 80
#define HEIGHT 24

int finished = 0;

void msleep(int ms) {
    struct timespec req;
    time_t sec = (ms/1000);
    ms = ms - (sec*1000);

    req.tv_sec=sec;
    req.tv_nsec=ms*1000000L;

    while (nanosleep(&req, &req) == -1)
        continue;
}

void keyboard_watch(void) {
    while (!finished) {
        if (tolower(getch()) == 'q')
            finished = 1;
    }

    return;
}

void print_grid(char grid[HEIGHT][WIDTH]) {
    int x, y;

    for (x=0; x<HEIGHT; x++) {
        for (y=0; y<WIDTH; y++) {
            move(x, y);
            addch(grid[x][y] ? 'x' : ' ');
        }
    }
}


int main(int argc, char **argv) {
    char grid_old[HEIGHT][WIDTH], grid_new[HEIGHT][WIDTH], tmp[HEIGHT][WIDTH];
    int x, y, cnt;

    pthread_t keys_thread;

    srand(time(NULL));

    initscr();
    /* raw(); Line buffering disabled */
    keypad(stdscr, TRUE);
    noecho();
    start_color();

    if (argc > 1 && strcmp("--glider", argv[1]) == 0) {
        memcpy(grid_old, glider_gun, sizeof(char) * HEIGHT * WIDTH);
    } else if (argc > 1 && strcmp("--exploder", argv[1]) == 0) {
        memcpy(grid_old, exploder, sizeof(char) * HEIGHT * WIDTH);
    } else if (argc > 1 && strcmp("--row", argv[1]) == 0) {
        memcpy(grid_old, row, sizeof(char) * HEIGHT * WIDTH);
    } else {
        for (x=0; x<HEIGHT; x++) {
            for (y=0; y<WIDTH; y++) {
                grid_old[x][y] = !(rand()%4);
            }
        }
    }

    pthread_create(&keys_thread, NULL, keyboard_watch, NULL);

    while (!finished) {
        print_grid(grid_old);
        refresh();

        for (x=0; x<HEIGHT; x++) {
            for (y=0; y<WIDTH; y++) {
                cnt = 0;

                /* Up */
                if (x-1 >= 0 && grid_old[x-1][y])
                    cnt++;
                /* Down */
                if (x+1 < HEIGHT && grid_old[x+1][y])
                    cnt++;
                /* Left */
                if (y-1 >= 0 && grid_old[x][y-1])
                    cnt++;
                /* Right */
                if (y+1 < WIDTH && grid_old[x][y+1])
                    cnt++;
                /* Up-Left */
                if (x-1 >= 0 && y-1 >= 0 && grid_old[x-1][y-1])
                    cnt++;
                /* Up-Right */
                if (x-1 >= 0 && y+1 < WIDTH && grid_old[x-1][y+1])
                    cnt++;
                /* Down-Left */
                if (x+1 < HEIGHT && y-1 >= 0 && grid_old[x+1][y-1])
                    cnt++;
                /* Down-Right */
                if (x+1 < HEIGHT && y+1 < WIDTH && grid_old[x+1][y+1])
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

        memcpy(tmp, grid_old, sizeof(char) * HEIGHT * WIDTH);
        memcpy(grid_old, grid_new, sizeof(char) * HEIGHT * WIDTH);
        memcpy(grid_new, tmp, sizeof(char) * HEIGHT * WIDTH);
        msleep(50);
    }

    pthread_join(keys_thread, NULL);
    endwin();

    return EXIT_SUCCESS;
}
