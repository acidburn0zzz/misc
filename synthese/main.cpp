#include <QApplication>
#include <QtSql>

#include "mainwindow.h"

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
    w->setWindowState(Qt::WindowMaximized);
    
    ret = app.exec();
    
    db.close();

    return ret;
}
