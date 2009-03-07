#include <ctime>
#include <cstdlib>

#include "doc.h"

Doc::Doc(int posX, int posY, int nbCases, int nbDaleks) {
    _posX = posX;
    _posY = posY;
    _mort = false;
    _nbCases = nbCases;
    _nbDaleks = nbDaleks;

    srand(time(NULL));
}

int Doc::getPos() {
    return _posX * _nbCases + _posY;
}

int Doc::getX() {
    return _posX;
}

int Doc::getY() {
    return _posY;
}

void Doc::setMort(bool b) {
    _mort = b;
}

bool Doc::getMort() {
    return _mort;
}

bool Doc::move(int X, int Y, Dalek *listeDaleks) {
    int newX = _posX+X;
    int newY = _posY+Y;
    Dalek *daleks = listeDaleks;

    for (int i=0; i<_nbDaleks; i++) { //le docteur ne peut se déplacer sur les tas de feraille
        if (daleks[i].getPos() == newX*_nbCases+newY && daleks[i].getStatut() == 1) {
            return false;
        }
    }

    if (newX >= 0 && newX < _nbCases && newY >= 0 && newY < _nbCases) { // si la case de déplacement est dans la grille
        _posX = newX;
        _posY = newY;
        return true;
    } else { //sinon le déplacement est non valide
        return false;
    }
}

int Doc::teleport(Dalek *listeDaleks) {
    int newX, newY;
    Dalek *daleks = listeDaleks;

    newX = rand()%_nbCases;
    newY = rand()%_nbCases;

    //Le doc ne se teleporte pas sur un tas de feraille
    for (int i=0; i<_nbDaleks; i++){
        if (daleks[i].getPos() == newX*_nbCases+newY && daleks[i].getStatut() == 1) {
            return teleport(daleks);
        }
    }

    _posX = newX;
    _posY = newY;

    return 0;
}
