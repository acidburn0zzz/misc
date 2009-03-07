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

#ifndef __PAQUET_H__
#define __PAQUET_H__

class Carte;

class Paquet {
    private:
    Carte *_cartes;
    
    public:
    Paquet();
    Paquet(Paquet *p);
    ~Paquet();
    
    Carte getCarte(int i);
    Carte *tirerCarte();
    Carte *tirerCarte(char c, char v);
};

#endif //__PAQUET_H__
