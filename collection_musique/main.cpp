#include <QApplication>

#include "vue.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    new Vue(NULL);
    
    return app.exec();
}
