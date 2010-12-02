#include <iostream>
#include <iomanip>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "state.h"
#include "struct.h"
#include "vue.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTranslator tr;
    Vue *v;

    QString locale = QLocale::system().name().section('_', 0, 0);
    tr.load(QString("chrono_editor_") + locale);
    app.installTranslator(&tr);

    if (argc == 2) {
        v = new Vue(argv[1]);
    } else {
        v = new Vue();
    }

    v->show();

    return app.exec();
}
