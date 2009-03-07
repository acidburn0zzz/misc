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

#include "jeu.h"
#include "paquet.h"

#include <iostream>

Jeu::Jeu(int nbJoueurs) {
    _paquet = new Paquet();
    
    if (nbJoueurs > 10)
        nbJoueurs = 10;
    
    _nbJoueurs = nbJoueurs;
    
    //Distribution des cartes aux joueurs
    for (int i=0; i<nbJoueurs; i++) {
        //On en profite pour initialiser les valeurs
        _joueurs[i].valeur_carre = 0;
        _joueurs[i].valeur_brelan = 0;
        _joueurs[i].valeur_paire_1 = 0;
        _joueurs[i].valeur_paire_2 = 0;
        _joueurs[i].c1 = _paquet->tirerCarte();
    }
    for (int i=0; i<nbJoueurs; i++) {
        _joueurs[i].c2 = _paquet->tirerCarte();
    }
    
    //Distribution des cartes communes
    Carte tmp;
    tmp = _paquet->tirerCarte();
    _communaute[0] = _paquet->tirerCarte();
    _communaute[1] = _paquet->tirerCarte();
    _communaute[2] = _paquet->tirerCarte();
    tmp = _paquet->tirerCarte();
    _communaute[3] = _paquet->tirerCarte();
    tmp = _paquet->tirerCarte();
    _communaute[4] = _paquet->tirerCarte();
}

Jeu::Jeu(Joueur j[], Carte c[], int nbJoueurs) {
    _paquet = NULL;
    
    if (nbJoueurs > 10)
        nbJoueurs = 10;
    
    _nbJoueurs = nbJoueurs;
    
    //Initialisation des joueurs
    for (int i=0; i<nbJoueurs; i++) {
        _joueurs[i] = j[i];
        _joueurs[i].valeur_carre = 0;
        _joueurs[i].valeur_brelan = 0;
        _joueurs[i].valeur_paire_1 = 0;
        _joueurs[i].valeur_paire_2 = 0;
    }
    
    for (int i=0; i<5; i++) {
        _communaute[i] = c[i];
    }
}

Jeu::~Jeu() {
    if (_paquet)
        delete _paquet;
}

int Jeu::getMainJoueur(int joueur) {
    //Remplir le tableau de cartes pour les tests
    Joueur *j = &_joueurs[joueur];
    Carte cartes[7];
    
    cartes[0] = j->c1;
    cartes[1] = j->c2;
    
    for (int i=0; i<5; i++)
        cartes[i+2] = _communaute[i];
    
    if (flush_royale(j, cartes)) {
        return FLUSH_ROYALE;
    }
    
    if (straight_flush(j, cartes)) {
        return STRAIGHT_FLUSH;
    }
    
    if (carre(j, cartes)) {
        return CARRE;
    }
    
    if (flush(j, cartes)) {
        return FLUSH;
    }
    
    if (suite(j, cartes)) {
        return SUITE;
    }
    
    if (main_pleine_brelan_paires(j, cartes)) {
        return j->main;
    }
    
    //Rendu ici on a une carte seule
    //On va former la main du joueur avec les 5 meilleures
    
    //Classer les cartes en ordre
    for (int i=0; i<7-1; i++) {
        for (int j=i+1; j<7; j++) {
            if (cartes[i] > cartes[j]) {
                Carte tmp = cartes[i];
                cartes[i] = cartes[j];
                cartes[j] = tmp;
            }
        }
    }
    
    for (int i=0; i<5; i++)
        j->c_jeu[i] = cartes[i+2];
    
    j->main = CARTE_SEULE;
    return CARTE_SEULE;
}

bool Jeu::flush_royale(Joueur *joueur, Carte *cartes) {
    int h = 0,
        d = 0,
        c = 0,
        s = 0;
    int couleur = -1;
    int nb = -1;
    
    for (int i=0; i<7; i++) {
        switch (cartes[i].getCouleur()) {
            case COEUR:
                h++;
                break;
            case CARREAU:
                d++;
                break;
            case TREFLE:
                c++;
                break;
            case PIQUE:
                s++;
                break;
        }
    }
    
    if (h >= 5) {
        couleur = COEUR;
        nb = h;
    } else if (d >= 5) {
        couleur = CARREAU;
        nb = d;
    } else if (c >= 5) {
        couleur = TREFLE;
        nb = c;
    } else if (s >= 5) {
        couleur = PIQUE;
        nb = s;
    }
    
    if (couleur != -1) {
        //On isole les cartes de la couleur
        Carte *c_coul = new Carte[nb];
        int c_ind = 0;
        
        for (int i=0; i<7; i++) {
            if (cartes[i].getCouleur() == couleur) {
                c_coul[c_ind++] = cartes[i];
            }
        }
        
        //On trie les cartes de la couleur
        for (int i=0; i<nb-1; i++) {
            for (int j=i+1; j<nb; j++) {
                if (c_coul[i] > c_coul[j]) {
                    Carte tmp = c_coul[i];
                    c_coul[i] = c_coul[j];
                    c_coul[j] = tmp;
                }
            }
        }
        
        //On regarde a la fin pour la flush royale
        if (c_coul[nb-1] == 14 && c_coul[nb-5] == 10) {
            joueur->main = FLUSH_ROYALE;
            return true;
        }
    }
    
    return false;
}

bool Jeu::straight_flush(Joueur *joueur, Carte *cartes) {
    int h = 0,
        d = 0,
        c = 0,
        s = 0;
    int couleur = -1;
    int nb = -1;
    
    for (int i=0; i<7; i++) {
        switch (cartes[i].getCouleur()) {
            case COEUR:
                h++;
                break;
            case CARREAU:
                d++;
                break;
            case TREFLE:
                c++;
                break;
            case PIQUE:
                s++;
                break;
        }
    }
    
    if (h >= 5) {
        couleur = COEUR;
        nb = h;
    } else if (d >= 5) {
        couleur = CARREAU;
        nb = d;
    } else if (c >= 5) {
        couleur = TREFLE;
        nb = c;
    } else if (s >= 5) {
        couleur = PIQUE;
        nb = s;
    }
    
    if (couleur != -1) {
        //On isole les cartes de la couleur
        Carte *c_coul = new Carte[nb];
        int c_ind = 0;
        
        for (int i=0; i<7; i++) {
            if (cartes[i].getCouleur() == couleur) {
                c_coul[c_ind++] = cartes[i];
            }
        }
        
        //On trie les cartes de la couleur
        for (int i=0; i<nb-1; i++) {
            for (int j=i+1; j<nb; j++) {
                if (c_coul[i] > c_coul[j]) {
                    Carte tmp = c_coul[i];
                    c_coul[i] = c_coul[j];
                    c_coul[j] = tmp;
                }
            }
        }
        
        //On cherche une suite dans la couleur
        for (int i=nb-1; i>=4; i--) {
            if (c_coul[i-4] == c_coul[i].getValeur() - 4) {
                joueur->main = STRAIGHT_FLUSH;
                return true;
            }
        }
    }
    
    return false;
}

bool Jeu::carre(Joueur *joueur, Carte *cartes) {
    //On commence par mettre les cartes en ordre
    for (int i=0; i<7-1; i++) {
        for (int j=i+1; j<7; j++) {
            if (cartes[i] > cartes[j]) {
                Carte tmp = cartes[i];
                cartes[i] = cartes[j];
                cartes[j] = tmp;
            }
        }
    }
    
    for (int i=0; i<4; i++) {
        if (cartes[i] == cartes[i+3]) {
            joueur->main = CARRE;
            joueur->valeur_carre = cartes[i].getValeur();
            return true;
        }
    }
    
    return false;
}

bool Jeu::main_pleine_brelan_paires(Joueur *joueur, Carte *cartes) {
    int nb_paires = 0,
        nb_brelan = 0;
    //On commence par mettre les cartes en ordre
    for (int i=0; i<7-1; i++) {
        for (int j=i+1; j<7; j++) {
            if (cartes[i] > cartes[j]) {
                Carte tmp = cartes[i];
                cartes[i] = cartes[j];
                cartes[j] = tmp;
            }
        }
    }
    
    //1ere passe : Brelan
    //La verification part de la fin pour
    //s'assurer d'avoir le + fort
    for (int i=4; i>=0; i--) {
        if (cartes[i] == cartes[i+2]) {
            joueur->valeur_brelan = cartes[i].getValeur();
            nb_brelan = 1;
            break;
        }
    }
    
    //2eme passe : Paires
    for (int i=5; i>=0; i--) {
        if (cartes[i] == cartes[i+1] && cartes[i] != joueur->valeur_brelan) {
            if (nb_paires == 0) {
                joueur->valeur_paire_1 = cartes[i].getValeur();
                nb_paires++;
            } else if (nb_paires == 1) {
                joueur->valeur_paire_2 = cartes[i].getValeur();
                nb_paires++;
                break;
            }
        }
    }
    
    //Creation de la main du joueur
    if (nb_brelan && nb_paires) { //Main pleine
        joueur->main = MAIN_PLEINE;
        
        int j = 0;
        for (int i=0; i<7; i++) {
            if (cartes[i] == joueur->valeur_brelan || cartes[i] == joueur->valeur_paire_1) {
                joueur->c_jeu[j] = cartes[i];
                j++;
            }
        }
        
        return true;
    } else if (nb_brelan) {
        joueur->main = BRELAN;
        
        int j = 0,
            nb_cartes_autre = 2;
        
        for (int i=6; j<5; i--) {
            if (cartes[i] == joueur->valeur_brelan || nb_cartes_autre) {
                joueur->c_jeu[j] = cartes[i];
                j++;
            }
        }
        
        return true;
    } else if (nb_paires == 2) {
        joueur->main = DEUX_PAIRES;
        
        int j = 0,
            nb_cartes_autre = 1;
        
        for (int i=6; j<5; i--) {
            if (cartes[i] == joueur->valeur_paire_1 ||
                cartes[i] == joueur->valeur_paire_2 || nb_cartes_autre) {
                joueur->c_jeu[j] = cartes[i];
                j++;
            }
        }
        
        return true;
    } else if (nb_paires == 1) {
        joueur->main = PAIRE;
        
        int j = 0,
            nb_cartes_autre = 3;
        
        for (int i=6; j<5; i--) {
            if (cartes[i] == joueur->valeur_paire_1 || nb_cartes_autre) {
                joueur->c_jeu[j] = cartes[i];
                j++;
            }
        }
        
        return true;
    }
    
    return false;
}

bool Jeu::flush(Joueur *joueur, Carte *cartes) {
    int h = 0,
        d = 0,
        c = 0,
        s = 0;
    int couleur = -1;
    
    for (int i=0; i<7; i++) {
        switch (cartes[i].getCouleur()) {
            case COEUR:
                h++;
                break;
            case CARREAU:
                d++;
                break;
            case TREFLE:
                c++;
                break;
            case PIQUE:
                s++;
                break;
        }
    }
    
    if (h >= 5)
        couleur = COEUR;
    else if (d >= 5)
        couleur = CARREAU;
    else if (c >= 5)
        couleur = TREFLE;
    else if (s >= 5)
        couleur = PIQUE;
    
    if (couleur != -1) {
        joueur->main = FLUSH;
        return true;
    } else {
        return false;
    }
}

bool Jeu::suite(Joueur *joueur, Carte *cartes) {
    Carte c_simple[7];
    
    //On commence par mettre les cartes en ordre
    for (int i=0; i<7-1; i++) {
        for (int j=i+1; j<7; j++) {
            if (cartes[i] > cartes[j]) {
                Carte tmp = cartes[i];
                cartes[i] = cartes[j];
                cartes[j] = tmp;
            }
        }
    }
    
    //Creation d'un tableau sans les doubles
    int j = 1; //Contiendra le nbre de cartes du nouveau tableau
    int prev = cartes[0].getValeur();
    c_simple[0] = cartes[0];
    
    for (int i=1; i<7; i++) {
        if (cartes[i] != prev) {
            c_simple[j] = cartes[i];
            prev = cartes[i].getValeur();
            j++;
        }
    }
    
    for (int i=0; i<j-4; i++) {
        if (c_simple[i] == c_simple[i+4].getValeur() - 4) {
            joueur->main = SUITE;
            
            //Creation de la main
            for (int k=0; k<5; k++) {
                joueur->c_jeu[k] = c_simple[k+i];
            }
            
            return true;
        }
    }
    
    return false;
}
