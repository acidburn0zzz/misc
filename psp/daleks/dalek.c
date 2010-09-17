#include <stdlib.h>

#include "dalek.h"
#include "doc.h"
#include "partie.h"

Dalek creerDalek(Dalek *daleks, Partie p) {
    Dalek d;
    int i;
    
    d.posX = rand() % p.nbCasesX;
    d.posY = rand() % p.nbCasesY;
    d.isDead = 0;
    d.isZapped = 0;

    for (i=0; i<p.niveau*5; i++) {
        if (d.posX == daleks[i].posX && d.posY == daleks[i].posY) {
            return creerDalek(daleks, p);
        }
    }
    
    return d;
}

void moveDaleks(Dalek *daleks, Doc doc, Partie p) {
    int i;
    
    for (i=0; i<p.niveau*5; i++) {
        if (daleks[i].isDead)
            continue;
        if (daleks[i].posX > doc.posX)
            daleks[i].posX--;
        if (daleks[i].posX < doc.posX)
            daleks[i].posX++;
        if (daleks[i].posY > doc.posY)
            daleks[i].posY--;
        if (daleks[i].posY < doc.posY)
            daleks[i].posY++;
    }
}
