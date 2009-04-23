#include <QtGui>

#include "mainwindow.h"

#include "vuegestionatelier.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
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
    //Initialisation de l'affichage
    this->setWindowTitle(tr("Liste des ateliers"));
    this->setMinimumSize(800, 600);
    
    _btnAtelier = new QPushButton(tr("&Atelier"));
    _btnExposant = new QPushButton(tr("Ex&posant"));
    _btnCategorie = new QPushButton(tr("&Categorie"));
    _btnEcole = new QPushButton(tr("Ec&ole"));
    _btnAuditeurs = new QPushButton(tr("A&uditeur"));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnAtelier);
    _layBoutons->addWidget(_btnExposant);
    _layBoutons->addWidget(_btnCategorie);
    _layBoutons->addWidget(_btnEcole);
    _layBoutons->addWidget(_btnAuditeurs);
    
    /* Reduit les boutons au maximum */
    _layBoutons->addStretch(this->width());
    
    _lblTitre = new QLabel(tr("TITRE"));
    
    VueGestionAtelier *t = new VueGestionAtelier(this);
    
    _layCentral = new QVBoxLayout();
    _layCentral->addLayout(_layBoutons);
    _layCentral->addWidget(_lblTitre);
    _layCentral->addWidget(t);
    
    _wdCentral = new QWidget();
    this->setCentralWidget(_wdCentral);
    _wdCentral->setLayout(_layCentral);
    
    //Actions des boutons
    connect(_btnAtelier, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnExposant, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnCategorie, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnEcole, SIGNAL(clicked()), this, SLOT(inDev()));
    connect(_btnAuditeurs, SIGNAL(clicked()), this, SLOT(inDev()));
}

void MainWindow::inDev() {
    QMessageBox::information(this, tr("En developpement"), tr("Cette fonction n'est pas encore implementee"));
}
