#include <QtGui>

#include "defaultsqlmodel.h"
#include "defaulttableview.h"

DefaultTableView::DefaultTableView(QWidget *parent) : QTableView(parent) {
    this->setSortingEnabled(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, SIGNAL(pressed(const QModelIndex &)), this, SLOT(setSelectedIndex(const QModelIndex &)));
}

void DefaultTableView::setSelectedIndex(const QModelIndex & index) {
    DefaultSqlModel *model;
    model = (DefaultSqlModel*)this->model();
    model->setSelectedIndex(index);
}
