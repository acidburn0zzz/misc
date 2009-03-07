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
    
    //~ QString locale = QLocale::system().name().section('_', 0, 0);
    //~ tr.load(QString("chrono_editor_") + locale);
    //~ app.installTranslator(&tr);
    
    if (argc == 2) {
        if (strcmp(argv[1], "-e") == 0) {
            tr.load("chrono_editor_en");
            app.installTranslator(&tr);
            new Vue();
        } else {
            new Vue(argv[1]);
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-e") == 0) {
            tr.load("chrono_editor_en");
            app.installTranslator(&tr);
        }
        new Vue(argv[2]);
    } else {
        new Vue();
    }
    
    return app.exec();
}
