#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "struct.h"

typedef enum Direction {HAUT, BAS, GAUCHE, DROITE} Direction;

void resetSnake(Snake *s, Partie p);
void moveSnake(Snake *s, Partie p);
void allongerSnake(Snake *s);
void changeDirectionSnake(Snake *s, int direction);
int checkCollisionSnake(Snake s, Partie p);
int checkCibleSnake(Snake s, Partie p);

#endif /*_SNAKE_H_*/
