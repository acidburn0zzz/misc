#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "graphics.h"
#include "struct.h"

int initialiserSurfaces(Surfaces *surfce) {
    surfce->back = IMG_Load("images/back.png"); 
    if (!surfce->back) {
        return -1;
    }
    surfce->imgDoc = IMG_Load("images/dr.png");
    if (!surfce->imgDoc) {
        return -1;
    }
    surfce->imgDocDead = IMG_Load("images/dr_dead.png");
    if (!surfce->imgDocDead) {
        return -1;
    }
    surfce->imgDalek = IMG_Load("images/dalek.png");
    if (!surfce->imgDalek) {
        return -1;
    }
    surfce->imgDalekDead = IMG_Load("images/dalek_dead.png");
    if (!surfce->imgDalekDead) {
        return -1;
    }
    
    /*Le text sera initialise plus tard*/
    surfce->text = NULL;
    surfce->font = TTF_OpenFont("fonts/vera.ttf", 11);
    surfce->txtColor.r = 255;
    surfce->txtColor.g = 255;
    surfce->txtColor.b= 255;
    surfce->txtBackColor.r = 0;
    surfce->txtBackColor.g = 0;
    surfce->txtBackColor.b= 0;
    surfce->txtPos.x = 2;
    surfce->txtPos.y = 258;
    
    return 0;
}

int libererSurfaces(Surfaces surfce) {
    SDL_FreeSurface(surfce.screen);
    SDL_FreeSurface(surfce.back);
    SDL_FreeSurface(surfce.imgDoc);
    SDL_FreeSurface(surfce.imgDocDead);
    SDL_FreeSurface(surfce.imgDalek);
    SDL_FreeSurface(surfce.imgDalekDead);
    TTF_CloseFont(surfce.font);
    
    return 0;
}

void afficherIntro(SDL_Surface *screen) {
    SDL_Surface *text;
    TTF_Font *font;
    SDL_Rect pos;
    SDL_Color bl = {0, 0, 0};
    SDL_Color wh = {255, 255, 255};
    char *texte;
    
    /*Affichage de l'ecran d'intro*/
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    
    font = TTF_OpenFont("fonts/vera_it.ttf", 40);
    if (font == NULL) {
        printf("Can't open fonts/vera.ttf\n");
        exit(EXIT_FAILURE);
    }
    
    texte = "DALEKS";
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);
    
    pos.x = 60;
    pos.y = 20;
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    texte = "ON A PSP!!!";
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);
    
    pos.x = 130;
    pos.y = 80;
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    /*Affichage du texte explicatif*/
    font = TTF_OpenFont("fonts/vera_it.ttf", 16);
    if (font == NULL) {
        printf("Can't open fonts/vera.ttf\n");
        exit(EXIT_FAILURE);
    }
    #ifdef PSP
        texte = "Veuillez appuyer sur un bouton pour continuer";
    #else
        texte = "Veuillez appuyer sur une touche pour continuer";
    #endif
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);
    
    pos.x = (SCREEN_W - text->w) / 2;
    pos.y = 200;
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    SDL_Flip(screen);

    SDL_FreeSurface(text);
    TTF_CloseFont(font);
}

void afficherLevelUp(SDL_Surface *screen) {
    SDL_Surface *lvlUp, *text, *back;
    TTF_Font *font;
    SDL_Rect pos;
    SDL_Color bl = {0, 0, 0};
    SDL_Color wh = {255, 255, 255};
    char *texte;
    
    /*Mettre le fond blanc*/
    back = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0, 0, 0, 0);
    SDL_FillRect(back, NULL, SDL_MapRGB(back->format, 255, 255, 255));
    SDL_BlitSurface(back, NULL, screen, NULL);
    
    lvlUp = IMG_Load("images/level_up.png");
    if (lvlUp == NULL) {
        printf("Can't open images/level_up.png");
    }
    
    /*Centrer l'image*/
    pos.x = (SCREEN_W - lvlUp->w) / 2;
    pos.y = (SCREEN_H - lvlUp->h) / 4; /*Plus haut que la moitie*/
    
    SDL_BlitSurface(lvlUp, NULL, screen, &pos);
    
    font = TTF_OpenFont("fonts/vera_it.ttf", 24);
    if (font == NULL) {
        printf("Can't open fonts/vera_it.ttf\n");
        exit(EXIT_FAILURE);
    }
    
    texte = "Level Up!";
    
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);

    /*Centrer le texte*/
    pos.x = (SCREEN_W - text->w) / 2;
    pos.y = 220;
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    SDL_Flip(screen);
    
    SDL_FreeSurface(lvlUp);
    SDL_FreeSurface(text);
    SDL_FreeSurface(back);
    TTF_CloseFont(font);
    SDL_Delay(2500);/*Attendre 2.5 sec*/
}

void afficherDead(SDL_Surface *screen) {
    SDL_Surface *back, *text;
    TTF_Font *font;
    SDL_Rect pos;
    SDL_Color col = {220, 220, 220};
    char *texte;
    
    /*Image de fond*/
    back = IMG_Load("images/dead.gif");
    if (back == NULL) {
        printf("Can't open images/dead.gif\n");
        exit(EXIT_FAILURE);
    }
    
    SDL_BlitSurface(back, NULL, screen, NULL);
    
    /*Texte*/
    font = TTF_OpenFont("fonts/vera_it.ttf", 24);
    if (font == NULL) {
        printf("Can't open fonts/vera_it.ttf\n");
        exit(EXIT_FAILURE);
    }
    
    texte = "Game Over!";
    
    text = TTF_RenderUTF8_Blended(font, texte, col);
    pos.x = (SCREEN_W - text->w) / 2;
    pos.y = 125;
    
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    SDL_Flip(screen);
    
    SDL_FreeSurface(back);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    SDL_Delay(2500);
}

void afficherImage(SDL_Surface *image, SDL_Surface *screen, int x, int y) {
    SDL_Rect pos;
    
    pos.x = 16 * x;
    pos.y = 16 * y;
    
    SDL_BlitSurface(image, NULL, screen, &pos);
}

void afficherStatusBar(Partie p, Surfaces surfce) {
    char *texte;
    
    texte = malloc(100);
    if (texte == NULL) {
        printf("Can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(texte, "Level: %d    Points: %d    Zappers: %d", p.niveau, p.nbPts, p.nbZap);
    
    surfce.text = TTF_RenderUTF8_Shaded(surfce.font, texte, surfce.txtColor, surfce.txtBackColor);
    SDL_BlitSurface(surfce.text, NULL, surfce.screen, &surfce.txtPos);
    SDL_FreeSurface(surfce.text);
    
    free(texte);
}

int afficherPartie(Partie p, Surfaces surfce, Doc doc, Dalek *daleks) {
    int i;
    
    afficherImage(surfce.back, surfce.screen, 0, 0);
    
    for (i=0; i<p.niveau*5; i++) {
        if (!daleks[i].isDead) {
            afficherImage(surfce.imgDalek, surfce.screen, daleks[i].posX, daleks[i].posY);
        } else if (daleks[i].isDead && !daleks[i].isZapped) {
            afficherImage(surfce.imgDalekDead, surfce.screen, daleks[i].posX, daleks[i].posY);
        }
    }
    
    if (!doc.isDead) {
        afficherImage(surfce.imgDoc, surfce.screen, doc.posX, doc.posY);
    } else {
        afficherImage(surfce.imgDocDead, surfce.screen, doc.posX, doc.posY);
    }
    
    afficherStatusBar(p, surfce);
    
    SDL_Flip(surfce.screen);
    
    return 0;
}
