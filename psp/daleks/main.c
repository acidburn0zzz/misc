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
#include "dalek.h"
#include "doc.h"
#include "partie.h"
#include "struct.h"

#ifdef PSP
    PSP_MODULE_INFO("Daleks", 0x00, 0, 1);
    PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
    PSP_HEAP_SIZE_KB(2048);
    
    #define printf pspDebugScreenPrintf
#endif

int main() {
    int fin=0, touche, action;
    Partie partie;
    Dalek *daleks = NULL;
    Doc doc;
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
        SDL_WM_SetCaption("Daleks-SDL", NULL);
    #endif
    
    /*Affichage de l'intro, sert de loading Screen*/
    afficherIntro(surfce.screen);
    
    /*Creation de la partie*/
    partie = nouvellePartie();
    
    /*Initialisation des personnages*/
    initNiveau(partie, &doc, &daleks);
    
    /*Initialisation des surfaces SDL*/
    if (initialiserSurfaces(&surfce) != 0) {
        exit(EXIT_FAILURE);
    }
    
    /*Le loading est fini, on attend une touche de l'utilisateur*/
    while (lireTouche() == -1) {
    }
    
    /*Sert a annuler la touche enfoncee precedement*/
    afficherPartie(partie, surfce, doc, daleks);
    
    while (!fin) {
        /**********
            ETAPES
            1. Lire la touche
            2. Effectuer l'action
            3. Bouger les daleks
            4. Verifier les morts
            5. Verifier l'etat de la partie
            6. Afficher le jeu
          **********/
        action = 1; /*Pour savoir si il y a eu une action ce tour*/
        touche = lireTouche();
        switch (touche) {
            case KEY_UP:
                if (moveDoc(0, -1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_DOWN:
                if (moveDoc(0, 1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_LEFT:
                if (moveDoc(-1, 0, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_RIGHT:
                if (moveDoc(1, 0, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_UP_LEFT:
                if (moveDoc(-1, -1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_UP_RIGHT:
                if (moveDoc(1, -1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_DOWN_LEFT:
                if (moveDoc(-1, 1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_DOWN_RIGHT:
                if (moveDoc(1, 1, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_STAY:
                if (moveDoc(0, 0, &doc, daleks, partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_ZAP:
                if (zapper(doc, daleks, &partie) != 0) {
                    action = 0;
                }
                break;
            case KEY_TELEPORT:
                teleportDoc(&doc, daleks, partie);
                break;
            case KEY_RUN:
                run(&doc, daleks, &partie, surfce);
                action = 0; /*Les daleks ont deja bouge*/
                break;
            case KEY_QUIT:
                fin = 1;
            default: /*aucune action*/
                action = 0;
        }
        
        /*Les daleks bougent seulement si le doc a fait une action*/
        if (action) {
            moveDaleks(daleks, doc, partie);
            checkMorts(&doc, daleks, &partie);
        }
        
        if (getStatutPartie(doc, daleks, partie) == 1) {
            afficherPartie(partie, surfce, doc, daleks); /*Affichage de la partie avant d'afficher lvlUp*/
            afficherLevelUp(surfce.screen);                 /*Parce que l'affichage est long*/
            monterNiveau(&partie);
            initNiveau(partie, &doc, &daleks);
        } else if (getStatutPartie(doc, daleks, partie) == 2) {
            afficherPartie(partie, surfce, doc, daleks); /*Affichage de la partie avant d'afficher la mort*/
            afficherDead(surfce.screen);                    /*Parce que l'affichage est long*/
            reset(&partie);
            initNiveau(partie, &doc, &daleks);
        }
        
        afficherPartie(partie, surfce, doc, daleks);
    }
    
    libererSurfaces(surfce);
    TTF_Quit();
    SDL_Quit();
    
    #ifdef PSP
        sceKernelExitGame();
    #endif
    return 0;
}
