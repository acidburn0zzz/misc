#include <iostream>

#include "controlleur.h"
#include "partie.h"

Partie::Partie(Controlleur *cont) {
    _cont = cont;
    _level = 1;
    _nbreDaleks = _level * 5;
    _nbreZap = 1;
    _nbrePts = 0;
    _nbCases = 30;
    _listeDaleks = NULL;
    _doc = NULL;
    _cheat = "";
    _cheatEnabled = false;
    initPartie();
}

Partie::~Partie() {
    delete _doc;
    delete[] _listeDaleks;
}

Dalek* Partie::getListeDaleks() {
    return _listeDaleks;
}

Doc* Partie::getDoc() {
    return _doc;
}

int Partie::getNbDaleks() {
    return _nbreDaleks;
}

int Partie::getLevel() {
    return _level;
}

int Partie::getPts() {
    return _nbrePts;
}

int Partie::getZaps() {
    return _nbreZap;
}

Dalek Partie::creerDalek() {
    int posX, posY;
    posX = rand()%_nbCases;
    posY = rand()%_nbCases;

    for (int i=0; i<_nbreDaleks; i++) {
        if (_listeDaleks[i].getPos() == posX*_nbCases+posY) {
            return creerDalek();
        }
    }

    return Dalek(posX, posY, _nbCases);
}

Doc* Partie::creerDoc() {
    int posX, posY;
    posX = rand()%_nbCases;
    posY = rand()%_nbCases;

    for (int i=0; i<_nbreDaleks; i++) {
        if (_listeDaleks[i].getPos() == posX*_nbCases+posY) {
            return creerDoc();
        }
    }

    return new Doc(posX, posY, _nbCases, _nbreDaleks);
}

void Partie::initPartie() {
    delete _doc;
    delete[] _listeDaleks;

    _listeDaleks = new Dalek[_nbreDaleks];

    for (int i=0; i<_nbreDaleks; i++) {
        _listeDaleks[i] = creerDalek();
    }

    _doc = creerDoc();
}

void Partie::monterNiveau() {
    _level++;
    _nbreDaleks = _level * 5;
    _nbreZap++;
    _cheat = "";
    _cheatEnabled = false;
    initPartie();
}

void Partie::moveDaleks() {
    int docX, docY;
    docX = _doc->getX();
    docY = _doc->getY();
    for (int i=0; i<_nbreDaleks; i++) {
        _listeDaleks[i].move(docX, docY);
    }
}

bool Partie::moveDoc(int x, int y) {
    return _doc->move(x, y, _listeDaleks);
}

void Partie::teleportDoc() {
    _doc->teleport(_listeDaleks);
}

bool Partie::zapper() {
    if (_nbreZap > 0) {
        for (int i=0; i<_nbreDaleks; i++) {
            if (abs(_listeDaleks[i].getX()-_doc->getX()) <= 1
                    && abs(_listeDaleks[i].getY()-_doc->getY()) <=1
                    && _listeDaleks[i].getStatut() == 0) {
                _listeDaleks[i].setStatut(2);
                _nbrePts += 5;
            }
        }
        _nbreZap--;
        return true;
    } else {
        return false;
    }
}

void Partie::checkMorts() {
    //Si le doc est sur un dalek
    for (int i=0; i<_level*5; i++) {
        if (_listeDaleks[i].getPos() == _doc->getPos() && _listeDaleks[i].getStatut() != 2) {
            _doc->setMort(true);
        }
    }

    //Collisions entre daleks
    for (int i=0; i<_nbreDaleks-1; i++) {
        for (int j=i+1; j<_nbreDaleks; j++) {
            if (_listeDaleks[i].getPos() == _listeDaleks[j].getPos() && _listeDaleks[i].getStatut() != 2 && _listeDaleks[j].getStatut() != 2) {
                if (_listeDaleks[i].getStatut() == 0) {
                    _listeDaleks[i].setStatut(1);
                    _nbrePts += 5;
                }
                if (_listeDaleks[j].getStatut() == 0) {
                    _listeDaleks[j].setStatut(1);
                    _nbrePts += 5;
                }
            }
        }
    }
}

void Partie::run() {
    bool ok = true;

    //On verifie si il n'y a pas de dalek colles au doc
    for (int i=0; i<_nbreDaleks; i++) {
        if (abs(_listeDaleks[i].getX()-_doc->getX()) <= 1
                    && abs(_listeDaleks[i].getY()-_doc->getY()) <=1
                    && _listeDaleks[i].getStatut() == 0) {
            ok = false;
        }
    }

    while (ok) {
        moveDaleks();
        checkMorts();

        //On verifie l'etat de la partie
        if (getStatutPartie() == 1)
            _cont->monterNiveau();
        else if (getStatutPartie() == 2)
            _cont->gameOver();

        //On verifie si il n'y a pas de dalek colles au doc
        for (int i=0; i<_nbreDaleks; i++) {
            if (abs(_listeDaleks[i].getX()-_doc->getX()) <= 1
                        && abs(_listeDaleks[i].getY()-_doc->getY()) <=1
                        && _listeDaleks[i].getStatut() == 0) {
                ok = false;
            }
        }
    }
}

void Partie::formerCheat(char c) {
    if (_cheat.length() < 4) {
        _cheat += c;
    } else {
        _cheat = _cheat.substr(1, 3) + c;
    }
    if (_cheat == "dude") {
        _cheatEnabled = true;
        //TODO: Message
    }
}

int Partie::getStatutPartie() {
    /**********
     * Valeurs de retour
     * 0 : On continue
     * 1 : Partie gagnée
     * 2 : Perdu
    **********/
    if (_doc->getMort()) {
        return 2;
    }
    for (int i=0; i<_nbreDaleks; i++) {
        if (_listeDaleks[i].getStatut() == 0) {
            return 0;
        }
    }
    return 1;
}
