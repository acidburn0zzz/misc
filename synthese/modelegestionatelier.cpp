#include <QtGui>
#include <QtSql>

#include "modelegestionatelier.h"

ModeleGestionAtelier::ModeleGestionAtelier(QObject *parent) : DefaultSqlModel(parent) {
    init();
}

QVariant ModeleGestionAtelier::data(const QModelIndex &index, int role) const {
    QVariant value = DefaultSqlModel::data(index, role);
    if (role == Qt::DisplayRole) {
        if (index.column() == JOUR)
            return QVariant::fromValue(_listeJours[value.toInt()]);
    }
    
    return value;
}

void ModeleGestionAtelier::init() {
    QSqlRecord rec;
    
    this->setQuery("SELECT a.noatel, a.titre, strftime('%w', a.dateatel) jour, strftime('%H', a.dateatel) heure, a.nolocal, a.langue, t.nomtype, c.nom, a.nbmaximum FROM p_atelier a, p_type t ON a.notype = t.notype, p_categorie c ON a.nocategorie = c.nocategorie ORDER BY a.noatel");
    rec = this->record();
    
    for (int i=0; i<rec.count(); i++) {
        this->setHeaderData(i, Qt::Horizontal, rec.fieldName(i));
    }
    
    _listeJours << tr("Dimanche") << tr("Lundi") << tr("Mardi") << tr("Mercredi") << tr("Jeudi") << tr("Vendredi") << tr("Samedi");
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
