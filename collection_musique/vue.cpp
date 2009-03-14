#include <QtGui>
#include <QtSql>

#include "albummodel.h"
#include "connexion.h"
#include "csvparser.h"
#include "defaulttableview.h"
#include "login.h"
#include "sqlparser.h"
#include "usersview.h"
#include "vue.h"

Vue::Vue(Controleur *cont, QWidget *parent) : QMainWindow(parent) {
    init();
}

Vue::~Vue() {
    delete _btnList;
    delete _btnNbAlbums;
    delete _btnInfos;
    delete _btnUsers;
    delete _btnDelete;
    delete _layBoutons;
    delete _tabView;
    delete _model;
    delete _centralLayout;
    delete _centralWidget;
}

void Vue::init() {
    //Connexion a la base
    openSqlConnection();
    
    #ifndef DEBUG
    Login l(this);
    if (l.exec() == 0)
        exit(-1);
    #endif
    
    _centralWidget = new QWidget();
    this->setCentralWidget(_centralWidget);
    this->setWindowTitle(tr("Collection de musique"));
    
    _centralLayout = new QVBoxLayout();
    
    //Creation des menus
    _mnuFile = new QMenu(tr("&File"), this);
    _mnuImport = new QMenu(tr("&import"), this);
    
    _actImportCSV = new QAction(tr("CSV File"), this);
    _actImportSQL = new QAction(tr("SQL File"), this);
    _actQuit = new QAction(tr("&Quit"), this);
    
    connect(_actImportCSV, SIGNAL(triggered()), this, SLOT(importCSV()));
    connect(_actImportSQL, SIGNAL(triggered()), this, SLOT(importSQL()));
    connect(_actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    menuBar()->addMenu(_mnuFile);
    _mnuFile->addMenu(_mnuImport);
    _mnuFile->addAction(_actQuit);
    _mnuImport->addAction(_actImportCSV);
    _mnuImport->addAction(_actImportSQL);
    
    //Ajout des boutons
    _btnList = new QPushButton(tr("Liste"));
    _btnNbAlbums = new QPushButton(tr("Nb. Albums"));
    _btnInfos = new QPushButton(tr("Infos"));
    _btnUsers = new QPushButton(tr("Users"));
    _btnDelete = new QPushButton(tr("Delete"));
    connect(_btnList, SIGNAL(clicked()), this, SLOT(showList()));
    connect(_btnNbAlbums, SIGNAL(clicked()), this, SLOT(showNbAlbums()));
    connect(_btnInfos, SIGNAL(clicked()), this, SLOT(showInfos()));
    connect(_btnUsers, SIGNAL(clicked()), this, SLOT(showUsers()));
    connect(_btnDelete, SIGNAL(clicked()), this, SLOT(emptyTable()));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnList);
    _layBoutons->addWidget(_btnNbAlbums);
    _layBoutons->addWidget(_btnInfos);
    _layBoutons->addWidget(_btnUsers);
    _layBoutons->addWidget(_btnDelete);
    
    _centralLayout->addLayout(_layBoutons);
    
    //Modèle initial
    _model = new AlbumModel();
    //~ showList();
    
    //Setup de la table (fixe)
    _tabView = new DefaultTableView();
    _tabView->setModel(_model);
    _tabView->resizeColumnsToContents();
    _tabView->sortByColumn(1, Qt::AscendingOrder);
    _centralLayout->addWidget(_tabView);
    
    _centralWidget->setLayout(_centralLayout);
    
    this->setMinimumSize(800, 600);
    this->show();
}

void Vue::showList() {
    _query = "SELECT id, artiste, album, annee, genre, pistes, time(duree, 'unixepoch') FROM collection ORDER BY artiste";
    _model->setQuery(_query);
    _model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Artiste"));
    _model->setHeaderData(2, Qt::Horizontal, tr("Album"));
    _model->setHeaderData(3, Qt::Horizontal, tr("Annee"));
    _model->setHeaderData(4, Qt::Horizontal, tr("Genre"));
    _model->setHeaderData(5, Qt::Horizontal, tr("Pistes"));
    _model->setHeaderData(6, Qt::Horizontal, tr("Duree"));
}

void Vue::showNbAlbums() {
    _query = "SELECT artiste, count(*), min(annee), sum(pistes), strftime('%w %H:%M:%S', sum(duree), 'unixepoch', '-4 days'), time(sum(duree) / sum(pistes), 'unixepoch') moy FROM collection GROUP BY artiste ORDER BY artiste";
    _model->setQuery(_query);
    _model->setHeaderData(0, Qt::Horizontal, tr("Artiste"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Nb. Albums"));
    _model->setHeaderData(2, Qt::Horizontal, tr("1ere Annee"));
    _model->setHeaderData(3, Qt::Horizontal, tr("Pistes"));
    _model->setHeaderData(4, Qt::Horizontal, tr("Duree"));
    _model->setHeaderData(5, Qt::Horizontal, tr("Moy Chanson"));
}

void Vue::showInfos() {
    _query = "SELECT count(distinct artiste) 'Nb Artistes', count(album) 'Nb Albums', count(distinct genre) 'Nb Genres', sum(pistes) 'Nb Pistes', strftime('%j %H:%M:%S', sum(duree), 'unixepoch', '-1 days') 'Duree tot', time(sum(duree) / sum(pistes), 'unixepoch', '-4 days') 'Moy long chanson' FROM collection";
    _model->setQuery(_query);
    _model->setHeaderData(0, Qt::Horizontal, tr("Nb Artistes"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Nb Albums"));
    _model->setHeaderData(2, Qt::Horizontal, tr("Nb Genres"));
    _model->setHeaderData(3, Qt::Horizontal, tr("Nb Pistes"));
    _model->setHeaderData(4, Qt::Horizontal, tr("Duree tot"));
    _model->setHeaderData(5, Qt::Horizontal, tr("Moy long chanson"));
}

void Vue::showUsers() {
    UsersView v;
    v.exec();
}

void Vue::emptyTable() {
    QSqlQuery q;
    q.exec("DELETE FROM collection");
    showList();
}

void Vue::importCSV() {
    CsvParser csv(this);
    csv.import();
    showList();
}

void Vue::importSQL() {
    SqlParser sql(this);
    sql.import();
    showList();
}
