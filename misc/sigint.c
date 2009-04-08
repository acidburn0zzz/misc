#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void catcher(int sig) {
    switch (sig) {
        case SIGINT:
            printf("\rCtrl+C: Exiting\n");
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, catcher);
    
    while(1){}
    
	return 0;
}
