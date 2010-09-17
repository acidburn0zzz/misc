#ifndef _TOUCHES_H_
#define _TOUCHES_H_

typedef enum {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_UP_LEFT, KEY_UP_RIGHT,
    KEY_DOWN_LEFT, KEY_DOWN_RIGHT, KEY_STAY, KEY_ZAP, KEY_TELEPORT, KEY_RUN, KEY_QUIT} key;

int lireTouche();

#endif /*_TOUCHES_H_*/
