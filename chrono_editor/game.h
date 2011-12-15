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

#include "character.h"
#include "struct.h"

class Game {
public:
    Game(game_t game);

    //char getNo();
    //void setNo(char no);
    
    game_t getGameStruct();
    void setGameStruct(game_t);
    
    Character getCharacter(int charId);
    void setCharacter(Character c, int charId);

    u8*  getName(int charId);
    void setName(u8* name, int charId);

    u32 getGold();
    void setGold(u32 gold);

private:
    char _no;
    game_t _game;
    u8 _namesDec[8][6];

    void decodeName(int charId);
};

#endif //__GAME_H__
