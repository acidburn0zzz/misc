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

#ifndef __SIMULATION_H__
#define __SIMULATION_H__

enum {CALCUL, RANDOM};

class Simulation {
    private:
        int nb_seule;
        int nb_paires;
        int nb_deux_paires;
        int nb_brelan;
        int nb_suites;
        int nb_flush;
        int nb_full;
        int nb_carre;
        int nb_straight_f;
        int nb_royale;
        unsigned int nb_passes;
    
    public:
        Simulation(int type, int nbJoueurs = 1);
    
        void simulerCalcul(int nbJoueurs);
        void simulerRandom(int nbJoueurs, unsigned int nbPasses = 1000000);
        void parseMain(int joueur, int main);
        void afficherStats(int joueur = 0);
};

#endif //__SIMULATION_H__
