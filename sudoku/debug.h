#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "grille.h"

void debugAfficherDispos(Grille *g, bool justeUn);
void debugAfficherRestants(Grille *g);
void debugPrintGrille(Grille *g);
void debugCheckErreurs(Grille *g);
void debugResoudreGrille(Grille *g);
void debugTestBloc(Grille *g);

#endif //__DEBUG_H__
