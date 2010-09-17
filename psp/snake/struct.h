#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <SDL/SDL.h>

typedef struct Snake {
    int length;
    int maxLength;
    int *posX;
    int *posY;
    int speed; /*nbCases / minute*/
    int direction;
} Snake;

typedef struct Partie {
    int niveau;
    int nbPts;
    int nbCasesX;
    int nbCasesY;
    int ciblePosX;
    int ciblePosY;
} Partie;

typedef struct Surfaces {
    SDL_Surface *screen;
    SDL_Surface *snake;
    SDL_Surface *snakeHead;
    SDL_Surface *back;
    SDL_Surface *cible;
} Surfaces;

#endif /*_STRUCT_H_*/
