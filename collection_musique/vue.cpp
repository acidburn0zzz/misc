#include <QtGui>
#include <QtSql>

#include "customsqlmodel.h"
#include "connexion.h"
#include "vue.h"

Vue::Vue(Controleur *cont, QWidget *parent) : QMainWindow(parent) {
    init();
}

Vue::~Vue() {
    delete _btnList;
    delete _btnNbAlbums;
    delete _btnInfos;
    delete _layBoutons;
    delete _tabView;
    delete _model;
    delete _centralLayout;
    delete _centralWidget;
}

void Vue::init() {
    _centralWidget = new QWidget();
    this->setCentralWidget(_centralWidget);
    this->setWindowTitle(tr("Collection de musique"));
    
    _centralLayout = new QVBoxLayout();
    
    //Ajout des boutons
    _btnList = new QPushButton(tr("Liste"));
    _btnNbAlbums = new QPushButton(tr("Nb. Albums"));
    _btnInfos = new QPushButton(tr("Infos"));
    connect(_btnList, SIGNAL(clicked()), this, SLOT(showList()));
    connect(_btnNbAlbums, SIGNAL(clicked()), this, SLOT(showNbAlbums()));
    connect(_btnInfos, SIGNAL(clicked()), this, SLOT(showInfos()));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnList);
    _layBoutons->addWidget(_btnNbAlbums);
    _layBoutons->addWidget(_btnInfos);
    
    _centralLayout->addLayout(_layBoutons);
    
    //Connexion a la base
    openSqlConnection();
    
    //Modèle initial
    _model = new CustomSqlModel();
    showList();
    
    //Setup de la table (fixe)
    _tabView = new QTableView();
    _tabView->setModel(_model);
    _tabView->resizeColumnsToContents();
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

