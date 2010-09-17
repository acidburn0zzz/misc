#ifndef _PARTIE_H_
#define _PARTIE_H_

#include "struct.h"

Partie nouvellePartie();
void monterNiveau(Partie *p);
void reset(Partie *p);

void initNiveau(Partie *p, Snake *s);
void creerCible(Snake s, Partie *p) ;

#endif /*_PARTIE_H_*/
