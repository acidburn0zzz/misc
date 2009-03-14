#ifndef __USERSMODEL_H__
#define __USERSMODEL_H__

#include "defaultsqlmodel.h"

class UsersModel : public DefaultSqlModel {
    Q_OBJECT
    
    enum cols {ID, USERNAME, PASSWORD, TYPE};

    public:
        UsersModel(QObject *parent = 0);

        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
};

#endif //__USERSMODEL_H__
