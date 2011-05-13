#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include <libg15.h>
#include <libg15render.h>
#include <g15daemon_client.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "pid.h"

#define true 1
#define false 0

#define NAME "G15HellaNZB"
#define VERSION "1.0"

int finished = 0;
int g15screen_fd;
g15canvas *canvas;
char pid_file[] = "/tmp/g15hella.pid";

unsigned int textSize = G15_TEXT_MED;

void hella_loop();
void reset_screen();

void signal_handler(int sig) {
    switch (sig) {
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
            }
        } else if(keystate & G15_KEY_L5) {
            if (textSize < G15_TEXT_LARGE) {
                textSize++;
            }
        }

        usleep(100000);
    }

    return;
}

static void die_if_fault_occurred(xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "XML-RPC Fault: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

void hella_loop() {
    xmlrpc_env env;
    xmlrpc_server_info *server;
    xmlrpc_value *result;
    const char *server_url = "http://localhost:8760";

    xmlrpc_value *current_nzb_arr;
    xmlrpc_value *current_nzb_p;
    xmlrpc_value *current_name_p;
    char *current_name;

    xmlrpc_value *is_paused_p;
    xmlrpc_bool is_paused;

    xmlrpc_value *rate_p;
    xmlrpc_int rate;

    xmlrpc_value *eta_p;
    xmlrpc_int eta;

    char row_name[512];
    char row_speed[512];
    char row_eta[512];

    /* Start up our XML-RPC client library. */
    xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION);
    xmlrpc_env_init(&env);

    /* Make a new object to represent our XML-RPC server. */
    server = xmlrpc_server_info_new(&env, server_url);
    die_if_fault_occurred(&env);

    /* Set up our authentication information. */
    /* TODO: Not hardcoding passwd */
    xmlrpc_server_info_set_basic_auth(&env, server, "hellanzb", "changeme");
    die_if_fault_occurred(&env);

    /* TODO: Handle the server becoming online/offline */
    while (!finished) {
        result = xmlrpc_client_call_server(&env, server, "status", "()");
        die_if_fault_occurred(&env);

        /* Check if something is downloading */
        xmlrpc_struct_find_value(&env, result, "currently_downloading", &current_nzb_arr);
        die_if_fault_occurred(&env);
        if (current_nzb_arr && xmlrpc_array_size(&env, current_nzb_arr) > 0) {
            /* Get current nzb name */
            xmlrpc_array_read_item(&env, current_nzb_arr, 0, &current_nzb_p);
            die_if_fault_occurred(&env);
            xmlrpc_struct_find_value(&env, current_nzb_p, "nzbName", &current_name_p);
            die_if_fault_occurred(&env);
            xmlrpc_read_string(&env, current_name_p, (const char**)&current_name);
            die_if_fault_occurred(&env);

            xmlrpc_struct_find_value(&env, result, "is_paused", &is_paused_p);
            xmlrpc_read_bool(&env, is_paused_p, &is_paused);

            sprintf(row_name, "%s%s", current_name, is_paused ? " (Paused)" : "");

            /* Get current speed */
            xmlrpc_struct_find_value(&env, result, "rate", &rate_p);
            xmlrpc_read_int(&env, rate_p, &rate);
            sprintf(row_speed, "%d kB/s", rate);

            /* Get current ETA */
            xmlrpc_struct_find_value(&env, result, "eta", &eta_p);
            xmlrpc_read_int(&env, eta_p, &eta);
            sprintf(row_eta, "%.2d:%.2d", eta/60, eta%60);
        } else {
            sprintf(row_name, "Nothing");
            row_speed[0] = '\0';
            row_eta[0] = '\0';
        }

        /* Printing on screen */
        g15r_clearScreen(canvas, G15_COLOR_WHITE);
        g15r_renderString(canvas, (unsigned char *) row_name, 0, textSize, 0, 0);
        g15r_renderString(canvas, (unsigned char *) row_speed, 1, textSize, 0, 0);
        g15r_renderString(canvas, (unsigned char *) row_eta, 2, textSize, 0, 0);
        g15_send(g15screen_fd, (char *) canvas->buffer, G15_BUFFER_LEN);

        usleep(2*1000*1000);
    }

    xmlrpc_DECREF(current_nzb_arr);
    xmlrpc_DECREF(current_nzb_p);
    xmlrpc_DECREF(current_name_p);
    xmlrpc_DECREF(is_paused_p);
    xmlrpc_DECREF(eta_p);
    xmlrpc_DECREF(result);

    xmlrpc_server_info_free(server);
    xmlrpc_env_clean(&env);
    xmlrpc_client_cleanup();
}

void reset_screen() {
    g15r_clearScreen(canvas, G15_COLOR_WHITE);
    g15r_renderString(canvas, (unsigned char *) "G15Hella Waiting", 0, G15_TEXT_LARGE, 0, 0);
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

    pid = is_app_running("g15hella", pid_file);

    if (kill_f) {
        if (pid > 0)
            kill(pid, SIGINT);
        return EXIT_SUCCESS;
    }

    if (pid != 0) {
        fprintf(stderr, "G15Hella already running\n");
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

    hella_loop();

    pthread_join(keys_thread, NULL);

    close(g15screen_fd);

    unlink(pid_file);
    return EXIT_SUCCESS;
}
