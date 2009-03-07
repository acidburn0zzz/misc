#include <QtGui>

#include "customsqlmodel.h"

CustomSqlModel::CustomSqlModel(QObject *parent)
    : QSqlQueryModel(parent) {
}

QVariant CustomSqlModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);
    /*if (value.isValid() && role == Qt::DisplayRole) {
        if (index.column() == 0)
            return value.toString().prepend("#");
        else if (index.column() == 2)
            return value.toString().toUpper();
    }*/
    
    if (role == Qt::BackgroundRole && index.row()%2 == 1)
        return qVariantFromValue(QBrush(QColor(224, 224, 224)));
    if (role == Qt::ForegroundRole)
        return qVariantFromValue(QBrush(QColor(Qt::black)));
    /*if (role == Qt::FontRole) {
        QFont f;
        return qVariantFromValue(f);
    }*/
    return value;
}
