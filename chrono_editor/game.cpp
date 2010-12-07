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

#include "game.h"

using namespace std;

Game::Game() {
}

Game::~Game() {
}

char Game::getNo() {
    return _no;
}

void Game::setNo(char no) {
    _no = no;
}

Character Game::getCharacter(int charNo) {
    return _chars[charNo];
}

void Game::setCharacter(Character c, int charNo) {
    _chars[charNo] = c;
}

unsigned int Game::getGold() {
    return _gold;
}

void Game::setGold(unsigned int gold) {
    _gold = gold;
}
