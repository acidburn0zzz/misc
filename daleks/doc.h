#ifndef _DOC_H_
#define _DOC_H_

#include "dalek.h"

class Doc {
private:
    int _posX, _posY, _nbCases, _nbDaleks;
    bool _mort;

public:
    Doc(int posX, int posY, int nbCases, int nbDaleks);

    void setMort(bool b);
    bool getMort();
    int getPos(); //retourne nbCases*X+Y (afin d'aller chercher X avec une division et Y avec un modulo)
    int getX();
    int getY();

    bool move(int X, int Y, Dalek *listeDaleks);
    int teleport(Dalek *listeDaleks);
};

#endif // _DOC_H_
