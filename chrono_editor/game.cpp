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

#include <cstring>
#include "game.h"

enum { TECH_0 = 0x00, TECH_1 = 0x80, TECH_2 = 0xc0, TECH_3 = 0xe0, TECH_4 = 0xf0,
       TECH_5 = 0xf8, TECH_6 = 0xfc, TECH_7 = 0xfe, TECH_8 = 0xff };

Game::Game(game_t game, int slot) {
    _game = game;
    _slot = slot;
}

game_t Game::getGameStruct() {
    return _game;
}

void Game::setGameStruct(game_t game) {
    _game = game;
}

u8 Game::getSlot() {
    return _slot;
}

void Game::setSlot(u8 slot) {
    _slot = slot;
}

Character Game::getCharacter(u8 charId) {
    return Character(_game.characters[charId]);
}

void Game::setCharacter(Character c, u8 charId) {
    _game.characters[charId] = c.getCharStruct();
}

char* Game::getName(u8 charId) {
    for (int i=0; i<5; i++) {
        if (_game.names[charId][i] >= 0xa0 && _game.names[charId][i] <= 0xb9)
            _names[charId][i] = _game.names[charId][i] - 95;
        else if (_game.names[charId][i] >= 0xba && _game.names[charId][i] <= 0xd3)
            _names[charId][i] = _game.names[charId][i] - 89;
        else
            _names[charId][i] = _game.names[charId][i];
        _names[charId][5] = '\0';
    }

    return _names[charId];
}

void Game::setName(char* name, u8 charId) {
    int len = strlen(name);
    int i;

    for (i=0; i<5 || i < len; i++) {
        if (name[i] >= 'A' && name[i] <= 'Z')
            _game.names[charId][i] = name[i] + 95;
        else if (name[i] >= 'a' && name[i] <= 'z')
            _game.names[charId][i] = name[i] + 89;
        else
            _game.names[charId][i] = name[i];
    }
    _game.names[charId][i] = '\0';
}

u8 Game::getTechLevel(u8 charId) {
    if (charId > MAGUS)
        return 0;

    switch (_game.techLevel[charId]) {
        case TECH_0:
            return 0;
        case TECH_1:
            return 1;
        case TECH_2:
            return 2;
        case TECH_3:
            return 3;
        case TECH_4:
            return 4;
        case TECH_5:
            return 5;
        case TECH_6:
            return 6;
        case TECH_7:
            return 7;
        case TECH_8:
            return 8;
        default:
            return 0;
    }
}

void Game::setTechLevel(u8 level, u8 charId) {
    if (charId > MAGUS)
        return;

    switch (level) {
        case 0:
            _game.techLevel[charId] = TECH_0;
            break;
        case 1:
            _game.techLevel[charId] = TECH_1;
            break;
        case 2:
            _game.techLevel[charId] = TECH_2;
            break;
        case 3:
            _game.techLevel[charId] = TECH_3;
            break;
        case 4:
            _game.techLevel[charId] = TECH_4;
            break;
        case 5:
            _game.techLevel[charId] = TECH_5;
            break;
        case 6:
            _game.techLevel[charId] = TECH_6;
            break;
        case 7:
            _game.techLevel[charId] = TECH_7;
            break;
        case 8:
            _game.techLevel[charId] = TECH_8;
            break;
        default:
            _game.techLevel[charId] = TECH_0;
            break;
    }
}

u8 Game::getPartyMember(int membNo) {
    switch (membNo) {
        case 0:
            return _game.partyMember1;
        case 1:
            return _game.partyMember2;
        case 2:
            return _game.partyMember3;
        default:
            return -1;
    }
}

void Game::setPartyMember(u8 charId, int membNo) {
    if (charId > MAGUS)
        charId = 0x80;  // nobody

    switch (membNo) {
        case 0:
            _game.partyMember1 = charId;
            break;
        case 1:
            _game.partyMember2 = charId;
            break;
        case 2:
            _game.partyMember3 = charId;
            break;
    }
}

u16 Game::getSaveCount() {
    return _game.saveCount;
}

void Game::setSaveCount(u16 count) {
    if (count > 999)
        count = 999;

    _game.saveCount = count;
}

u32 Game::getGold() {
    return (_game.gold3 << 16 & 0xff0000)
         | (_game.gold2 <<  8 & 0x00ff00)
         | (_game.gold1       & 0x0000ff);
}

void Game::setGold(u32 gold) {
    if (gold > 9999999)
        gold = 9999999;

    _game.gold3 = gold >> 16 & 0x0000ff;
    _game.gold2 = gold >>  8 & 0x0000ff;
    _game.gold1 = gold       & 0x0000ff;
}

u32 Game::getTime() {
    return (_game.min1*10 + _game.min2) + (_game.hour1*10 + _game.hour2)*60;
}

void Game::setTime(u32 time) {
    int min, hour;
    if (time > 99*60 + 59)
        time = 99*60 + 59;

    min = time % 60;
    hour = time / 60;

    _game.hour1 = hour / 10;
    _game.hour2 = hour % 10;
    _game.min1 = min / 10;
    _game.min2 = min % 10;
}
