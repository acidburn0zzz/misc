#ifndef _DOC_H_
#define _DOC_H_

#include "struct.h"

Doc creerDoc(Dalek *daleks, Partie p);
int moveDoc(int x, int y, Doc *doc, Dalek *daleks, Partie p);
int teleportDoc(Doc *doc, Dalek *daleks, Partie p);

#endif /*_DOC_H_*/
