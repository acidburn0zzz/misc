#include <iomanip>
#include <iostream>

#include "debug.h"

using namespace std;

void debugAfficherDispos(Grille *g, bool justeUn) {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (justeUn) {
                if (g->getDisponibles(i, j) == 1)
                    cout << dec << "Ran : " << i << ", Col : " << j << " -> " << g->getNbDisponibles(i, j) << " (0x" << hex << g->getDisponibles(i, j) << ")" << endl;
            } else {
                if (g->getDisponibles(i, j) > 0)
                    cout << dec << "Ran : " << i << ", Col : " << j << " -> " << g->getNbDisponibles(i, j) << " (0x" << hex << g->getDisponibles(i, j) << ")" << endl;
            }
        }
    }
}

void debugAfficherRestants(Grille *g) {
    cout << "Cases restantes : " << g->getNbCasesVides() << endl;
}

void debugPrintGrille(Grille *g) {
    int v;
    for (int i=0; i<9; i++) {
        if (i == 3 || i == 6)
            cout << endl;
        for (int j=0; j<9; j++) {
            if (j == 3 || j == 6)
                cout << " ";
            v = g->getValeurCase(i, j);
            if (v == 0)
                cout << "- ";
            else
                cout << g->getValeurCase(i, j) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void debugCheckErreurs(Grille *g) {
    int nb=0;
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            if (!g->verifCase(i, j)) {
                nb++;
                if (nb == 1)
                    cout << "Conflit(s) :" << endl;
                cout << "\t(" << i << ", " << j << ")" << endl;
            }
}

void debugResoudreGrille(Grille *g) {
    if (g->resoudreGrille()) {
        cout << "Grille resolue :)" << endl;
    } else {
        cout << "Grille impossible :(" << endl;
    }
}

void debugTestBloc(Grille *g) {
    int i=0;
    for (int bloc=5; bloc<6; bloc++) {
        for (int pos=0; pos<9; pos++) {
            int x = (bloc/3)*3 + (pos/3);
            int y = (bloc%3)*3 + (pos%3);
            cout << "(" << x << ", " << y << ")" << endl;
            g->setValeurCase(x, y, i++);
        }
    }
}
