#include <QtGui>

#include "defaulttableview.h"

DefaultTableView::DefaultTableView(QWidget *parent) : QTableView(parent) {
    this->setSortingEnabled(true);
}
