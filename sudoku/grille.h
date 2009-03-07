#ifndef __GRILLE_H__
#define __GRILLE_H__

enum chiffres {
    UN      = 0x0001,
    DEUX    = 0x0002,
    TROIS   = 0x0004,
    QUATRE  = 0x0008,
    CINQ    = 0x0010,
    SIX     = 0x0020,
    SEPT    = 0x0040,
    HUIT    = 0x0080,
    NEUF    = 0x0100
};

enum difficulte {
    FACILE, MOYEN, DUR, TRES_DUR
};

struct case_s {
    int valeur;     //Valeur de la case, 0 si vide
    bool fixe;      //Vrai si la case est fixe (definie par la grille)
    int dispos;     //Chiffres qui peuvent entrer dans la case
    int nbDispos; //Nombre de chiffre qui peuvent entrer dans la case
};

class Grille {
    private:
    case_s _cases[9][9];    //La grille principale
    case_s _solution[9][9]; //La grille complete

    public:
    Grille(int difficulte = FACILE);
    ~Grille();

    /**
    * Aller chercher le chiffre d'une case.
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns La valeur de la case, 0 si vide, -1 si erreur
    */
    int getValeurCase(int x, int y);

    /**
    * Assigner un chiffre a une case.
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns La valeur de la case, 0 si vide, -1 si erreur
    */
    int setValeurCase(int x, int y, int valeur);

    /**
    * Savoir si une case est fixe
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns 1 si la case est fixe, 0 si non-fixe, -1 si erreur
    */
    int getFixeCase(int x, int y);

    /**
    * Rendre une case fixe ou non
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns -1 si erreur, 0 sinon
    */
    int setFixeCase(int x, int y, bool fixe);

    /**
    * Aller chercher le chiffre d'une case dans la grille de solution.
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns La valeur de la case, 0 si vide, -1 si erreur
    */
    int getValeurSolution(int x, int y);

    /**
    * Va chercher une grille dans un des 4 fichiers
    *
    * @param difficulte - La difficulte souhaitee
    */
    void chargerGrille(int difficulte);
    
    /**
    * Genere une nouvelle grille avec un pseudo hasard
    */
    void genererGrille();

    /**
    * Inverse deux colonnes entre elles
    */
    void inverserColonnes();

    /**
    * Inverse deux rangees entre elles
    */
    void inverserRangees();

    /**
    * Inverse deux blocs de colonnes entre eux
    */
    void inverserBlocsColonnes();

    /**
    * Inverse deux blocs de rangees entre eux
    */
    void inverserBlocsRangees();
    
    /**
    * Inverse la grille selon un axe horizontal
    */
    void symetrieHorizontale();
    
    /**
    * Inverse la grille selon un axe vertical
    */
    void symetrieVerticale();
    
    /**
    * Inverse la grille selon un axe diagonal
    */
    void symetrieDiagonale();

    /**
    * Verifier si une case n'entre pas en conflit avec les autres
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns True si la case est valide, False si il y a conflit
    */
    bool verifCase(int x, int y);

    /**
    * Chiffres qui pourraient entrer dans une case
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns Un nombre sur 9 bits, chaque bit vaut un chiffre, -1 si erreur.
    */
    int getDisponibles(int x, int y);

    /**
    * Nombre de chiffres qui peuvent entrer dans une case
    *
    * @param x - Position de la case en x.
    * @param y - Position de la case en y.
    *
    * @returns Nbre de chiffres, -1 si erreur.
    */
    int getNbDisponibles(int x, int y);
    
    /**
    * Verifie si il y a des cases vides
    *
    * @returns Nbre de cases vides.
    */
    int getNbCasesVides();
    
    /**
    * Verifie si il y a des erreurs dans la grille
    *
    * @returns Nbre d'erreurs.
    */
    int getNbErreurs();

    /**
    * Tente de resoudre la grille par 
    * 1. Possibilite unique -> Un seul chiffre peut aller dans une case
    * 2. Position unique -> Un chiffre peut aller dans une seule case de la colone, rangee ou bloc.
    *
    * @returns True si la grille a pu etre resolue.
    */
    bool resoudreGrille();

    /**
    * Remplit le plus de cases possible par possibilite unique
    *
    * @returns Le nombre de chiffres trouves.
    */
    int resoudreGrillePossibilite();

    /**
    * Remplit le plus de cases possible par position unique
    *
    * @returns Le nombre de chiffres trouves.
    */
    int resoudreGrillePosition();
    
    //debug
    void grilleTest(bool faisable = true);
};

#endif //__GRILLE_H__
