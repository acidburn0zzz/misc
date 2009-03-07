#ifndef _CONTROLLEUR_H_
#define _CONTROLLEUR_H_

#include "dalek.h"
#include "partie.h"
#include "vue.h"

class Controlleur {
private:
    Vue *_vue;
    Partie *_game;
    bool _isStarted;

public:
    Controlleur();
    ~Controlleur();

    //Parlent a Partie
    Dalek* getListeDaleks();
    Doc* getDoc();
    int getNbDaleks();
    Partie* getPartie();
    int getLevel();
    int getPts();
    int getZaps();
    bool gameIsStarted();

    void nouvellePartie();
    void moveDoc(int x, int y);
    void teleport();
    void zap();
    void run();
    void moveDaleks();
    void monterNiveau();
    void gameOver();


    //Parlent a Vue
    int getNbCases();
    Vue* getVue();
};

#endif //_CONTROLLEUR_H_
