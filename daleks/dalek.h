#ifndef _DALEK_H_
#define _DALEK_H_

class Dalek {
private:
    int _posX, _posY, _nbCases;
    bool _mort, _zappe;

public:
    Dalek();
    Dalek(int posX, int posY, int nbCases);
    void move(int drPosX, int drPosY);
    int getPos(); //retourne nbCases*X+Y (afin d'aller chercher X avec une division et Y avec un modulo)
    int getX();
    int getY();

    //Statuts : 0 = en vie, 1 = mort, 2 = zappe
    void setStatut(int statut);
    int getStatut();
};

#endif // _DALEK_H_
