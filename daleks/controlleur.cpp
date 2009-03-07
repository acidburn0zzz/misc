#include "controlleur.h"

Controlleur::Controlleur() {
    _vue = new Vue(this);
    //_vue->setGeometry(100, 100, 0, 0);
    _vue->show();
    _game = NULL;
    _isStarted = false;
}

Controlleur::~Controlleur() {
    delete _game;
    delete _vue;
}

void Controlleur::nouvellePartie() {
    delete _game;
    _game = new Partie(this);
    _isStarted = true;
}

Dalek* Controlleur::getListeDaleks() {
    return _game->getListeDaleks();
}

Doc* Controlleur::getDoc() {
    return _game->getDoc();
}

int Controlleur::getNbDaleks() {
    return _game->getNbDaleks();
}

Partie* Controlleur::getPartie() {
    return _game;
}

int Controlleur::getLevel() {
    return _game->getLevel();
}

int Controlleur::getPts() {
    return _game->getPts();
}

int Controlleur::getZaps() {
    return _game->getZaps();
}

bool Controlleur::gameIsStarted() {
    return _isStarted;
}

Vue* Controlleur::getVue() {
    return _vue;
}

int Controlleur::getNbCases() {
    return _vue->getNbCases();
}

void Controlleur::moveDoc(int x, int y) {
    if (_game->moveDoc(x, y)) {
        moveDaleks();
        _game->checkMorts();
        _vue->drawGame();

        // On verifie l'etat de la partie
        if (_game->getStatutPartie() == 1) {
            monterNiveau();
        } else if (_game->getStatutPartie() == 2) {
            gameOver();
        }
    }
}

void Controlleur::teleport() {
    _game->teleportDoc();
    moveDaleks();
    _game->checkMorts();
    _vue->drawGame();

    // On verifie l'etat de la partie
    if (_game->getStatutPartie() == 1) {
        monterNiveau();
    } else if (_game->getStatutPartie() == 2) {
        gameOver();
    }
}

void Controlleur::zap() {
    if (_game->zapper()) {
        moveDaleks();
        _vue->drawGame();
    }

    // On verifie l'etat de la partie
    if (_game->getStatutPartie() == 1) {
        monterNiveau();
    } else if (_game->getStatutPartie() == 2) {
        gameOver();
    }
}

void Controlleur::run() {
    _game->run();
    _vue->drawGame();


    // On verifie l'etat de la partie
    if (_game->getStatutPartie() == 1) {
        monterNiveau();
    } else if  (_game->getStatutPartie() == 2) {
        gameOver();
    }
}

void Controlleur::moveDaleks() {
    _game->moveDaleks();
}

void Controlleur::monterNiveau() {
    _vue->showMessage("Congratulations", "You go up one level", MSG_INFORMATION);
    _game->monterNiveau();
    _vue->drawGame();
}

void Controlleur::gameOver() {
    _vue->askNewGameAfterLose();
}
