#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "graphics.h"
#include "struct.h"

int initialiserSurfaces(Surfaces *surfce) {
    surfce->snake = IMG_Load("images/snake_body.png");
    surfce->snakeHead = IMG_Load("images/snake_head.png");
    surfce->back =  SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, 240, 32, 0, 0, 0, 0);
    surfce->cible = SDL_CreateRGBSurface(SDL_HWSURFACE, 12, 12, 32, 0, 0, 0, 0);
    
    if (!surfce->snake || !surfce->snakeHead || !surfce->back || !surfce->cible) {
        printf("Can't allocate memory");
        return -1;
    }
    
    SDL_FillRect(surfce->back, NULL, SDL_MapRGB(surfce->back->format, 0, 0, 0));
    SDL_FillRect(surfce->cible, NULL, SDL_MapRGB(surfce->cible->format, 0, 0, 192));
    return 0;
}

int libererSurfaces(Surfaces surfce) {
    SDL_FreeSurface(surfce.snake);
    SDL_FreeSurface(surfce.snakeHead);
    SDL_FreeSurface(surfce.back);
    SDL_FreeSurface(surfce.cible);
    
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
    
    texte = "SNAKES";
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
    SDL_Surface *back, *text;
    TTF_Font *font;
    SDL_Rect pos;
    SDL_Color bl = {0, 0, 0};
    SDL_Color wh = {255, 255, 255};
    char *texte;
    
    /*Couleur de fond*/
    back =  SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0, 0, 0, 0);
    SDL_FillRect(back, NULL, SDL_MapRGB(back->format, 255, 255, 255));
    SDL_BlitSurface(back, NULL, screen, NULL);
    
    /*Texte*/
    font = TTF_OpenFont("fonts/vera_it.ttf", 24); /*SIGABRT, fuck!*/
    if (font == NULL) {
        printf("Can't open fonts/vera_it.ttf\n");
        exit(EXIT_FAILURE);
    }
    
    texte = "Level Up!";
    
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);

    /*Centrer le texte*/
    pos.x = (SCREEN_W - text->w) / 2;
    pos.y = (SCREEN_H - text->h) / 2;
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    SDL_Flip(screen);
    
    SDL_FreeSurface(back);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    SDL_Delay(2500);/*Attendre 2.5 sec*/
}

void afficherDead(SDL_Surface *screen) {
    SDL_Surface *back, *text;
    TTF_Font *font;
    SDL_Rect pos;
    SDL_Color bl = {0, 0, 0};
    SDL_Color wh = {255, 255, 255};
    char *texte;
    
    /*Couleur de fond*/
    back =  SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0, 0, 0, 0);
    SDL_FillRect(back, NULL, SDL_MapRGB(back->format, 255, 255, 255));
    SDL_BlitSurface(back, NULL, screen, NULL);
    
    /*Texte*/
    font = TTF_OpenFont("fonts/vera_it.ttf", 24);
    if (font == NULL) {
        printf("Can't open fonts/vera_it.ttf\n");
        exit(EXIT_FAILURE);
    }
    
    texte = "Game Over!";
    
    text = TTF_RenderUTF8_Shaded(font, texte, bl, wh);
    pos.x = (SCREEN_W - text->w) / 2;
    pos.y = (SCREEN_H - text->h) / 2;
    
    SDL_BlitSurface(text, NULL, screen, &pos);
    
    SDL_Flip(screen);
    
    SDL_FreeSurface(back);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    SDL_Delay(2500);
}

void afficherSurface(SDL_Surface *surface, SDL_Surface *screen, int x, int y) {
    SDL_Rect pos;
    
    pos.x = 12 * x;
    pos.y = 12 * y;
    
    SDL_BlitSurface(surface, NULL, screen, &pos);
}

void afficherStatusBar(Partie p, Surfaces surfce) {
    char *texte;
    
    texte = malloc(100);
    if (texte == NULL) {
        printf("Can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(texte, "Level: %d    Points: %d", p.niveau, p.nbPts);
    
    /*surfce.text = TTF_RenderUTF8_Shaded(surfce.font, texte, surfce.txtColor, surfce.txtBackColor);
    SDL_BlitSurface(surfce.text, NULL, surfce.screen, &surfce.txtPos);
    SDL_FreeSurface(surfce.text);*/
    
    free(texte);
}

int afficherPartie(Partie p, Surfaces *surfce, Snake s) {
    int i;
    
    /*S'assurer que le fond est blanc*/
    SDL_FillRect(surfce->screen, NULL, SDL_MapRGB(surfce->screen->format, 255, 255, 255));
    
    /*Afficher l'arriere plan*/
    afficherSurface(surfce->back, surfce->screen, 0, 0);
    
    /*Tete du serpent*/
    afficherSurface(surfce->snakeHead, surfce->screen, s.posX[0], s.posY[0]);
    
    /*Affichage du serpent*/
    for (i=1; i<s.length; i++) {
        afficherSurface(surfce->snake, surfce->screen, s.posX[i], s.posY[i]);
    }
    
    /*Affichage de la cible*/
    afficherSurface(surfce->cible, surfce->screen, p.ciblePosX, p.ciblePosY);
    
    /*afficherStatusBar(p, surfce);*/
    
    SDL_Flip(surfce->screen);
    
    return 0;
}
