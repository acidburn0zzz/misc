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

#ifndef __SRAMFILE_H__
#define __SRAMFILE_H__

#include <iostream>

class Game;

enum char_value_offsets_name {
    ID, CHAR_ID, CUR_HP, MAX_HP, CUR_MP, MAX_MP, BASE_POW, BASE_STA,
    BASE_SPD, BASE_MAG, BASE_HIT, BASE_EVD, BASE_MDEF, LEVEL, EXP,
    HELMET, ARMOR, WEAPON, RELIC, XP_TO_NEXT, SP_TO_NEXT
};

static const int CHAR_VALUE_OFFSETS[] = {
    0x00, // ID
    0x01, // charID
    0x03, // Current HP
    0x05, // Max HP
    0x07, // Current MP
    0x09, // Max MP
    0x0b, // Base Power
    0x0c, // Base Stamina
    0x0d, // Base Speed
    0x0e, // Base Magic
    0x0f, // Base Hit
    0x10, // Base Evade
    0x11, // Base Magic Defence
    0x12, // Level
    0x13, // Experience
    0x27, // Helmet
    0x28, // Armor
    0x29, // Weapon
    0x2a, // Relic
    0x2b, // Exp to Level
    0x2d, // SP to Tech
    0x36, // Current Power
    0x37, // Current Stamina
    0x38, // Current Speed
    0x39, // Current Magic
    0x3a, // Current Hit
    0x3b, // Current Evade
    0x3c, // Current Magic Defence
    0x3d, // Current Attack
    0x3e, // Current Defence
    0x3f  // Current Max HP
};

static const int VALUE_OFFSETS[] = {
    0x0000, 0x0100, 0x0200, 0x0580,
    0x059C, 0x05B0, 0x05E0, 0x05E3,
    0x05F3, 0x05F5
};

enum value_offsets_name {
    ITEM_TYPE, ITEM_COUNT, CHARACTERS, PARTY_MEMBERS,
    SAVE_CNT, NAMES, GOLD, TIME,
    WORLD, PLAYER_POS
};

class SRAMFile {
public:
    static const int CHARACTER_SIZE = 0x0050;
    static const int NAME_SIZE = 0x0006;
    static const int GAME_SIZE = 0x0A00;
    static const int SRAM_SIZE = 0x2000;

    static const int GAME_OFFSET = 0x0000;
    static const int CHECKSUM_OFFSET = 0x1ff0;

    SRAMFile(const std::string fn) throw(std::exception);
    ~SRAMFile();

    void read() throw(std::exception);
    void write() throw(std::exception);

    void getGame(Game *game, int gameNo);
    void setGame(Game *game, int gameNo);

    void foo();

private:
    std::string fn;
    unsigned char *sram;

    void checksum();
};

#endif //__SRAMFILE_H__
