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
#include <iomanip>
#include <fstream>

#include "game.h"
#include "sramfile.h"
#include "struct.h"

using namespace std;

int main(int, char **) {
    cout << sizeof(sram_t) << endl;
    try {
        SRAMFile sf(std::string("test.srm"));
        sf.read();
        
        Game g = sf.getGame(0);
        Character c = g.getCharacter(0);
        cout << g.getName(0) << endl;
        character_t cs = c.getCharStruct();
        cout << cs.maxHP << endl;
        //sf.setGame(g, 1);
        //sf.write();
        //sf.foo(0);
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
