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

#ifndef __CARTE_H__
#define __CARTE_H__

enum {COEUR, CARREAU, TREFLE, PIQUE};

class Carte {
    private:
        char _couleur;
        char _valeur;
        bool _sortie;

    public:
        Carte();
        Carte(Carte *c);
        Carte(char couleur, char valeur);
    
        bool operator<(const Carte &c);
        bool operator>(const Carte &c);
        bool operator==(const Carte &c);
        bool operator==(const int i);
        bool operator!=(const Carte &c);
        bool operator!=(const int i);
    
        char getCouleur();
        char getValeur();
        bool estSortie();
    
        void setCouleur(char c);
        void setValeur(char v);
        void setSortie(bool s);
    
        void afficher();
};

#endif //__CARTE_H__
