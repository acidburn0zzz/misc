/**************************************************************************
 *  AcidRain's Poker Odds Calculator                                      *
 *  Copyright (C) 2008 by Mathieu Lemay <acidrain1@gmail.com>             *
 *                                                                        *
 *  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 **************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#include "simulation.h"

using namespace std;

int main(int argc, char **argv) {
    int seed = time(NULL);
    int nb_passes = 0;
    Simulation *s;
    
#ifdef __gnu_linux__
    FILE *f;
    unsigned char c[1];
    
    f = fopen("/dev/urandom", "rb");
    if (f != NULL) {
        if (fread(c, 1, 1, f) != 1)
            c[0] = 0;
        fclose(f);
        seed += c[0];
    }
#endif
    stringstream ss;
    if (argc == 2) {
        string s(argv[1]);
        stringstream ss(s);
        ss >> nb_passes;
    }
    
    srand(seed);

    s = new Simulation(CALCUL);
    s->afficherStats();
    
    return EXIT_SUCCESS;
}
