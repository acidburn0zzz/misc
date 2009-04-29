#include <QtSql>

#include "modeleajoutupdate.h"

ModeleAjoutUpdate::ModeleAjoutUpdate() {
}

void ModeleAjoutUpdate::init() {
    /* On va chercher le no du prochain atelier */
    if (!_query.exec("SELECT seq FROM sqlite_sequence WHERE name='" + _tableName + "'")) {
        _id = 1;
    } else {
        _query.first();
        _id = _query.value(0).toInt() + 1;
    }

    //On definit la taille du vecteur pour pouvoir
    //y inserer des donnees
    _values.resize(_columnNames.size());
}

void ModeleAjoutUpdate::init(int id) {
    _id = id;

    /* On remplit les infos */
    QString sQuery;
    sQuery = "SELECT ";
    for (unsigned int i=0; i<_columnNames.size(); i++) {
        sQuery += _columnNames[i];
        if (i != _columnNames.size() - 1)
            sQuery += ", ";
    }
    sQuery += " FROM " + _tableName;
    sQuery += " WHERE " + _tableId + "=" + QString::number(_id);

//    _query.exec(sQuery);
    if (!_query.exec(sQuery)) {
        qDebug() << sQuery;
        qDebug() << _query.lastError();
    }

    _query.first();

    for (unsigned int i=0; i<_columnNames.size(); i++)
        _values.push_back(_query.value(i));
}

void ModeleAjoutUpdate::fillListes() {
    //Doit etre reimplementee
    //Servira a creer les listes uniques a chaque table
}

bool ModeleAjoutUpdate::addEntry() {
    QString sQuery;
    bool ret;

    sQuery  = "INSERT INTO " + _tableName + " (";
    for (unsigned int i=0; i<_columnNames.size(); i++) {
        sQuery += _columnNames[i];
        if (i != _columnNames.size() - 1)
            sQuery += ", ";
        else
            sQuery += " ";
    }
    sQuery += ") VALUES (";
    for (unsigned int i=0; i<_values.size(); i++) {
        if (_columnTypes[i] == "int")
            sQuery += _values[i].toString();
        else if (_columnTypes[i] == "str")
            sQuery += "'" + _values[i].toString() + "'";

        if (i != _columnNames.size() - 1)
            sQuery += ", ";
    }
    sQuery += ")";

    ret = _query.exec(sQuery);
    if (!ret)
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << _query.lastError().text();

    return ret;
}

bool ModeleAjoutUpdate::updateEntry() {
    QString sQuery;
    bool ret;

    sQuery  = "UPDATE " + _tableName + " SET ";
    for (unsigned int i=0; i<_values.size(); i++) {
        sQuery += _columnNames[i] + "=";
        if (_columnTypes[i] == "int")
            sQuery += _values[i].toString();
        else if (_columnTypes[i] == "str")
            sQuery += "'" + _values[i].toString() + "'";

        if (i != _columnNames.size() - 1)
            sQuery += ", ";
    }
    sQuery += " WHERE " + _tableId + "=" + QString::number(_id);

    ret = _query.exec(sQuery);
    if (!ret)
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << _query.lastError().text();

    return ret;
}

int ModeleAjoutUpdate::getId() {
    return _id;
}

QString ModeleAjoutUpdate::getType(int index) {
    return _columnTypes[index];
}

QVariant ModeleAjoutUpdate::getValue(int index) {
    return _values[index];
}

void ModeleAjoutUpdate::setValue(QVariant value, int index) {
    _values[index] = value;
}
