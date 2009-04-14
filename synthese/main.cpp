#include <QApplication>
#include <QtSql>

#include "mainwindow.h"

#include "vuegestionatelier.h"

int main(int argc, char *argv[]) {
    int ret;
    QSqlDatabase db;
    QApplication app(argc, argv);
    
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("expo.db");
    
    if (!db.open()) {
        qDebug() << "Impossible d'ouvrir la base de donnees";
        return 1;
    }
    
    MainWindow *w = new MainWindow();
    w->setVisible(true);
#ifndef DEBUG
    w->setWindowState(Qt::WindowMaximized);
#endif
    
    ret = app.exec();
    
    db.close();

    return ret;
}
