#ifndef __CONNEXION_H__
#define __CONNEXION_H__

#include <QtGui>
#include <QtSql>

static bool openSqlConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("music.db");
    
    /* Pas d'affaire ici */
    db.open();
    QSqlQuery q;
    if (!q.exec("select * from collection")) {
        q.exec("CREATE TABLE collection (id INTEGER PRIMARY KEY, artiste TEXT, album TEXT, annee INTEGER, genre TEXT, pistes INTEGER, duree INTEGER)");
        q.exec("CREATE UNIQUE INDEX album_id ON collection (artiste, album)");
    }
    return true;
    /* Pas d'affaire ici END */
    
    //~ return db.open();
}

#endif //__CONNEXION_H__
