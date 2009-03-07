#ifndef _PARTIE_H_
#define _PARTIE_H_

#include <iostream>

#include "dalek.h"
#include "doc.h"

class Controlleur;

class Partie {
private:
    Controlleur *_cont;
    Dalek *_listeDaleks;
    Doc *_doc;
    int _level, _nbreZap, _nbrePts, _nbCases, _nbreDaleks;
    std::string _cheat;
    bool _cheatEnabled;

public:
    Partie(Controlleur *cont);
    ~Partie();

    Dalek* getListeDaleks();
    Doc* getDoc();
    int getNbDaleks();
    int getLevel();
    int getPts();
    int getZaps();

    Dalek creerDalek();
    Doc* creerDoc();
    void initPartie();
    void monterNiveau();
    void moveDaleks();
    bool moveDoc(int x, int y);
    void teleportDoc();
    bool zapper();
    void checkMorts();
    void run();
    void formerCheat(char c);
    int getStatutPartie();
};

#endif //_PARTIE_H_
