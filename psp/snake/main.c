#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef PSP
    #include <pspkernel.h>
    #include <pspdebug.h>
    #include <pspdisplay.h>
    #include <pspctrl.h>
    #include "callbacks.h"
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "touches.h"
#include "graphics.h"
#include "snake.h"
#include "partie.h"
#include "struct.h"

#ifdef PSP
    PSP_MODULE_INFO("Snake", 0x00, 0, 1);
    PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
    PSP_HEAP_SIZE_KB(1024);
    
    #define printf pspDebugScreenPrintf
#endif

int main() {
    int fin=0, touche;
    Partie partie;
    Snake snake;
    Surfaces surfce;
    
    /*Initialisations*/
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    srand(time(NULL));
    #ifdef PSP
    pspDebugScreenInit();
    SetupCallbacks();
    #endif
    
    /*Initialisation de l'ecran principal*/
    surfce.screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    #ifndef PSP /*Titre si pas sur le PSP*/
        SDL_WM_SetCaption("Snakes-SDL", NULL);
    #endif
    
    /*Affichage de l'intro, sert de loading Screen*/
    afficherIntro(surfce.screen);
    
    /*Creation de la partie*/
    partie = nouvellePartie();
    
    /*S'assurer que snake.pos == NULL*/
    snake.posX = NULL;
    snake.posY = NULL;
    
    /*Initialisation des personnages*/
    initNiveau(&partie, &snake);
    
    /*Initialisation des surfaces SDL*/
    if (initialiserSurfaces(&surfce) != 0) {
        exit(EXIT_FAILURE);        
    }
    
    /*Le loading est fini, on attend une touche de l'utilisateur*/
    touche = lireTouche();
    while (touche == -1 || touche == -2) {
        touche = lireTouche();
    }
    
    /*Sert a annuler la touche enfoncee precedement*/
    afficherPartie(partie, &surfce, snake);
    
    while (!fin) {
        SDL_Delay(60000/snake.speed);
        touche = lireTouche();
        
        switch (touche) {
            case KEY_UP:
                changeDirectionSnake(&snake, HAUT);
                break;
            case KEY_DOWN:
                changeDirectionSnake(&snake, BAS);
                break;
            case KEY_LEFT:
                changeDirectionSnake(&snake, GAUCHE);
                break;
            case KEY_RIGHT:
                changeDirectionSnake(&snake, DROITE);
                break;
            case KEY_QUIT:
                fin = 1;
        }
        
        /*Tweak pour "oublier" les touches et events inconnues*/
        while (lireTouche() == -2 || lireTouche() == -3) {
        }
        
        moveSnake(&snake, partie);
        
        if (checkCibleSnake(snake, partie)) {
            allongerSnake(&snake);
            creerCible(snake, &partie);
            if (snake.length > snake.maxLength) {
                afficherLevelUp(surfce.screen);
                monterNiveau(&partie);
                initNiveau(&partie, &snake);
            }
        }
        
        if (checkCollisionSnake(snake, partie)) {
            afficherDead(surfce.screen);
            partie = nouvellePartie();
            initNiveau(&partie, &snake);
        }

        afficherPartie(partie, &surfce, snake);
    }
    
    free(snake.posX);
    free(snake.posY);
    
    libererSurfaces(surfce);
    TTF_Quit();
    SDL_Quit();
    
    #ifdef PSP
        sceKernelExitGame();
    #endif
    return 0;
}
