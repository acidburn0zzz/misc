#include <stdlib.h>
#include <malloc.h>

#include <SDL/SDL.h>

#include "dalek.h"
#include "doc.h"
#include "partie.h"
#include "graphics.h"

Partie nouvellePartie() {
    Partie p;
    p.niveau = 1;
    p.nbZap = 1;
    p.nbPts = 0;
    
    p.nbCasesX = 30;
    p.nbCasesY = 16;
    
    return p;
}

void monterNiveau(Partie *p) {
    p->niveau++;
    p->nbZap++;
}

void reset(Partie *p) {
    p->niveau = 1;
    p->nbPts = 0;
    p->nbZap = 1;
}

void initNiveau(Partie p, Doc *doc, Dalek **daleks) {
    int i;
    
    if (*daleks) { free(*daleks); }
    *daleks = malloc(sizeof(Dalek) * p.niveau * 5);
    if (*daleks == NULL) {
        printf("Can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    /*Creation de tous les daleks*/
    for (i=0; i<p.niveau*5; i++) {
        (*daleks)[i] = creerDalek(*daleks, p);
    }
    
    /*Creation du doc*/
    *doc = creerDoc(*daleks, p);
}

int zapper(Doc doc, Dalek *daleks, Partie *p) {
    int i;
    if (p->nbZap > 0) {
        for (i=0; i<p->niveau*5; i++) {
            if (abs(daleks[i].posX - doc.posX) <= 1 && abs(daleks[i].posY - doc.posY) <= 1) {
                if (!daleks[i].isDead) {
                    p->nbPts += 5;
                }
                daleks[i].isDead = 1;
                daleks[i].isZapped = 1;
            }
        }
        p->nbZap--;
        return 0;
    } else {
        return -1;
    }
}

int run(Doc *doc, Dalek *daleks, Partie *p, Surfaces surfce) {
    int i;
    
    while (1) {
        /*On verifie si les daleks peuvent bouger*/
        for (i=0; i<p->niveau*5; i++) {
            if (!daleks[i].isDead && abs(daleks[i].posX - doc->posX) <= 1 && abs(daleks[i].posY - doc->posY) <= 1) {
                return 0;
            }
        }
        
        moveDaleks(daleks, *doc, *p);
        checkMorts(doc, daleks, p);
        
        /*On arrete la boucle si le niveau n'est pas termine*/
        if (getStatutPartie(*doc, daleks, *p) != 0) {
            break;
        }
        
        /*Animation du run*/
        afficherPartie(*p, surfce, *doc, daleks);
        SDL_Delay(100);
    }
    
    return 0;
}
    

void checkMorts(Doc *doc, Dalek *daleks, Partie *p) {
    int i, j;
    
    /*Verification si le doc est mort*/
    for (i=0; i<p->niveau*5; i++) {
        if (doc->posX == daleks[i].posX && doc->posY == daleks[i].posY && !daleks[i].isZapped)
            doc->isDead = 1;
    }
    
    /*Verification des collisions entre daleks*/
    for (i=0; i<p->niveau*5-1; i++) {
        for (j=i+1; j<p->niveau*5; j++) {
            if (!daleks[i].isZapped && !daleks[j].isZapped && daleks[i].posX == daleks[j].posX 
                && daleks[i].posY == daleks[j].posY) {
                    if (!daleks[i].isDead) {
                        daleks[i].isDead = 1;
                        p->nbPts += 5;
                    }
                    if (!daleks[j].isDead) {
                        daleks[j].isDead = 1;
                        p->nbPts += 5;
                    }
            }
        }
    }
}


/**********
 * Valeurs de retour
 * 0 : On continue
 * 1 : Partie gagnee
 * 2 : Perdu
**********/
int getStatutPartie(Doc doc, Dalek *daleks, Partie p) {
    int i;
    
    if (doc.isDead) {
        return 2;
    }
    
    /*Si au moins un dalek est en vie, on continue/*/
    for (i=0; i<p.niveau*5; i++) {
        if (!daleks[i].isDead)
            return 0;
    }
    
    /*Sinon le niveau est gagne*/
    return 1;
}
