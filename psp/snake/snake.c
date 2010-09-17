#include <stdio.h>
#include <stdlib.h>

#include "snake.h"
#include "struct.h"

void resetSnake(Snake *s, Partie p) {
    int i;
    
    /*Caracteristiques de base du serpent*/
    s->length = 5;
    s->maxLength = 15;
    s->speed = 150 + (p.niveau-1) * 5;
    s->direction = DROITE;
    
    if (s->posX) { free(s->posX); }
    s->posX = malloc(sizeof(int) * s->maxLength);
    if (s->posX == NULL) {
        printf("Can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    if (s->posY) { free(s->posY); }
    s->posY = malloc(sizeof(int) * s->maxLength);
    if (s->posY == NULL) {
        printf("Can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    for (i=0; i<s->length; i++) {
        s->posX[i] = ((p.nbCasesX - s->length) / 2 + (4-i));
        s->posY[i] = p.nbCasesY / 2;
    }
}

void moveSnake(Snake *s, Partie p) {
    int i;
    
    for (i=s->length-1; i>0; i--) {
        s->posX[i] = s->posX[i-1];
        s->posY[i] = s->posY[i-1];
    }
    
    switch(s->direction) {
        case HAUT:
            s->posY[0]--;
            break;
        case BAS:
            s->posY[0]++;
            break;
        case GAUCHE:
            s->posX[0]--;
            break;
        case DROITE:
            s->posX[0]++;
            break;
    }
}

void allongerSnake(Snake *s) {
    s->length++;
    s->posX[s->length-1] = s->posX[s->length-2];
    s->posY[s->length-1] = s->posY[s->length-2];
}

void changeDirectionSnake(Snake *s, int direction) {
    switch(direction) {
        case HAUT:
            if (s->direction == GAUCHE || s->direction == DROITE)
                s->direction = HAUT;
            break;
        case BAS:
            if (s->direction == GAUCHE || s->direction == DROITE)
                s->direction = BAS;
            break;
        case GAUCHE:
            if (s->direction == HAUT || s->direction == BAS)
                s->direction = GAUCHE;
            break;
        case DROITE:
            if (s->direction == HAUT || s->direction == BAS)
                s->direction = DROITE;
            break;
    }
}

/*Retourne 1 si collision, 0 sinon*/
int checkCollisionSnake(Snake s, Partie p) {
    int i;
    
    /*Collision avec lui meme*/
    for (i=1; i<s.length; i++) {
        if (s.posX[0] == s.posX[i] && s.posY[0] == s.posY[i]) {
            return 1;
        }
    }
    
    /*Collision avec le "mur"*/
    if (s.posX[0] == -1 || s.posX[0] == p.nbCasesX || s.posY[0] == -1 || s.posY[0] == p.nbCasesY) {
        return 1;
    }
    
    return 0;
}

/*Retourne 1 si cible touchee, 0 sinon*/
int checkCibleSnake(Snake s, Partie p) {
    if (s.posX[0] == p.ciblePosX && s.posY[0] == p.ciblePosY)
        return 1;
    else
        return 0;
}
