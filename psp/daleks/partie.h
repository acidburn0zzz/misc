#ifndef _PARTIE_H_
#define _PARTIE_H_

#include "struct.h"

Partie nouvellePartie();
void monterNiveau(Partie *p);
void reset(Partie *p);

/*daleks pointe sur le pointeur, pour que dans le main
daleks pointe sur la bonne adresse*/
void initNiveau(Partie p, Doc *doc, Dalek **daleks);

int zapper(Doc doc, Dalek *daleks, Partie *p);
int run(Doc *doc, Dalek *daleks, Partie *p, Surfaces surfce);

void checkMorts(Doc *doc, Dalek *daleks, Partie *p);
int getStatutPartie(Doc doc, Dalek *daleks, Partie p);

#endif /*_PARTIE_H_*/
