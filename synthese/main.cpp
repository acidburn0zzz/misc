#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow *w = new MainWindow();
    w->setVisible(true);
    
    return app.exec();
}
