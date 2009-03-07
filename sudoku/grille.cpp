#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "grille.h"

using namespace std;

Grille::Grille(int difficulte) {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            _cases[i][j].fixe = false;
            _cases[i][j].valeur = 0;
        }
    }
    
    chargerGrille(difficulte); //En attendant de pouvoir generer les grilles
    //genererGrille();
}

Grille::~Grille() {
    /**
        Dude 1: Guess what?
        Dude 2: What?
        Dude 1: We're useless!!!
    */
}

int Grille::getValeurCase(int x, int y) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    return _cases[x][y].valeur;
}

int Grille::setValeurCase(int x, int y, int valeur) {
    //Si le test getFixeCase est active, la grille se remplit toute seule !?!?
    if (x < 0 || x > 8 || y < 0 || y > 8/* || getFixeCase(x, y) == 1*/) {
        return -1;
    }
    
    if (valeur < 0 || valeur > 9) {
        return -1;
    }
    
    _cases[x][y].valeur = valeur;
    
    if (valeur != 0) {
        _cases[x][y].dispos = 0;
        _cases[x][y].nbDispos = 0;
    }
    
    return valeur;
}

int Grille::getFixeCase(int x, int y) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    return (_cases[x][y].fixe == true) ? 1 : 0;
}

int Grille::setFixeCase(int x, int y, bool fixe) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    _cases[x][y].fixe = fixe;
    
    return 0;
}

int Grille::getValeurSolution(int x, int y) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    return _solution[x][y].valeur;
}

void Grille::chargerGrille(int difficulte) {
    fstream f;
    string f_name;
    int taille;
    char *buffer = new char[81];
    
    switch (difficulte) {
        case FACILE:
            f_name = "puzzles/01_facile";
            break;
        case MOYEN:
            f_name = "puzzles/02_moyen";
            break;
        case DUR:
            f_name = "puzzles/03_dur";
            break;
        case TRES_DUR:
            f_name = "puzzles/04_tres_dur";
            break;
    }
    
    f.open(f_name.c_str(), ios_base::in);
    
    if (!f) {
        cerr << "\"" << f_name << "\" : Fichier introuvable" << endl;
        return;
    }
    
    f.seekg(0, ios_base::end);
    taille = f.tellg();
    
    //Aller chercher un puzzle au hazard dans le fichier
    //LE FICHIER DOIT ETRE AU FORMAT UNIX OU MAC (CR OU LF, PAS CR+LF)
    //Chaque ligne fait 82 octets (81 pour les chiffres + 1 pour le saut de ligne)
    f.seekg((rand()%(taille/82)) * 82, ios_base::beg);
    f.read(buffer, 81);
    
    for (int i=0; i<81; i++) {
        setValeurCase(i/9, i%9, buffer[i] - '0');
        setFixeCase(i/9, i%9, buffer[i] != '0');
    }    
    
    delete [] buffer;
    f.close();
}

void Grille::genererGrille() {
    /**
        Dude 1: Dood, on est trop pas optimal.
        Dude 2: [Wilhelm scream] Nooooooo!!!!
    */
    
    int chiffres[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int translation[9] = {0, 3, 6, 1, 4, 7, 2, 5, 8};
    
    //Melange du tableau de chiffres
    for (int i=0; i<(9-1); i++) {
        int r = i + rand()%(9-i);
        
        int tmp = chiffres[i];
        chiffres[i] = chiffres[r];
        chiffres[r] = tmp;
    }
    
    //Initialisation de la grille de base
    //a partir du tableau de chiffres (9! variantes)
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            int valeur = chiffres[(i + translation[j])%9] + 1;
            
            setValeurCase(i, j, valeur);
            setFixeCase(i, j, true); //Toutes les cases sont fixe
                                     //On "de-fixera" celles que l'on va vider
        }
    }
    
    //Pseudo-randomization de la grille
    for (int i=0; i<100; i++) {
        switch (rand()%7) {
            case 0:
                inverserColonnes();
                break;
            case 1:
                inverserRangees();
                break;
            case 2:
                inverserBlocsColonnes();
                break;
            case 3:
                inverserBlocsRangees();
                break;
            case 4:
                symetrieHorizontale();
                break;
            case 5:
                symetrieVerticale();
                break;
            case 6:
                symetrieDiagonale();
                break;
        }
    }
    
    //Copie de la grille dans la solution
    memcpy(_solution, _cases, sizeof(case_s)*81);
}


/*****************************
 * Fonctions de modification *
 * de la grille de base      *
 *****************************/
void Grille::inverserColonnes() {
    int a, b, bloc, tmp;
    
    //Choix des colonnes a inverser
    bloc = rand()%3;
    a = rand()%3 + 3*bloc;
    b = rand()%3 + 3*bloc;
    
    for (int i=0; i<9; i++) {
        tmp = getValeurCase(i, a);
        setValeurCase(i, a, getValeurCase(i, b));
        setValeurCase(i, b, tmp);
    }
}

void Grille::inverserRangees() {
    int a, b, bloc, tmp;
    
    //Choix des colonnes a inverser
    bloc = rand()%3;
    a = rand()%3 + 3*bloc;
    b = rand()%3 + 3*bloc;
    
    for (int i=0; i<9; i++) {
        tmp = getValeurCase(a, i);
        setValeurCase(a, i, getValeurCase(b, i));
        setValeurCase(b, i, tmp);
    }
}

void Grille::inverserBlocsColonnes() {
    int a, b, tmp;
    
    //Choix des colonnes a inverser
    a = rand()%3;
    b = rand()%3;
    
    for (int i=0; i<9; i++) {
        for (int j=0; j<3; j++) {
            tmp = getValeurCase(i, j+a*3);
            setValeurCase(i, j+a*3, getValeurCase(i, j+b*3));
            setValeurCase(i, j+b*3, tmp);
        }
    }
}

void Grille::inverserBlocsRangees() {
    int a, b, tmp;
    
    //Choix des colonnes a inverser
    a = rand()%3;
    b = rand()%3;
    
    for (int i=0; i<9; i++) {
        for (int j=0; j<3; j++) {
            tmp = getValeurCase(j+a*3, i);
            setValeurCase(j+a*3, i, getValeurCase(j+b*3, i));
            setValeurCase(j+b*3, i, tmp);
        }
    }
}

void Grille::symetrieHorizontale() {
    
}

void Grille::symetrieVerticale() {
    
}

void Grille::symetrieDiagonale() {
    
}

/*****************************
 * Fonctions de controle des *
 * valeurs de la grille      *
 *****************************/
bool Grille::verifCase(int x, int y) {
    //Si la valeur est 0, la case est automatiquement valide
    if (getValeurCase(x, y) == 0)
        return true;
    
    //Verification de la rangee
    for (int i=0; i<9; i++) {
        if (i != y && getValeurCase(x, y) == getValeurCase(x, i)) {
            return false;
        }
    }
    
    //Verification de la colone
    for (int i=0; i<9; i++) {
        if (i != x && getValeurCase(x, y) == getValeurCase(i, y)) {
            return false;
        }
    }
    
    //Verification du bloc de 3x3
    int bloc_x = 3*(x/3); //Premiere rangee du bloc de 3
    int bloc_y = 3*(y/3); //Premiere colone du bloc de 3
    
    for (int i=bloc_x; i<bloc_x+3; i++) {
        for (int j=bloc_y; j<bloc_y+3; j++) {
            if (i != x && j != y && getValeurCase(x, y) == getValeurCase(i, j)) {
                return false;
            }
        }
    }
    
    return true;
}

int Grille::getDisponibles(int x, int y) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    int dispos = 0, valeur = 0;;
    
    if (!getFixeCase(x, y)) {
        //On garde la valeur temporairement
        valeur = getValeurCase(x, y);
        
        //On teste chaque valeur pour un conflit
        for (int i=1; i<=9; i++) {
            setValeurCase(x, y, i);

            if (verifCase(x, y)) {
                switch (i) {
                    case 1:
                        dispos |= UN;
                        break;
                    case 2:
                        dispos |= DEUX;
                        break;
                    case 3:
                        dispos |= TROIS;
                        break;
                    case 4:
                        dispos |= QUATRE;
                        break;
                    case 5:
                        dispos |= CINQ;
                        break;
                    case 6:
                        dispos |= SIX;
                        break;
                    case 7:
                        dispos |= SEPT;
                        break;
                    case 8:
                        dispos |= HUIT;
                        break;
                    case 9:
                        dispos |= NEUF;
                        break;
                }
            }
        }
        
        setValeurCase(x, y, valeur); //Remettre la valeur initiale
        
        return dispos;
    } else { //La case est fixe
        return 0;
    }
}

int Grille::getNbDisponibles(int x, int y) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1;
    }
    
    int count = 0;
    
    if (getValeurCase(x, y) == 0 && !getFixeCase(x, y)) {
        //On teste chaque valeur pour un conflit
        for (int i=1; i<=9; i++) {
            setValeurCase(x, y, i);

            if (verifCase(x, y)) {
                count++;
            }
        }
        
        setValeurCase(x, y, 0); //Remettre la valeur initiale
        
        return count;
    } else { //Il y a une valeur dans la case
        return 0;
    }
}

bool Grille::resoudreGrille() {
    while (resoudreGrillePossibilite() || resoudreGrillePosition()) {
        //Tourne tant qu'il y a des ajouts a la grille
    }
    
    if (getNbCasesVides() == 0)
        return true;
    else
        return false;
}

int Grille::resoudreGrillePossibilite(){
    int ajouts = 0; //Nombre d'ajouts faits au total
    bool ok = true; //Pour savoir si on continue la boucle
    
    while (ajouts) {
        ok = false;
        
        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                if (getNbDisponibles(i, j) == 1) {
                    ajouts++;
                    ok = true;
                    switch (getDisponibles(i, j)) {
                        case UN:
                            setValeurCase(i, j, 1);
                            break;
                        case DEUX:
                            setValeurCase(i, j, 2);
                            break;
                        case TROIS:
                            setValeurCase(i, j, 3);
                            break;
                        case QUATRE:
                            setValeurCase(i, j, 4);
                            break;
                        case CINQ:
                            setValeurCase(i, j, 5);
                            break;
                        case SIX:
                            setValeurCase(i, j, 6);
                            break;
                        case SEPT:
                            setValeurCase(i, j, 7);
                            break;
                        case HUIT:
                            setValeurCase(i, j, 8);
                            break;
                        case NEUF:
                            setValeurCase(i, j, 9);
                            break;
                    }
                }
            }
        }
    }
    
    return ajouts;
}

int Grille::resoudreGrillePosition() {
    bool remplacement = true;
    int nbRemplacements = 0;
    int dernierePos;    //Garde la derniere position ou le nombre peut aller
    int nbPosPossible;  //Le nombre de positions ou le nombre peut aller
    
    while (remplacement) {
        remplacement = false;
        
        //Verification rangee par rangee
        for (int ran=0; ran<9; ran++) {
            for (int val=1; val<=9; val++) {
                nbPosPossible = 0;
                for (int pos=0; pos<9; pos++) {
                    if (getValeurCase(ran, pos) == 0) {
                        setValeurCase(ran, pos, val);
                        if (verifCase(ran, pos)) {
                            nbPosPossible++;
                            dernierePos = pos;
                        }
                        setValeurCase(ran, pos, 0);
                    }
                }
                
                if (nbPosPossible == 1) {  //Le chiffre ne peut aller qu'a une seule place
                    //dernierePos est forcement la place en question
                    setValeurCase(ran, dernierePos, val);
                    remplacement = true;
                    nbRemplacements++;
                }
            }
        }
        
        //Verification colone par colone
        for (int col=0; col<9; col++) {
            for (int val=1; val<=9; val++) {
                nbPosPossible = 0;
                for (int pos=0; pos<9; pos++) {
                    if (getValeurCase(pos, col) == 0) {
                        setValeurCase(pos, col, val);
                        if (verifCase(pos, col)) {
                            nbPosPossible++;
                            dernierePos = pos;
                        }
                        setValeurCase(pos, col, 0);
                    }
                }
                
                if (nbPosPossible == 1) {  //Le chiffre ne peut aller qu'a une seule place
                    //dernierePos est forcement la place en question
                    setValeurCase(dernierePos, col, val);
                    remplacement = true;
                    nbRemplacements++;
                }
            }
        }
        
        //Verification bloc par bloc
        for (int bloc=0; bloc<9; bloc++) {
            for (int val=1; val<=9; val++) {
                nbPosPossible = 0;
                for (int pos=0; pos<9; pos++) {
                    int x = (bloc/3)*3 + (pos/3);
                    int y = (bloc%3)*3 + (pos%3);
                    if (getValeurCase(x, y) == 0) {
                        setValeurCase(x, y, val);
                        if (verifCase(x, y)) {
                            nbPosPossible++;
                            dernierePos = pos;
                        }
                        setValeurCase(x, y, 0);
                    }
                }
                
                if (nbPosPossible == 1) {  //Le chiffre ne peut aller qu'a une seule place
                    //dernierePos est forcement la place en question
                    int x = (bloc/3)*3 + (dernierePos/3);
                    int y = (bloc%3)*3 + (dernierePos%3);
                    setValeurCase(x, y, val);
                    remplacement = true;
                    nbRemplacements++;
                }
            }
        }
    }
    
    return 0;
}

int Grille::getNbCasesVides() {
    int count = 0;
    
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            if (getValeurCase(i, j) == 0)
                count++;
    
    return count;
}

int Grille::getNbErreurs() {
    int count = 0;
    
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            if (!verifCase(i, j))
                count++;
    
    return count;
}

/*****************************
 * Fonction debug uniquement *
 *****************************/
void Grille::grilleTest(bool faisable) {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            setValeurCase(i, j, 0);
            setFixeCase(i, j, false);
        }
    }
    
    if (faisable) {
        /*
        setValeurCase(0, 7, 1);
        setValeurCase(1, 0, 4);
        setValeurCase(2, 1, 2);
        setValeurCase(3, 4, 5);
        setValeurCase(3, 6, 4);
        setValeurCase(3, 8, 7);
        setValeurCase(4, 2, 8);
        setValeurCase(4, 6, 3);
        setValeurCase(5, 2, 1);
        setValeurCase(5, 4, 9);
        setValeurCase(6, 0, 3);
        setValeurCase(6, 3, 4);
        setValeurCase(6, 6, 2);
        setValeurCase(7, 1, 5);
        setValeurCase(7, 3, 1);
        setValeurCase(8, 3, 8);
        setValeurCase(8, 5, 6);
        
        setFixeCase(0, 7, true);
        setFixeCase(1, 0, true);
        setFixeCase(2, 1, true);
        setFixeCase(3, 4, true);
        setFixeCase(3, 6, true);
        setFixeCase(3, 8, true);
        setFixeCase(4, 2, true);
        setFixeCase(4, 6, true);
        setFixeCase(5, 2, true);
        setFixeCase(5, 4, true);
        setFixeCase(6, 0, true);
        setFixeCase(6, 3, true);
        setFixeCase(6, 6, true);
        setFixeCase(7, 1, true);
        setFixeCase(7, 3, true);
        setFixeCase(8, 3, true);
        setFixeCase(8, 5, true);
        */
        
        setValeurCase(0, 6, 8);
        setValeurCase(0, 7, 4);
        setValeurCase(1, 3, 9);
        setValeurCase(1, 4, 7);
        setValeurCase(1, 6, 2);
        setValeurCase(2, 0, 8);
        setValeurCase(2, 1, 7);
        setValeurCase(2, 4, 3);
        setValeurCase(3, 0, 7);
        setValeurCase(3, 1, 1);
        setValeurCase(4, 1, 6);
        setValeurCase(4, 2, 8);
        setValeurCase(4, 3, 5);
        setValeurCase(4, 5, 2);
        setValeurCase(4, 6, 7);
        setValeurCase(5, 7, 6);
        setValeurCase(5, 8, 1);
        setValeurCase(6, 4, 6);
        setValeurCase(6, 7, 5);
        setValeurCase(6, 8, 8);
        setValeurCase(7, 2, 5);
        setValeurCase(7, 4, 9);
        setValeurCase(7, 5, 3);
        setValeurCase(8, 1, 9);
        setValeurCase(8, 2, 1);
        
        setFixeCase(0, 6, true);
        setFixeCase(0, 7, true);
        setFixeCase(1, 3, true);
        setFixeCase(1, 4, true);
        setFixeCase(1, 6, true);
        setFixeCase(2, 0, true);
        setFixeCase(2, 1, true);
        setFixeCase(2, 4, true);
        setFixeCase(3, 0, true);
        setFixeCase(3, 1, true);
        setFixeCase(4, 1, true);
        setFixeCase(4, 2, true);
        setFixeCase(4, 3, true);
        setFixeCase(4, 5, true);
        setFixeCase(4, 6, true);
        setFixeCase(5, 7, true);
        setFixeCase(5, 8, true);
        setFixeCase(6, 4, true);
        setFixeCase(6, 7, true);
        setFixeCase(6, 8, true);
        setFixeCase(7, 2, true);
        setFixeCase(7, 4, true);
        setFixeCase(7, 5, true);
        setFixeCase(8, 1, true);
        setFixeCase(8, 2, true);
    } else {
        setValeurCase(0, 2, 2);
        setValeurCase(0, 7, 8);
        setValeurCase(1, 3, 7);
        setValeurCase(1, 4, 3);
        setValeurCase(2, 8, 6);
        setValeurCase(3, 0, 9);
        setValeurCase(3, 3, 4);
        setValeurCase(3, 7, 2);
        setValeurCase(4, 7, 4);
        setValeurCase(5, 0, 7);
        setValeurCase(5, 3, 5);
        setValeurCase(6, 0, 5);
        setValeurCase(6, 4, 2);
        setValeurCase(6, 8, 1);
        setValeurCase(7, 2, 4);
        setValeurCase(7, 5, 8);
        setValeurCase(7, 6, 3);
        setValeurCase(8, 1, 6);
        setValeurCase(8, 6, 7);
        
        setFixeCase(0, 2, true);
        setFixeCase(0, 7, true);
        setFixeCase(1, 3, true);
        setFixeCase(1, 4, true);
        setFixeCase(2, 8, true);
        setFixeCase(3, 0, true);
        setFixeCase(3, 3, true);
        setFixeCase(3, 7, true);
        setFixeCase(4, 7, true);
        setFixeCase(5, 0, true);
        setFixeCase(5, 3, true);
        setFixeCase(6, 0, true);
        setFixeCase(6, 4, true);
        setFixeCase(6, 8, true);
        setFixeCase(7, 2, true);
        setFixeCase(7, 5, true);
        setFixeCase(7, 6, true);
        setFixeCase(8, 1, true);
        setFixeCase(8, 6, true);
    }
}
