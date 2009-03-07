#include "dalek.h"

Dalek::Dalek() {
    _posX = 0;
    _posY = 0;
    _nbCases = 30;
    _mort = false;
    _zappe = false;
}

Dalek::Dalek(int posX, int posY, int nbCases) {
    _posX = posX;
    _posY = posY;
    _nbCases = nbCases;
    _mort = false;
    _zappe = false;
}

void Dalek::move(int drPosX, int drPosY) {
    if (getStatut() == 0) {
        if (_posX > drPosX) {
            _posX--;
        }
        if (_posX < drPosX) {
            _posX++;
        }
        if (_posY > drPosY) {
            _posY--;
        }
        if (_posY < drPosY) {
            _posY++;
        }
    }
}

//Statuts : 0 = en vie, 1 = mort, 2 = zappe
int Dalek::getStatut() {
    if (_zappe) {
        return 2;
    } else if (_mort) {
        return 1;
    } else {
        return 0;
    }
}

void Dalek::setStatut(int statut) {
    if (statut == 0) {
        _mort = false;
        _zappe = false;
    } else if (statut == 1) {
        _mort = true;
        _zappe = false;
    } else {
        _mort = true;
        _zappe = true;
    }
}

int Dalek::getPos() {
    return _posX * _nbCases + _posY;
}

int Dalek::getX() {
    return _posX;
}

int Dalek::getY() {
    return _posY;
}
