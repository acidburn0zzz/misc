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

using namespace std;

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

//TODO: Use enum for charId
Character Game::getCharacter(u8 charId) {
    return Character(_game.characters[charId]);
}

void Game::setCharacter(Character c, u8 charId) {
    _game.characters[charId] = c.getCharStruct();
}

u8* Game::getName(u8 charId) {
    decodeName(charId);
    return _namesDec[charId];
}

void Game::setName(u8* name, u8 charId) {
    for (int i=0; i<5; i++) {
        if (name[i] >= 'A' && name[i] <= 'Z')
            _game.names[charId][i] = name[i] + 95;
        else if (name[i] >= 'a' && name[i] <= 'z')
            _game.names[charId][i] = name[i] + 89;
        else
            _game.names[charId][i] = name[i];
    }
    _game.names[charId][5] = '\0';
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

void Game::decodeName(u8 charId) {
    for (int i=0; i<5; i++) {
        if (_game.names[charId][i] >= 0xa0 && _game.names[charId][i] <= 0xb9)
            _namesDec[charId][i] = _game.names[charId][i] - 95;
        else if (_game.names[charId][i] >= 0xba && _game.names[charId][i] <= 0xd3)
            _namesDec[charId][i] = _game.names[charId][i] - 89;
        else
            _namesDec[charId][i] = _game.names[charId][i];
        _namesDec[charId][5] = '\0';
    }
}
