#include <stdlib.h>
#include <malloc.h>

#include <SDL/SDL.h>

#include "snake.h"
#include "partie.h"
#include "graphics.h"

Partie nouvellePartie() {
    Partie p;
    p.niveau = 1;
    p.nbPts = 0;
    
    p.nbCasesX = 40;
    p.nbCasesY = 20;
    
    p.ciblePosX = 0;
    p.ciblePosY = 0;
    
    return p;
}

void monterNiveau(Partie *p) {
    p->niveau++;
}

void reset(Partie *p) {
    p->niveau = 1;
    p->nbPts = 0;
}

void initNiveau(Partie *p, Snake *s) {
    /*Reinitialisation du serpent*/
   resetSnake(s, *p);
    
    /*Creation de la cible initiale*/
    creerCible(*s, p);
}

void creerCible(Snake s, Partie *p) {
    int i, ok=0;
    
    p->ciblePosX = rand()%p->nbCasesX;
    p->ciblePosY = rand()%p->nbCasesY;
    
    while (!ok) {
        ok = 1;
        
        for (i=0; i<s.length; i++) {
            if (p->ciblePosX == s.posX[i] && p->ciblePosY == s.posY[i]) {
                ok = 0;
                i = s.length;
                p->ciblePosX = rand()%p->nbCasesX;
                p->ciblePosY = rand()%p->nbCasesY;
            }
        }
    }
}
