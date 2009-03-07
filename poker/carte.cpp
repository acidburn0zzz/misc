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

#include "carte.h"

Carte::Carte() {
    _couleur = 0;
    _valeur = 0;
    _sortie = false;
}

Carte::Carte(Carte *c) {
    _couleur = c->getCouleur();
    _valeur = c->getValeur();
    _sortie = c->estSortie();
}


Carte::Carte(char couleur, char valeur) {
    _couleur = couleur;
    _valeur = valeur;
    _sortie = false;
    
    if (_valeur == 1)
        _valeur = 14;
}

bool Carte::operator<(const Carte &c) {
    if (_valeur < c._valeur)
        return true;
    else
        return false;
}

bool Carte::operator>(const Carte &c) {
    if (_valeur > c._valeur)
        return true;
    else
        return false;
}

bool Carte::operator==(const Carte &c) {
    if (_valeur == c._valeur)
        return true;
    else
        return false;
}

bool Carte::operator==(const int i) {
    if (_valeur == i)
        return true;
    else
        return false;
}

bool Carte::operator!=(const Carte &c) {
    if (_valeur != c._valeur)
        return true;
    else
        return false;
}

bool Carte::operator!=(const int i) {
    if (_valeur != i)
        return true;
    else
        return false;
}

char Carte::getCouleur() {
    return _couleur;
}

char Carte::getValeur() {
    if (_valeur == 1)
        return 14;
    else
        return _valeur;
}

bool Carte::estSortie() {
    return _sortie;
}

void Carte::setCouleur(char c) {
    _couleur = c;
}

void Carte::setValeur(char v) {
    _valeur = v;
}

void Carte::setSortie(bool s) {
    _sortie = s;
}

void Carte::afficher() {
    char c = -1, v[3];
    
    switch (_couleur) {
        case COEUR:
            c = 'H';
            break;
        case CARREAU:
            c = 'D';
            break;
        case TREFLE:
            c = 'C';
            break;
        case PIQUE:
            c = 'S';
            break;
    }
    
    switch (_valeur) {
        case 10:
            v[0] = '1';
            v[1] = '0';
            v[2] = 0;
            break;
        case 11:
            v[0] = 'J';
            v[1] = 0;
            break;
        case 12:
            v[0] = 'Q';
            v[1] = 0;
            break;
        case 13:
            v[0] = 'K';
            v[1] = 0;
            break;
        case 14:
            v[0] = 'A';
            v[1] = 0;
            break;
        default:
            v[0] = _valeur + '0';
            v[1] = 0;
    }
    
    std::cout << v << c << std::endl;
}
