#include <QtGui>
#include <QtSql>

#include "defaulttableview.h"
#include "usersmodel.h"
#include "usersview.h"
#include "sha2.h"

UsersView::UsersView(QWidget *parent) : QDialog(parent, Qt::Dialog) {
    init();
}

UsersView::~UsersView() {
    delete _btnAdd;
    delete _btnClose;
    
    delete _model;
    delete _tableView;
    
    delete _layBoutons;
    delete _centralLayout;
}

void UsersView::init() {
    this->setWindowTitle(tr("UsersView"));
    this->setMinimumSize(640, 480);
    
    //Table
    _model = new UsersModel();
    _tableView = new DefaultTableView();
    _tableView->setModel(_model);
    _tableView->resizeColumnsToContents();
    _tableView->sortByColumn(0, Qt::AscendingOrder);
    _tableView->setColumnHidden(2, true); //On cache le password
    
    //Ajout des boutons
    _btnAdd = new QPushButton(tr("&Add User"));
    _btnClose = new QPushButton(tr("&Quit"));
    connect(_btnAdd, SIGNAL(clicked()), this, SLOT(addUser()));
    connect(_btnClose, SIGNAL(clicked()), this, SLOT(accept()));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnAdd);
    _layBoutons->addWidget(_btnClose);
    
    _centralLayout = new QVBoxLayout();
    _centralLayout->addWidget(_tableView);
    _centralLayout->addLayout(_layBoutons);
    
    this->setLayout(_centralLayout);
}

void UsersView::addUser() {
    return;
}
