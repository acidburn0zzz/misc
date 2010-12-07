/***************************************************************************
 * Copyright (C) 2010 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>

#include "character.h"
#include "struct.h"

class Game {
public:
    Game();
    ~Game();

    char getNo();
    void setNo(char no);

    Character getCharacter(int charNo);
    void setCharacter(Character c, int charNo);
    unsigned int getGold();
    void setGold(unsigned int gold);

private:
    char _no;

    Character _chars[7];
    s_items _items;
    unsigned int _gold;
    s_time _time;
};

#endif //__GAME_H__
