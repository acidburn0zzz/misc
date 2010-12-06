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

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdint.h>

#include <cstring>

#include "character.h"
#include "game.h"
#include "sramfile.h"

using namespace std;

SRAMFile::SRAMFile(const std::string fn) throw(exception) {
    this->fn = fn;
    sram = new unsigned char[SRAM_SIZE];

    read();
}

SRAMFile::~SRAMFile() {
    if (sram) delete [] sram;
}

void SRAMFile::read() throw(exception) {
    fstream file;
    long s;

    file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    try {
        file.open(fn.c_str(), ios_base::in | ios_base::binary);
    } catch (exception e) {
        cerr << "Unable to open file: " << fn << endl;
        throw e;
    }

    file.seekg(0, ios_base::end);
    s = file.tellg();

    if (s != 0x2000) {
        cerr << "Invalid SRAM file: " << fn << endl;
        file.close();
        throw runtime_error("Invalid SRAM file");
    }

    file.seekg(0, ios_base::beg);
    file.read((char*)sram, SRAM_SIZE);
    file.close();
}

void SRAMFile::write() throw(exception) {
    fstream file;
    file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    try {
        file.open(fn.c_str(), ios_base::out | ios_base::binary);
    } catch (fstream::failure e) {
        cerr << "Unable to open file: " << fn << endl;
        throw e;
    }

    checksum();

    file.write((char*)sram, SRAM_SIZE);
    file.close();
}

void SRAMFile::checksum() {
    uint32_t checksum;
    uint16_t *tmp, *offset, over;
    int g, i;

    for (g=0; g<3; g++) {
        checksum = 0x00000000;
        over = 0;
        offset = (uint16_t *)(sram + CHECKSUM_OFFSET + 2 * g);

        for (i=0; i<GAME_SIZE; i+=2) {
            tmp = (uint16_t *)(sram + i + g * GAME_SIZE);
            checksum += *tmp;
            checksum += over;

            over = 0;

            if (checksum > 0xffff) {
                over = 1;
                checksum &= 0xffff;
            }
        }

        *offset = (uint16_t)checksum;
    }
}

void SRAMFile::getGame(Game *game, int gameNo) {
    int i, gameOffset, charOffset;
    unsigned int gold;

    gameOffset = gameNo * GAME_SIZE;

    for (i=0; i<7; i++) {
        charOffset = gameOffset + VALUE_OFFSETS[CHARACTERS] + i * CHARACTER_SIZE;
        Character c;
        unsigned int exp;

        c.setId(sram[charOffset + CHAR_VALUE_OFFSETS[ID]]);
        c.setCharId(sram[charOffset + CHAR_VALUE_OFFSETS[CHAR_ID]]);
        c.setCurrentHP(*((unsigned short *)(sram + charOffset + CHAR_VALUE_OFFSETS[CUR_HP])));
        c.setMaxHP(*((unsigned short *)(sram + charOffset + CHAR_VALUE_OFFSETS[MAX_HP])));
        c.setCurrentMP(*((unsigned short *)(sram + charOffset + CHAR_VALUE_OFFSETS[CUR_MP])));
        c.setMaxMP(*((unsigned short *)(sram + charOffset + CHAR_VALUE_OFFSETS[MAX_MP])));
        c.setBasePower(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_POW]]);
        c.setBaseStamina(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_STA]]);
        c.setBaseSpeed(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_SPD]]);
        c.setBaseMagic(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_MAG]]);
        c.setBaseHit(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_HIT]]);
        c.setBaseEvade(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_EVD]]);
        c.setBaseMagicDef(sram[charOffset + CHAR_VALUE_OFFSETS[BASE_MDEF]]);
        c.setLevel(sram[charOffset + CHAR_VALUE_OFFSETS[LEVEL]]);
        c.setHelmet(sram[charOffset + CHAR_VALUE_OFFSETS[HELMET]]);
        c.setArmor(sram[charOffset + CHAR_VALUE_OFFSETS[ARMOR]]);
        c.setWeapon(sram[charOffset + CHAR_VALUE_OFFSETS[WEAPON]]);
        c.setRelic(sram[charOffset + CHAR_VALUE_OFFSETS[RELIC]]);
        c.setNameFromSRAM(sram + VALUE_OFFSETS[NAMES] + i * NAME_SIZE);

        exp = *((unsigned int *)(sram + charOffset + CHAR_VALUE_OFFSETS[EXP]));
        exp &= 0x00ffffff;
        c.setExp(exp);

        game->setCharacter(c, i);
    }

    gold = *((unsigned int *)(sram + gameOffset + VALUE_OFFSETS[GOLD]));
    gold &= 0x00ffffff;
    game->setGold(gold);
}

void SRAMFile::setGame(Game *game, int gameNo) {

}

void SRAMFile::foo() {
    Game g;

    getGame(&g, 0);

    for (int i=0; i<7; i++) {
        printf("Char %d\n", i);
        printf("\tName: %s\n", g.getCharacter(i).getName());
        printf("\tExp: %d\n", g.getCharacter(i).getExp());
        printf("\tHP: %d\n", g.getCharacter(i).getCurrentHP());
        printf("\tMP: %d\n", g.getCharacter(i).getCurrentMP());
        printf("\tWeapon: %s\n", itemList[g.getCharacter(i).getWeapon()]);
        printf("\tArmor: %s\n", itemList[g.getCharacter(i).getArmor()]);
        printf("\tHelmet: %s\n", itemList[g.getCharacter(i).getHelmet()]);
        printf("\tRelic: %s\n", itemList[g.getCharacter(i).getRelic()]);
    }

    printf("Gold: %d\n", g.getGold());

    write();
}
