#ifndef __DEFAULTSQLMODEL_H__
#define __DEFAULTSQLMODEL_H__

#include <QSqlQueryModel>

class DefaultSqlModel : public QSqlQueryModel {
    Q_OBJECT

    public:
        DefaultSqlModel(QObject *parent = 0);

        QVariant data(const QModelIndex &item, int role) const;
};

#endif //__DEFAULTSQLMODEL_H__
