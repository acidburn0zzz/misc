#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include <libg15.h>
#include <libg15render.h>
#include <g15daemon_client.h>

#include "pid.h"

#define true 1
#define false 0

int finished = 0;
int g15screen_fd;
g15canvas *canvas;
char pid_file[256], song_file[256];

unsigned int textSize = G15_TEXT_HUGE;

int show_playing_song();
void reset_screen();

void signal_handler(int sig) {
    switch (sig) {
        case SIGUSR1:
            show_playing_song();
            break;
        case SIGUSR2:
            reset_screen();
            break;
        case SIGINT:
        case SIGKILL:
        case SIGTERM:
            finished = 1;
            break;
    }
}

void keyboard_watch(void) {
    unsigned int keystate;

    while (!finished) {
        recv(g15screen_fd, &keystate, 4, 0);

        if(keystate & G15_KEY_L1) {
        } else if(keystate & G15_KEY_L2) {
        } else if(keystate & G15_KEY_L3) {
        } else if(keystate & G15_KEY_L4) {
            if (textSize > G15_TEXT_SMALL) {
                textSize--;
                show_playing_song();
            }
        } else if(keystate & G15_KEY_L5) {
            if (textSize < G15_TEXT_HUGE) {
                textSize++;
                show_playing_song();
            }
        }

        usleep(100 * 900);
    }

    return;
}

int show_playing_song() {
    FILE *fp;
    long fsize;
    unsigned char *buffer;
    char *rowdata;
    int row = 0;

    fp = fopen(song_file, "r");
    if (fp == NULL) {
        perror(song_file);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = malloc(sizeof(unsigned char) * (fsize + 1));
    if (buffer == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, fsize, 1, fp) != 1) {
        fprintf(stderr, "Unable to read from file\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    buffer[fsize] = '\0';

    g15r_clearScreen(canvas, G15_COLOR_WHITE);

    rowdata = strtok((char *) buffer, ";");
    while (rowdata != NULL) {
        g15r_renderString(canvas, (unsigned char *) rowdata, row, textSize, 0, 0);
        rowdata = strtok(NULL, ";");
        row++;
    }

    g15_send(g15screen_fd, (char *) canvas->buffer, G15_BUFFER_LEN);

    free(buffer);
    return true;
}

void reset_screen() {
    g15r_clearScreen(canvas, G15_COLOR_WHITE);
    g15r_renderString(canvas, (unsigned char *) "Nothing playing", 0, G15_TEXT_LARGE, 0, 0);
    g15_send(g15screen_fd, (char *) canvas->buffer, G15_BUFFER_LEN);
}

int main(int argc, char **argv) {
    struct sigaction act;
    pid_t pid;
    int i, daemonize_f = 0, kill_f = 0;

    pthread_t keys_thread;

    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            daemonize_f = 1;
        else if (strcmp(argv[i], "-k") == 0)
            kill_f = 1;
    }

    snprintf(pid_file, 256, "%s/.g15music.pid", "/tmp");
    snprintf(song_file, 256, "%s/.g15music_song", "/tmp");

    pid = is_app_running("g15music", pid_file);

    if (kill_f) {
        if (pid > 0)
            kill(pid, SIGINT);
        return EXIT_SUCCESS;
    }

    if (pid != 0) {
        fprintf(stderr, "G15Music already running\n");
        return EXIT_FAILURE;
    }

    if (daemonize_f) {
        pid = fork();

        if (pid != 0) {
            return EXIT_SUCCESS;
        }
    }

    printf("Running at pid %d\n", getpid());
    write_pid(pid_file);

    if (daemonize_f) {
        if (freopen("/dev/null", "r", stdin) == NULL) {
            perror("stdin");
            return EXIT_FAILURE;
        }

        if (freopen("/dev/null", "w", stdout) == NULL) {
            perror("stderr");
            return EXIT_FAILURE;
        }

        if (freopen("/dev/null", "w", stderr) == NULL) {
            perror("stderr");
            return EXIT_FAILURE;
        }
    }

    if ((g15screen_fd = new_g15_screen(G15_G15RBUF)) < 0) {
        fprintf(stderr, "Unable to connect to G15daemon\n");
        return EXIT_FAILURE;
    }

    canvas = malloc(sizeof(g15canvas));

    if (canvas != NULL)
        g15r_initCanvas(canvas);
    else
        return EXIT_FAILURE;

    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    pthread_create(&keys_thread, NULL, (void*)keyboard_watch, NULL);

    if (!show_playing_song())
        reset_screen();

    while (!finished) {
        pause();
    }

    pthread_join(keys_thread, NULL);

    close(g15screen_fd);

    unlink(pid_file);
    return EXIT_SUCCESS;
}
