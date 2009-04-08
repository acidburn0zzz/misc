#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    init();
}

MainWindow::~MainWindow() {
    delete _btnAtelier;
    delete _btnExposant;
    delete _btnCategorie;
    delete _btnEcole;
    delete _btnAuditeurs;
    delete _lblTitre;
    delete _wdCentral;
    delete _layBoutons;
    delete _layCentral;
}

void MainWindow::init() {
    //Connexion a la base
    //~ openSqlConnection();
    
    //Initialisation de l'affichage
    this->setWindowTitle(tr("Liste des ateliers"));
    
    _btnAtelier = new QPushButton(tr("Atelier"));
    _btnExposant = new QPushButton(tr("Exposant"));
    _btnCategorie = new QPushButton(tr("Categorie"));
    _btnEcole = new QPushButton(tr("Ecole"));
    _btnAuditeurs = new QPushButton(tr("Auditeur"));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnAtelier);
    _layBoutons->addWidget(_btnExposant);
    _layBoutons->addWidget(_btnCategorie);
    _layBoutons->addWidget(_btnEcole);
    _layBoutons->addWidget(_btnAuditeurs);
    
    _lblTitre = new QLabel(tr("TITRE"));
    
    _layCentral = new QVBoxLayout();
    _layCentral->addLayout(_layBoutons);
    _layCentral->addWidget(_lblTitre);
    
    
    _wdCentral = new QWidget();
    this->setCentralWidget(_wdCentral);
    _wdCentral->setLayout(_layCentral);
    
    //Actions des boutons
    connect(_btnAtelier, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnExposant, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnCategorie, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnEcole, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnAuditeurs, SIGNAL(clicked()), this, SLOT(inDev()));
    
    //~ this->setMinimumSize(800, 600);
}

void MainWindow::inDev() {
    QMessageBox::information(this, tr("En developpement"), tr("Cette fonction n'est pas encore implementee"));
}
