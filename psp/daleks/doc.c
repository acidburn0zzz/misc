#include <stdlib.h>

#include "dalek.h"
#include "doc.h"
#include "struct.h"

Doc creerDoc(Dalek *daleks, Partie p) {
    Doc d;
    int i;
    
    d.posX = rand() % p.nbCasesX;
    d.posY = rand() % p.nbCasesY;
    d.isDead = 0;
    
    for (i=0; i<p.niveau*5; i++) {
        if (d.posX == daleks[i].posX && d.posY == daleks[i].posY) {
            return creerDoc(daleks, p);
        }
    }
    
    return d;
}

int moveDoc(int x, int y, Doc *doc, Dalek *daleks, Partie p) {
    int _x, _y, i;
    
    _x = doc->posX + x;
    _y = doc->posY + y;
    
    /*Verifier si il y a un tas de feraille*/
    for (i=0; i<p.niveau*5; i++) {
        if (daleks[i].isDead && !daleks[i].isZapped
            && _x == daleks[i].posX && _y == daleks[i].posY) {
                return -1;
        }
    }
    
    /*Verifier si on est au bout du tableau*/
    if (_x < 0 || _x == p.nbCasesX || _y < 0 || _y == p.nbCasesY) {
        return -1;
    }
    
    doc->posX = _x;
    doc->posY = _y;
    
    return 0;
}


int teleportDoc(Doc *doc, Dalek *daleks, Partie p) {
    int i;
    
    doc->posX = rand() % p.nbCasesX;
    doc->posY = rand() % p.nbCasesY;
    
    /*Ne pas se teleporter sur un tas de feraille*/
    for (i=0; i<p.niveau*5; i++) {
        if (daleks[i].isDead && !daleks[i].isZapped && doc->posX == daleks[i].posX && doc->posY == daleks[i].posY) {
            return teleportDoc(doc, daleks, p);
        }
    }
    
    return 0;
}
