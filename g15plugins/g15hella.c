#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>

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

#define HELLANZB_DEFAULT_HOST "localhost"
#define HELLANZB_DEFAULT_PORT "8760"
#define HELLANZB_DEFAULT_PASSWD "changeme"

int finished = 0;
int g15screen_fd;
g15canvas *canvas;
char pid_file[] = "/tmp/g15hella.pid";

unsigned int textSize = G15_TEXT_HUGE;

char row_name[512];
char row_speed[512];
char row_eta[512];

char *host = NULL, *port = NULL, *passwd = NULL;

void status_loop();
void reset_screen();
void update_screen();

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
                update_screen();
            }
        } else if(keystate & G15_KEY_L5) {
            if (textSize < G15_TEXT_HUGE) {
                textSize++;
                update_screen();
            }
        }

        usleep(100000);
    }

    return;
}

static void die_if_fault_occurred(xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "XML-RPC Fault: %s (%d)\nExiting.\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

char *get_current_name(xmlrpc_env *env, xmlrpc_value *status) {
    xmlrpc_value *current_nzb_arr;
    xmlrpc_value *current_nzb_p;
    xmlrpc_value *current_name_p;
    char *current_name;

    /* Check if something is downloading */
    xmlrpc_struct_find_value(env, status, "currently_downloading", &current_nzb_arr);
    die_if_fault_occurred(env);
    if (current_nzb_arr && xmlrpc_array_size(env, current_nzb_arr) > 0) {
        /* Get current nzb name */
        xmlrpc_array_read_item(env, current_nzb_arr, 0, &current_nzb_p);
        die_if_fault_occurred(env);
        xmlrpc_struct_find_value(env, current_nzb_p, "nzbName", &current_name_p);
        die_if_fault_occurred(env);
        xmlrpc_read_string(env, current_name_p, (const char**)&current_name);
        die_if_fault_occurred(env);

        xmlrpc_DECREF(current_nzb_arr);
        xmlrpc_DECREF(current_nzb_p);
        xmlrpc_DECREF(current_name_p);

        return current_name;
    } else {
        xmlrpc_DECREF(current_nzb_arr);
        return NULL;
    }
}

xmlrpc_bool get_is_paused(xmlrpc_env *env, xmlrpc_value *status) {
    xmlrpc_value *is_paused_p;
    xmlrpc_bool is_paused;

    xmlrpc_struct_find_value(env, status, "is_paused", &is_paused_p);
    xmlrpc_read_bool(env, is_paused_p, &is_paused);

    xmlrpc_DECREF(is_paused_p);

    return is_paused;
}

xmlrpc_int get_rate(xmlrpc_env *env, xmlrpc_value *status) {
    xmlrpc_value *rate_p;
    xmlrpc_int rate;

    xmlrpc_struct_find_value(env, status, "rate", &rate_p);
    xmlrpc_read_int(env, rate_p, &rate);

    xmlrpc_DECREF(rate_p);

    return rate;
}

xmlrpc_int get_eta(xmlrpc_env *env, xmlrpc_value *status) {
    xmlrpc_value *eta_p;
    xmlrpc_int eta;

    xmlrpc_struct_find_value(env, status, "eta", &eta_p);
    xmlrpc_read_int(env, eta_p, &eta);

    xmlrpc_DECREF(eta_p);

    return eta;
}

xmlrpc_server_info *connect_to_hellanzb(xmlrpc_env *env, const char *server_url, const char *server_passwd) {
    xmlrpc_server_info *server;

    /* Make a new object to represent our XML-RPC server. */
    server = xmlrpc_server_info_new(env, server_url);
    die_if_fault_occurred(env);

    /* Set up our authentication information. */
    xmlrpc_server_info_set_basic_auth(env, server, "hellanzb", server_passwd);
    die_if_fault_occurred(env);

    return server;
}

void status_loop() {
    xmlrpc_env *env;
    xmlrpc_server_info *server;
    xmlrpc_value *status = NULL;
    char server_url[1024];
    char server_passwd[1024];

    char *current_name;
    xmlrpc_bool is_paused;
    xmlrpc_int rate;
    xmlrpc_int eta;

    snprintf(server_url, 1024, "http://%s:%s", host ? host : HELLANZB_DEFAULT_HOST, port ? port : HELLANZB_DEFAULT_PORT);
    snprintf(server_passwd, 1024, "%s", passwd ? passwd : HELLANZB_DEFAULT_PASSWD);

    /* Start up our XML-RPC client library. */
    env = malloc(sizeof(xmlrpc_env));
    xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION);
    xmlrpc_env_init(env);

    server = connect_to_hellanzb(env, server_url, server_passwd);

    /* "Ping" the server once to check if everything OK */
    status = xmlrpc_client_call_server(env, server, "status", "()");
    die_if_fault_occurred(env);

    while (!finished) {
        /* Clear traces of previous error */
        env->fault_occurred = 0;

        if (server == NULL)
            server = connect_to_hellanzb(env, server_url, server_passwd);

        if (server != NULL) {
            status = xmlrpc_client_call_server(env, server, "status", "()");

            /* Check if connection is still alive */
            if (env->fault_occurred && env->fault_code == -504) {
                xmlrpc_server_info_free(server);
                server = NULL;
            } else {
                /* Check for other errors and continue as usual */
                die_if_fault_occurred(env);

                /* Check if something is downloading */
                current_name = get_current_name(env, status);
                if (current_name) {
                    is_paused = get_is_paused(env, status);
                    snprintf(row_name, 512, "%s%s", current_name, is_paused ? " (Paused)" : "");

                    /* Get current speed */
                    rate = get_rate(env, status);
                    sprintf(row_speed, "Speed: %d kB/s", rate);

                    /* Get current ETA */
                    eta = get_eta(env, status);
                    sprintf(row_eta, "ETA: %.2d:%.2d:%.2d", eta/3600, eta%3600/60, eta%60);
                } else {
                    sprintf(row_name, "Hellanzb");
                    sprintf(row_speed, "Not downloading");
                    row_eta[0] = '\0';
                }
            }
        }

        if (server == NULL) {
            sprintf(row_name, "Hellanzb Error");
            sprintf(row_speed, "Unable to connect");
            row_eta[0] = '\0';
        }

        /* Printing on screen */
        update_screen();

        usleep(2000000);
    }

    xmlrpc_DECREF(status);

    xmlrpc_server_info_free(server);
    xmlrpc_env_clean(env);
    free(env);
    xmlrpc_client_cleanup();
}

void update_screen() {
    g15r_clearScreen(canvas, G15_COLOR_WHITE);
    g15r_renderString(canvas, (unsigned char *) row_name, 0, textSize, 0, 0);
    g15r_renderString(canvas, (unsigned char *) row_speed, 1, textSize, 0, 0);
    g15r_renderString(canvas, (unsigned char *) row_eta, 2, textSize, 0, 0);
    g15_send(g15screen_fd, (char *) canvas->buffer, G15_BUFFER_LEN);
}

void reset_screen() {
    g15r_clearScreen(canvas, G15_COLOR_WHITE);
    g15r_renderString(canvas, (unsigned char *) "G15Hella Waiting", 0, G15_TEXT_LARGE, 0, 0);
    g15_send(g15screen_fd, (char *) canvas->buffer, G15_BUFFER_LEN);
}

inline long get_elapsed_ms(struct timeval before, struct timeval after) {
    return ((after.tv_sec - before.tv_sec) * 1000 + (after.tv_usec - before.tv_usec) / 1000);
}

void show_usage(char *prog) {
    printf("Usage: %s [OPTIONS]\n\n", prog);
}

int main(int argc, char **argv) {
    struct sigaction act;
    pid_t pid;
    int daemonize_f = 0, kill_f = 0, restart_f = 0;
    char opt;

    pthread_t keys_thread;

    struct timeval t1, t2;

    while ((opt = getopt(argc, argv, "dkh:p:P:r")) != -1) {
        switch (opt) {
        case 'd':
            daemonize_f = 1;
            break;
        case 'k':
            kill_f = 1;
            break;
        case 'r':
            restart_f = 1;
            break;
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'P':
            passwd = optarg;
            break;
        }
    }

    if (kill_f && restart_f) {
        fprintf(stderr, "You can't use -k and -r at the same time\n");
        return EXIT_FAILURE;
    }

    pid = is_app_running("g15hella", pid_file);

    if (kill_f || restart_f) {
        if (pid <= 0) {
            fprintf(stderr, "Cannot kill G15Hella: Not running.\nExiting.\n");
            return EXIT_FAILURE;
        }

        kill(pid, SIGINT);

        if (kill_f)
            return EXIT_SUCCESS;

        /* We're restarting, wait until previous instance is actually killed
         * Quit if it takes more than 2 seconds
         */
        gettimeofday(&t1, 0);
        while (is_app_running("g15hella", pid_file) != 0) {
            gettimeofday(&t2, 0);
            if (get_elapsed_ms(t1, t2) > 2000) {
                fprintf(stderr, "Unable to kill G15Hella.\nExiting.\n");
                return EXIT_FAILURE;
            }

            usleep(10 * 1000);
        }
    } else if (pid != 0) {
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
            perror("stdout");
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

    status_loop();

    pthread_join(keys_thread, NULL);

    close(g15screen_fd);

    unlink(pid_file);
    return EXIT_SUCCESS;
}
