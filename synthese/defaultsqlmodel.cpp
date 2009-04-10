#include <QtGui>

#include "defaultsqlmodel.h"

DefaultSqlModel::DefaultSqlModel(QObject *parent)
        : QSqlQueryModel(parent) {
}

QVariant DefaultSqlModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);
    
    if (role == Qt::BackgroundRole && index.row() == _currentRow)
        return QVariant::fromValue(QBrush(QColor(224, 224, 224)));
    if (role == Qt::BackgroundRole && index.row()%2 == 1)
        return QVariant::fromValue(QBrush(QColor(240, 240, 240)));
    if (role == Qt::ForegroundRole)
        return QVariant::fromValue(QBrush(QColor(Qt::black)));
    if (role == Qt::FontRole) {
        //~ QFont f = QSqlQueryModel::data(index, role).value<QFont>();
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
    /*Doit etre reimplementee*/
}

void DefaultSqlModel::setSelectedIndex(const QModelIndex &index) {
    _currentColumn = index.column();
    _currentRow = index.row();
}
