#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

typedef struct Doc {
    int posX;
    int posY;
    int isDead;
} Doc;

typedef struct Dalek {
    int posX;
    int posY;
    int isDead;
    int isZapped;
} Dalek;

typedef struct Partie {
    int niveau;
    int nbZap;
    int nbPts;
    int nbCasesX;
    int nbCasesY;
} Partie;

typedef struct Surfaces {
    SDL_Surface *screen;
    SDL_Surface *back;
    SDL_Surface *text;
    SDL_Surface *imgDoc;
    SDL_Surface *imgDocDead;
    SDL_Surface *imgDalek;
    SDL_Surface *imgDalekDead;
    TTF_Font *font;
    SDL_Color txtColor;
    SDL_Color txtBackColor;
    SDL_Rect txtPos;
} Surfaces;

#endif /*_STRUCT_H_*/
