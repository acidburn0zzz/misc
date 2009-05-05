#include <QtGui>
#include <QtSql>

#include "modelegestionatelier.h"

ModeleGestionAtelier::ModeleGestionAtelier(QObject *parent) : DefaultSqlModel(parent) {
    _query  = "SELECT a.noatel No, ";
    _query += "a.titre Titre, ";
    _query += "strftime('%w', a.dateatel) Jour, ";
    _query += "strftime('%H', a.dateatel) Heure, ";
    _query += "l.nolocal Local, ";
    _query += "a.langue Langue, ";
    _query += "t.nomtype Type, ";
    _query += "c.nom Categorie, ";
    _query += "a.nbmaximum - (SELECT COUNT(noatel) FROM p_inscription i WHERE i.noatel = a.noatel) Disponibilite ";
    _query += "FROM p_atelier a, p_type t ON a.notype = t.notype, ";
    _query += "p_local l ON a.nolocal = l.idlocal, ";
    _query += "p_categorie c ON a.nocategorie = c.nocategorie";
    
    _tableName = "p_atelier";
    _tableId = "noatel";
    
    _listeJours << tr("Dimanche") << tr("Lundi") << tr("Mardi") << tr("Mercredi") << tr("Jeudi") << tr("Vendredi") << tr("Samedi");
    
    DefaultSqlModel::init();
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
