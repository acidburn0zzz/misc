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

#include "jeu.h"
#include "paquet.h"
#include "simulation.h"

using namespace std;

Simulation::Simulation(int type, int nbJoueurs) {
    nb_seule = 0;
    nb_paires = 0;
    nb_deux_paires = 0;
    nb_brelan = 0;
    nb_suites = 0;
    nb_flush = 0;
    nb_full = 0;
    nb_carre = 0;
    nb_straight_f = 0;
    nb_royale = 0;
    nb_passes = 0;
    
    if (type == CALCUL) {
        simulerCalcul(nbJoueurs);
    }
}

void Simulation::simulerCalcul(int nbJoueurs) {
    Joueur *j = new Joueur[nbJoueurs];
    Paquet p;
    Carte comm[5];
    Jeu *jeu;
    int v;
    char c;
    
    /*for (int i=0; i<nbJoueurs; i++) {
        cout << "Veuillez entrer la carte #1 du joueur #" << i+1 << ": ";
        cin >> v >> c;
        
        switch(tolower(c)) {
            case 'h':
                c = COEUR;
                break;
            case 'd':
                c = CARREAU;
                break;
            case 'c':
                c = TREFLE;
                break;
            case 's':
                c = PIQUE;
                break;
        }
        
        j[i].c1 = p.tirerCarte(c, v);
        
        cout << "Veuillez entrer la carte #2 du joueur #" << i+1 << ": ";
        cin >> v >> c;
        
        switch(tolower(c)) {
            case 'h':
                c = COEUR;
                break;
            case 'd':
                c = CARREAU;
                break;
            case 'c':
                c = TREFLE;
                break;
            case 's':
                c = PIQUE;
                break;
        }
        
        j[i].c2 = p.tirerCarte(c, v);
    }*/
    
    j[0].c1 = p.tirerCarte(COEUR, 1);
    j[0].c2 = p.tirerCarte(COEUR, 13);
    
    //Boucle des calculs
    for (int a=0; a<52-4; a++) {
        if ((comm[0] = p.getCarte(a)).estSortie()) {
            continue;
        }
        
        for (int b=a+1; b<52-3; b++) {
            if ((comm[1] = p.getCarte(b)).estSortie()) {
                continue;
            }
        
            for (int c=b+1; c<52-2; c++) {
                if ((comm[2] = p.getCarte(c)).estSortie()) {
                    continue;
                }
                
                for (int d=c+1; d<52-1; d++) {
                    if ((comm[3] = p.getCarte(d)).estSortie()) {
                        continue;
                    }
                    
                    for (int e=d+1; e<52; e++) {
                        if ((comm[4] = p.getCarte(e)).estSortie()) {
                            continue;
                        }
                        
                        nb_passes++;
                        jeu = new Jeu(j, comm, nbJoueurs);
                        parseMain(0, jeu->getMainJoueur(0));
                        delete jeu;
                    }
                }
            }
        }
    }
    
    delete [] j;
}

void Simulation::parseMain(int joueur, int main) {
    switch (main) {
        case FLUSH_ROYALE:
            nb_royale++;
            break;
        case STRAIGHT_FLUSH:
            nb_straight_f++;
            break;
        case CARRE:
            nb_carre++;
            break;
        case MAIN_PLEINE:
            nb_full++;
            break;
        case FLUSH:
            nb_flush++;
            break;
        case SUITE:
            nb_suites++;
            break;
        case BRELAN:
            nb_brelan++;
            break;
        case DEUX_PAIRES:
            nb_deux_paires++;
            break;
        case PAIRE:
            nb_paires++;
            break;
        case CARTE_SEULE:
            nb_seule++;
            break;
    }
}

void Simulation::afficherStats(int joueur) {
    char format[] = "%-17s%8.4f%%%10d\n";
    printf("Apres %d jeux :\n", nb_passes);
    printf(format, "Flush Royale : ", (double)nb_royale/nb_passes*100, nb_passes/nb_royale);
    printf(format, "Straight Flush : ", (double)nb_straight_f/nb_passes*100, nb_straight_f);
    printf(format, "Carre : ", (double)nb_carre/nb_passes*100, nb_carre);
    printf(format, "Main Pleine : ", (double)nb_full/nb_passes*100, nb_full);
    printf(format, "Flush : ", (double)nb_flush/nb_passes*100, nb_flush);
    printf(format, "Suite : ", (double)nb_suites/nb_passes*100, nb_suites);
    printf(format, "Brelan : ", (double)nb_brelan/nb_passes*100, nb_brelan);
    printf(format, "Deux Paires : ", (double)nb_deux_paires/nb_passes*100, nb_deux_paires);
    printf(format, "Paire : ", (double)nb_paires/nb_passes*100, nb_paires);
    printf(format, "Carte Seule : ", (double)nb_seule/nb_passes*100, nb_seule);
}
