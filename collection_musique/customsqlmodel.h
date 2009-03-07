#ifndef __CUSTOMSQLMODEL_H__
#define __CUSTOMSQLMODEL_H__

#include <QSqlQueryModel>

class CustomSqlModel : public QSqlQueryModel {
    Q_OBJECT

public:
    CustomSqlModel(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;
};

#endif //__CUSTOMSQLMODEL_H__
