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
    _fn = fn;
}

void SRAMFile::read() throw(exception) {
    fstream file;
    long s;

    file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    try {
        file.open(_fn.c_str(), ios_base::in | ios_base::binary);
    } catch (exception e) {
        cerr << "Unable to open file: " << _fn << endl;
        throw e;
    }

    file.seekg(0, ios_base::end);
    s = file.tellg();

    if (s != 0x2000) {
        cerr << "Invalid SRAM file: " << _fn << endl;
        file.close();
        throw runtime_error("Invalid SRAM file");
    }

    file.seekg(0, ios_base::beg);
    file.read((char *)&sram, SRAM_SIZE);
    file.close();
}

void SRAMFile::write() throw(exception) {
    fstream file;
    file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    try {
        file.open(_fn.c_str(), ios_base::out | ios_base::binary);
    } catch (fstream::failure e) {
        cerr << "Unable to open file: " << _fn << endl;
        throw e;
    }

    computeChecksums();

    file.write((char *)&sram, SRAM_SIZE);
    file.close();
}

void SRAMFile::computeChecksums() {
    u32 checksum;
    u8 *pSram, *slot;
    int carry;

    pSram = (u8 *)&sram;

    for (int s=0; s<3; s++) {
        slot = (pSram + s * GAME_SIZE);
        checksum = 0x00000000;
        carry = 0;

        for (int i=GAME_SIZE-2; i>=0; i-=2) {
            checksum += *((u16 *)(slot + i));
            checksum += carry;

            carry = 0;

            if (checksum > 0xffff) {
                carry = 1;
                checksum &= 0xffff;
            }
        }

        *((u16 *)(pSram + CHECKSUM_OFFSET + 2 * s)) = checksum;
    }
}

Game *SRAMFile::getGame(int gameNo) {
    return new Game(&sram.games[gameNo]);
}

u8 SRAMFile::getLastGame() {
    return sram.lastGame;
}

void SRAMFile::setLastGame(u8 slot) {
    if (slot > 2)
        slot = 2;

    sram.lastGame = slot;
}

bool SRAMFile::getNewGamePlus() {
    return sram.ngPlus == 0 ? false : true;
}

void SRAMFile::setNewGamePlus(bool ngPlus) {
    sram.ngPlus = ngPlus ? 1 : 0;
}

void SRAMFile::clearGame(u8 slot) {
    if (slot > 2)
        return;

    memset((void *)&sram.games[slot], 0xff, GAME_SIZE);
    sram.isUsed[slot] = 0xffff;
}

void SRAMFile::foo(int gameNo) {
    hexdump(&sram.games[gameNo], 0, GAME_SIZE);
}
