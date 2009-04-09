#include <QtGui>
#include <QtSql>

#include "modelegestionatelier.h"

ModeleGestionAtelier::ModeleGestionAtelier(QObject *parent) : DefaultSqlModel(parent) {
    init();
}

void ModeleGestionAtelier::init() {
    QSqlRecord rec;
    
    this->setQuery("SELECT noatel, titre, langue, duree, nolocal, dateatel, nocategorie, notype, noexposant FROM p_atelier ORDER BY noatel");
    rec = this->record();
    
    for (int i=0; i<rec.count(); i++) {
        this->setHeaderData(i, Qt::Horizontal, rec.fieldName(i));
    }
}

void ModeleGestionAtelier::sort(int column, Qt::SortOrder order) {
    //~ QString query = "SELECT id, artiste, album, annee, genre, pistes, time(duree, 'unixepoch') FROM collection ORDER BY ";
    
    //~ switch (column) {
        //~ case ID:
            //~ query += "id ";
            //~ break;
        //~ case ARTIST:
            //~ query += "artiste ";
            //~ break;
        //~ case ALBUM:
            //~ query += "album ";
            //~ break;
        //~ case YEAR:
            //~ query += "annee ";
            //~ break;
        //~ case GENRE:
            //~ query += "genre ";
            //~ break;
        //~ case TRACKS:
            //~ query += "pistes ";
            //~ break;
        //~ case LENGTH:
            //~ query += "duree ";
            //~ break;
    //~ }
    
    //~ if (order == Qt::AscendingOrder)
        //~ query += "ASC";
    //~ else
        //~ query += "DESC";
    
    //~ this->setQuery(query);
}
