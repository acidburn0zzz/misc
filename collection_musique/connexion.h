#ifndef __CONNEXION_H__
#define __CONNEXION_H__

#include <QtSql>

static bool openSqlConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("music.db");
    db.open();
    return true;
}

#endif //__CONNEXION_H__
