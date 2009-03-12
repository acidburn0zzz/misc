#include <QtGui>

#include "albummodel.h"

AlbumModel::AlbumModel(QObject *parent)
    : DefaultSqlModel(parent) {
    this->setQuery("SELECT id, artiste, album, annee, genre, pistes, time(duree, 'unixepoch') FROM collection ORDER BY artiste");
    this->setHeaderData(0, Qt::Horizontal, tr("Id"));
    this->setHeaderData(1, Qt::Horizontal, tr("Artiste"));
    this->setHeaderData(2, Qt::Horizontal, tr("Album"));
    this->setHeaderData(3, Qt::Horizontal, tr("Annee"));
    this->setHeaderData(4, Qt::Horizontal, tr("Genre"));
    this->setHeaderData(5, Qt::Horizontal, tr("Pistes"));
    this->setHeaderData(6, Qt::Horizontal, tr("Duree"));
}

void AlbumModel::sort(int column, Qt::SortOrder order) {
    QString query = "SELECT id, artiste, album, annee, genre, pistes, time(duree, 'unixepoch') FROM collection ORDER BY ";
    
    switch (column) {
        case ID:
            query += "id ";
            break;
        case ARTIST:
            query += "artiste ";
            break;
        case ALBUM:
            query += "album ";
            break;
        case YEAR:
            query += "annee ";
            break;
        case GENRE:
            query += "genre ";
            break;
        case TRACKS:
            query += "pistes ";
            break;
        case LENGTH:
            query += "duree ";
            break;
    }
    
    if (order == Qt::AscendingOrder)
        query += "ASC";
    else
        query += "DESC";
    
    this->setQuery(query);
}
