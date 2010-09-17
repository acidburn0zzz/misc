#include "touches.h"

#include <SDL/SDL.h>

#ifdef PSP

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>

int lireTouche() {
    static SceCtrlData pad, oldPad;
    sceCtrlReadBufferPositive(&pad, 1);
    
    if (pad.Buttons != oldPad.Buttons) { /*Pour empecher le deplacement trop rapide*/
        oldPad = pad;
        /* Les touches double sont en premier sinon la valeur de retour
            etait celle des touches simples*/
        if (pad.Buttons & PSP_CTRL_UP && pad.Buttons & PSP_CTRL_LTRIGGER) { return KEY_UP_LEFT; }
        if (pad.Buttons & PSP_CTRL_UP && pad.Buttons & PSP_CTRL_RTRIGGER) { return KEY_UP_RIGHT; }
        if (pad.Buttons & PSP_CTRL_DOWN && pad.Buttons & PSP_CTRL_LTRIGGER) { return KEY_DOWN_LEFT; }
        if (pad.Buttons & PSP_CTRL_DOWN && pad.Buttons & PSP_CTRL_RTRIGGER) { return KEY_DOWN_RIGHT; }
        if (pad.Buttons & PSP_CTRL_UP) { return KEY_UP; }
        if (pad.Buttons & PSP_CTRL_DOWN) { return KEY_DOWN; }
        if (pad.Buttons & PSP_CTRL_LEFT) { return KEY_LEFT; }
        if (pad.Buttons & PSP_CTRL_RIGHT) { return KEY_RIGHT; }
        if (pad.Buttons & PSP_CTRL_SQUARE) { return KEY_STAY; }
        if (pad.Buttons & PSP_CTRL_CROSS) { return KEY_ZAP; }
        if (pad.Buttons & PSP_CTRL_CIRCLE) { return KEY_TELEPORT; }
        if (pad.Buttons & PSP_CTRL_TRIANGLE) { return KEY_RUN; }
        if (pad.Buttons & PSP_CTRL_START) { return KEY_QUIT; }
        if (pad.Buttons) { return -2; } /*Il y a eu une touche enfoncee, mais qui n'est pas connue*/
    }
    
    return -1;
}

#else

int lireTouche() {
    SDL_Event evt;
    SDL_WaitEvent(&evt); /*WaitEvent est utilise car il ne se passe rien entre 2 inputs*/
                                       /*Remplacer par PollEvent sinon*/
    
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
            case SDLK_SPACE:
                return KEY_STAY;
            case SDLK_z:
                return KEY_ZAP;
            case SDLK_t:
                return KEY_TELEPORT;
            case SDLK_r:
                return KEY_RUN;
            case SDLK_ESCAPE:
                return KEY_QUIT;
            default:
                return -2; /*Il y a eu une touche enfoncee, mais qui n'est pas connue*/
        }
    } else if (evt.type == SDL_QUIT) { /*Petit tweak si on ferme la fenetre*/
        return KEY_QUIT; /*WTF aller sur la ligne #12 envoie un signal SDL_QUIT !!?!?!*/
    }
    
    return -1;
}

#endif
