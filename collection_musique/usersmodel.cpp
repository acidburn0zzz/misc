#include "usersmodel.h"

UsersModel::UsersModel(QObject *parent)
    : DefaultSqlModel(parent) {
    this->setQuery("SELECT id, username, password, account_type FROM users ORDER BY artiste");
    this->setHeaderData(0, Qt::Horizontal, tr("Id"));
    this->setHeaderData(1, Qt::Horizontal, tr("Username"));
    this->setHeaderData(2, Qt::Horizontal, tr("Password"));
    this->setHeaderData(3, Qt::Horizontal, tr("Type"));
}

void UsersModel::sort(int column, Qt::SortOrder order) {
    QString query = "SELECT id, username, password, account_type FROM users ORDER BY ";
    
    switch (column) {
        case ID:
            query += "id ";
            break;
        case USERNAME:
            query += "username ";
            break;
        case PASSWORD:
            query += "password ";
            break;
        case TYPE:
            query += "account_type ";
            break;
    }
    
    if (order == Qt::AscendingOrder)
        query += "ASC";
    else
        query += "DESC";
    
    this->setQuery(query);
}
