#ifndef __DEFAULTSQLMODEL_H__
#define __DEFAULTSQLMODEL_H__

#include <QSqlQueryModel>

class DefaultSqlModel : public QSqlQueryModel {
    Q_OBJECT

    public:
    DefaultSqlModel(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    
    virtual void init();
    void setSelectedIndex(const QModelIndex & index);
    int getSelectedColumn();
    int getSelectedRow();
    void commit();
    void rollback();
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    
    private:
    int _currentColumn;
    int _currentRow;
    
    protected:
    QString _query;
    QString _tableName;
    QString _tableId;
};

#endif //__DEFAULTSQLMODEL_H__
