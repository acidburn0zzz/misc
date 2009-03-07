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
#include <cstdlib>

#include "carte.h"
#include "paquet.h"

Paquet::Paquet() {
    _cartes = new Carte[52];

    for (int c=0; c<4; c++) {
        for (int v=0; v<13; v++) {
            _cartes[13*c+v] = Carte(c, v+1);
        }
    }
}

Paquet::Paquet(Paquet *p) {
    _cartes = new Carte[52];

    for (int i=0; i<52; i++) {
        _cartes[52] = p->getCarte(i);
    }
}

Paquet::~Paquet() {
    delete [] _cartes;
}

Carte Paquet::getCarte(int i) {
    return _cartes[i];
}

Carte *Paquet::tirerCarte() {
    int i = rand()%52;
    Carte *c = &_cartes[i];

    if (c->estSortie())
        c = tirerCarte();

    c->setSortie(true);
    return c;
}

Carte *Paquet::tirerCarte(char c, char v) {
    int i = 13*c+(v-1);
    Carte *cte = &_cartes[i];

    cte->setSortie(true);
    return cte;
}
