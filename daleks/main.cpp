#include <QApplication>

#include "controlleur.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    srand(time(NULL));
    new Controlleur();

    return app.exec();
}
