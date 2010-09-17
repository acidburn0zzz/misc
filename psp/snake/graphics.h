#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#ifdef PSP
    #include <pspkernel.h>
    #include <pspdebug.h>
    #include <pspctrl.h>
    #include <pspdisplay.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "struct.h"

#define SCREEN_W 480
#define SCREEN_H 272

int initialiserSurfaces(Surfaces *surfce);
int libererSurfaces(Surfaces surfce);
void afficherIntro(SDL_Surface *screen);
void afficherLevelUp(SDL_Surface *screen);
void afficherDead(SDL_Surface *screen);
void afficherSurface(SDL_Surface *surface, SDL_Surface *screen, int x, int y);
/*void afficherStatusBar(Partie p, Surfaces surfce);*/
int afficherPartie(Partie p, Surfaces *surfce, Snake s);

#endif /*_GRAPHICS_H_*/
