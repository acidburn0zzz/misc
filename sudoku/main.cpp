#include <QApplication>

#include <ctime>

#include "debug.h"
#include "grille.h"
#include "vue.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));  //Pseudo-random

    //Debug
    if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        Grille *g = new Grille();
        g->grilleTest();
        debugResoudreGrille(g);
        debugPrintGrille(g);
        debugAfficherRestants(g);
        debugCheckErreurs(g);
        delete g;
        
        return 0;
    } else if (argc == 2 && strcmp(argv[1], "-e") == 0) {
        QApplication app(argc, argv);
        Vue *vue;
        vue = new Vue("en");
        vue->show();
        return app.exec();
    } else {
    //End debug
    QApplication app(argc, argv);
    Vue *vue;
    vue = new Vue();
    vue->show();
    return app.exec();
    } //Debug
}
