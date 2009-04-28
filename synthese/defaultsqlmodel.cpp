#include <QtGui>
#include <QtSql>

#include "defaultsqlmodel.h"

DefaultSqlModel::DefaultSqlModel(QObject *parent)
        : QSqlQueryModel(parent) {
    /*Valeurs par defaut au cas*/
    _sortColumn = 0;
    _sortAsc = true;
}

QVariant DefaultSqlModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);

    if (role == Qt::BackgroundRole && index.row() == _currentRow)
        return QVariant::fromValue(QBrush(QColor(192, 255, 192)));
    if (role == Qt::BackgroundRole && index.row()%2 == 1)
        return QVariant::fromValue(QBrush(QColor(224, 224, 224)));
    if (role == Qt::ForegroundRole)
        return QVariant::fromValue(QBrush(QColor(Qt::black)));
    if (role == Qt::FontRole) {
        QFont f = value.value<QFont>();
        f.setPointSize(10);
        return QVariant::fromValue(f);
    }

    return value;
}

QVariant DefaultSqlModel::headerData(int section, Qt::Orientation orientation, int role) const  {
    if (role == Qt::FontRole) {
        QFont f = QSqlQueryModel::headerData(section, orientation, role).value<QFont>();
        f.setWeight(QFont::DemiBold);
        return QVariant::fromValue(f);
    }

    return QSqlQueryModel::headerData(section, orientation, role);
}

void DefaultSqlModel::init() {
    QSqlQuery q;
    QSqlRecord rec;

    q.exec("BEGIN;");

    qDebug() << _query;
    this->setQuery(_query);
    rec = this->record();

    for (int i=0; i<rec.count(); i++) {
        this->setHeaderData(i, Qt::Horizontal, rec.fieldName(i));
    }

    /* On se positionne par defaut sur le premier enregistrement */
    _currentColumn = 0;
    _currentRow = 0;
}

void DefaultSqlModel::setSelectedIndex(const QModelIndex &index) {
    _currentColumn = index.column();
    _currentRow = index.row();
}

int DefaultSqlModel::getSelectedColumn() {
    return _currentColumn;
}

int DefaultSqlModel::getSelectedRow() {
    return _currentRow;
}

void DefaultSqlModel::commit() {
    QSqlQuery q;
    q.exec("COMMIT");
}

void DefaultSqlModel::rollback() {
    QSqlQuery q;
    q.exec("ROLLBACK");
}

void DefaultSqlModel::sort() {
    if (_sortAsc)
        sort(_sortColumn, Qt::AscendingOrder);
    else
        sort(_sortColumn, Qt::DescendingOrder);
}

void DefaultSqlModel::sort(int column, Qt::SortOrder order) {
    _sortColumn = column;

    QString sQuery = _query;
    QSqlRecord rec = this->record();

    sQuery += " ORDER BY " + rec.fieldName(column) + " ";

    if (order == Qt::AscendingOrder) {
        sQuery += "ASC";
        _sortAsc = true;
    } else {
        sQuery += "DESC";
        _sortAsc = false;
    }

    this->setQuery(sQuery);
}

bool DefaultSqlModel::removeRows(int row, int count, const QModelIndex & parent) {
    bool success;
    QString sQuery;
    QSqlQuery q;

    beginRemoveRows(parent, row, row + (count - 1));

    sQuery  = "DELETE FROM " + _tableName + " WHERE " + _tableId + "=";
    sQuery += index(row, 0, parent).data().toString();

    success = q.exec(sQuery);

    endRemoveRows();
    return success;
}
