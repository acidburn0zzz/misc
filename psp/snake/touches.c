#include "touches.h"

#include <SDL/SDL.h>

#ifdef PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>

/**********
 * Retourne
 * La touche associee a l'event
 * -1 si il n'y a pas eu d'event
 * -2 si il y a eu un event dont on ne veut pas
 * -3 si il y a eu une touche non traitee
 **********/
int lireTouche() {
    static SceCtrlData pad, oldPad;
    sceCtrlPeekBufferPositive(&pad, 1);
    
    if (pad.Buttons & PSP_CTRL_UP) { return KEY_UP; }
    if (pad.Buttons & PSP_CTRL_DOWN) { return KEY_DOWN; }
    if (pad.Buttons & PSP_CTRL_LEFT) { return KEY_LEFT; }
    if (pad.Buttons & PSP_CTRL_RIGHT) { return KEY_RIGHT; }
    if (pad.Buttons & PSP_CTRL_START) { return KEY_QUIT; }
    if (pad.Buttons) { return -3; } /*Il y a eu une touche enfoncee, mais qui n'est pas connue*/
    
    return -1;
}

#else

/**********
 * Retourne
 * La touche associee a l'event
 * -1 si il n'y a pas eu d'event
 * -2 si il y a eu un event dont on ne veut pas
 * -3 si il y a eu une touche de clavier non traitee
 **********/
int lireTouche() {
    SDL_Event evt;
    
    if (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    return KEY_UP;
                case SDLK_DOWN:
                    return KEY_DOWN;
                case SDLK_LEFT:
                    return KEY_LEFT;
                case SDLK_RIGHT:
                    return KEY_RIGHT;
                case SDLK_ESCAPE:
                    return KEY_QUIT;
                default:
                    return -3;
            }
        } else if (evt.type == SDL_QUIT) { /*Petit tweak si on ferme la fenetre*/
            return KEY_QUIT;
        } else {
            return -2;
        }
    } else {
        return -1;
    }
}

#endif
