#include <QtGui>
#include <QtSql>

#include "modelegestionatelier.h"

ModeleGestionAtelier::ModeleGestionAtelier(QObject *parent) : DefaultSqlModel(parent) {
    init();
}

QVariant ModeleGestionAtelier::data(const QModelIndex &index, int role) const {
    QVariant value = DefaultSqlModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole) {
        if (index.column() == JOUR)
            return QVariant::fromValue(_listeJours[value.toInt()]);
    }
    if (value.isValid() && role == Qt::ForegroundRole) {
        if (index.column() == NOATEL && record(index.row()).value(DISPO) == 0)
            return QVariant::fromValue(QBrush(QColor(Qt::red)));
        if (index.column() == DISPO && record(index.row()).value(DISPO) == 0)
            return QVariant::fromValue(QBrush(QColor(Qt::red)));
    }
    
    return value;
}

void ModeleGestionAtelier::init() {
    QSqlQuery q;
    QSqlRecord rec;
    QString sQuery;
    
    q.exec("BEGIN;");
    
    sQuery  = "SELECT a.noatel No, ";
    sQuery += "a.titre Titre, ";
    sQuery += "strftime('%w', a.dateatel) Jour, ";
    sQuery += "strftime('%H', a.dateatel) Heure, ";
    sQuery += "l.nolocal Local, ";
    sQuery += "a.langue Langue, ";
    sQuery += "t.nomtype Type, ";
    sQuery += "c.nom Categorie, ";
    sQuery += "a.nbmaximum - (SELECT COUNT(noatel) FROM p_inscription i WHERE i.noatel = a.noatel) Disponibilite ";
    sQuery += "FROM p_atelier a, p_type t ON a.notype = t.notype, ";
    sQuery += "p_local l ON a.nolocal = l.idlocal, ";
    sQuery += "p_categorie c ON a.nocategorie = c.nocategorie ORDER BY a.noatel";
    
    this->setQuery(sQuery);
    rec = this->record();
    
    for (int i=0; i<rec.count(); i++) {
        this->setHeaderData(i, Qt::Horizontal, rec.fieldName(i));
    }
    
    _listeJours << tr("Dimanche") << tr("Lundi") << tr("Mardi") << tr("Mercredi") << tr("Jeudi") << tr("Vendredi") << tr("Samedi");
}

void ModeleGestionAtelier::sort(int column, Qt::SortOrder order) {
    QString sQuery;
    QSqlRecord rec = this->record();
    
    sQuery  = "SELECT a.noatel No, ";
    sQuery += "a.titre Titre, ";
    sQuery += "strftime('%w', a.dateatel) Jour, ";
    sQuery += "strftime('%H', a.dateatel) Heure, ";
    sQuery += "a.nolocal Local, ";
    sQuery += "a.langue Langue, ";
    sQuery += "t.nomtype Type, ";
    sQuery += "c.nom Categorie, ";
    sQuery += "a.nbmaximum - (SELECT COUNT(noatel) FROM p_inscription i WHERE i.noatel = a.noatel) Disponibilite ";
    sQuery += "FROM p_atelier a, p_type t ON a.notype = t.notype, ";
    sQuery += "p_categorie c ON a.nocategorie = c.nocategorie ORDER BY ";
    
    sQuery += rec.fieldName(column) + " ";
    
    if (order == Qt::AscendingOrder)
        sQuery += "ASC";
    else
        sQuery += "DESC";
    
    this->setQuery(sQuery);
}

bool ModeleGestionAtelier::removeRows(int row, int count, const QModelIndex & parent) {
    bool success;
    QString sQuery;
    QSqlQuery q;
    
    beginRemoveRows(parent, row, row + (count - 1));
    
    sQuery  = "DELETE FROM p_atelier WHERE noatel=";
    sQuery += index(row, 0, parent).data().toString();
    
    qDebug() << sQuery;
    success = q.exec(sQuery);
    qDebug() << success;
    
    endRemoveRows();
    return success;
}
