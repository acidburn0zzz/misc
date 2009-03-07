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

#ifndef __JEU_H__
#define __JEU_H__

#include "carte.h"

class Paquet;

enum {CARTE_SEULE, PAIRE, DEUX_PAIRES, BRELAN, SUITE, FLUSH, MAIN_PLEINE, CARRE, STRAIGHT_FLUSH, FLUSH_ROYALE};

struct Joueur {
    Carte c1;
    Carte c2;
    int main;
    
    //Les 5 cartes utilisees pour le jeu
    //Triees par valeur
    Carte c_jeu[5];
    
    //Carre
    int valeur_carre;
    
    //Brelan
    int valeur_brelan;
    
    //Paires
    int valeur_paire_1;
    int valeur_paire_2;
};

class Jeu {
    private:
    Paquet *_paquet;
    Joueur _joueurs[10];
    Carte _communaute[5];
    int _nbJoueurs;
    
    public:
    Jeu(int nbJoueurs = 2);
    Jeu(Joueur j[], Carte c[], int nbJoueurs = 2);
    ~Jeu();
    
    int getMainJoueur(int joueur);
    bool flush_royale(Joueur *joueur, Carte *cartes);
    bool straight_flush(Joueur *joueur, Carte *cartes);
    bool carre(Joueur *joueur, Carte *cartes);
    bool main_pleine_brelan_paires(Joueur *joueur, Carte *cartes);
    bool flush(Joueur *joueur, Carte *cartes);
    bool suite(Joueur *joueur, Carte *cartes);
};

#endif //__JEU_H__
