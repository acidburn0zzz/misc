#include <iostream>
#include <sstream>

#include <QAction>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QTranslator>
#include <QVBoxLayout>

#include "boutondispo.h"
#include "casegrille.h"
#include "defines.h"
#include "grille.h"
#include "vue.h"

Vue::Vue(QString lang, QWidget *parent) : QMainWindow(parent) {
    affDispos = false;
    affErr = false;
    affTemps = true;
    langue = lang;
    
    //A deplacer dans nouvelle partie
    _grille = new Grille();
    temps = 0;
    //_grille->resoudreGrille();
    
    qtInit();
    afficherGrille();
    majGrille(); //Pour appliquer les stylesheets
    majDispos(0, 0); //Pour avoir les disponibles des le debut
}

Vue::~Vue() {
    delete _grille;
    
    delete translator;
    delete timer;
    
    delete [] cases;
    delete gridGrille;
    
    delete [] btnDispos;
    delete tbDispos;
    
    delete mnuFichier;
    delete mnuOptions;
    delete mnuAide;
    
    delete actNouveau;
    delete actQuitter;
    delete actAfficherDispos;
    delete actAfficherErreurs;
    delete actAfficherTemps;
    delete actPause;
    delete actAPropos;
    delete actAProposQt;
    
    delete centralLayout;
    
    delete centralWidget;
}

void Vue::qtInit() {
    if (langue.length() > 0) {
        translator = new QTranslator();
        translator->load("sudoku_" + langue);
        qApp->installTranslator(translator);
    }
    
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle(__APP_NAME__);
    setWindowIcon(QIcon("images/icon.svg"));
    setWindowFlags(Qt::WindowContextHelpButtonHint | windowFlags());
    //centralLayout = new QHBoxLayout();
    
    timer = new QTimer(this);
    timer->setInterval(1000); //1 seconde
    connect(timer, SIGNAL(timeout()), this, SLOT(majTemps()));
    
    creerActions();
    creerMenus();
    creerBarreOutils();
    creerStatusBar();
    
    tailleCase = 35;
    styleBase = "font-family: \"Bitstream Vera Sans\", Verdana, Arial; font-size: 16px;";
    styleBase += "padding-left: 7px;"; //Pour un pseudo "centrage"
    
    //Partie grille
    gridGrille = new QGridLayout();
    cases = new CaseGrille[81];

    for (int i=0; i<81; i++) {
        cases[i].setPosX(i/9);
        cases[i].setPosY(i%9);
        cases[i].setTaille(tailleCase);
        gridGrille->addWidget(&cases[i], i/9, i%9);
        connect(&cases[i], SIGNAL(textEdited(QString)), this, SLOT(majGrille()));
        connect(&cases[i], SIGNAL(recuFocus(int, int)), this, SLOT(majDispos(int, int)));
    }
    
    //centralLayout->addLayout(gridGrille);
    
    centralWidget->setLayout(gridGrille);
    timer->start();
}

void Vue::creerActions() {
    actNouveau = new QAction(tr("&Nouveau"), this);
    actNouveau->setShortcut(tr("Ctrl+N"));
    actNouveau->setStatusTip(tr("Nouvelle partie"));
    //connect(actNouveau, SIGNAL(triggered()), this, SLOT(/*Nouvelle partie*/));
    
    actQuitter = new QAction(tr("&Quitter"), this);
    actQuitter->setShortcut(tr("Ctrl+Q"));
    actQuitter->setStatusTip(tr("Quitter le sudoku"));
    connect(actQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    actAfficherDispos = new QAction(tr("Afficher les chiffres &disponibles"), this);
    actAfficherDispos->setCheckable(true);
    actAfficherDispos->setChecked(affDispos);
    actAfficherDispos->setShortcut(tr("Ctrl+D"));
    actAfficherDispos->setStatusTip(tr("Afficher les chiffres disponibles"));
    actAfficherDispos->setIcon(QIcon("images/dispos.svg"));
    connect(actAfficherDispos, SIGNAL(triggered()), this, SLOT(toggleDispos()));
    
    actAfficherErreurs = new QAction(tr("Afficher les &erreurs"), this);
    actAfficherErreurs->setCheckable(true);
    actAfficherErreurs->setChecked(affErr);
    actAfficherErreurs->setShortcut(tr("Ctrl+E"));
    actAfficherErreurs->setStatusTip(tr("Afficher les erreurs"));
    actAfficherErreurs->setIcon(QIcon("images/erreurs.svg"));
    connect(actAfficherErreurs, SIGNAL(triggered()), this, SLOT(toggleErreurs()));
    
    actAfficherTemps = new QAction(tr("Afficher le &chronometre"), this);
    actAfficherTemps->setCheckable(true);
    actAfficherTemps->setChecked(affTemps);
    actAfficherTemps->setShortcut(tr("Ctrl+T"));
    actAfficherTemps->setStatusTip(tr("Afficher le chronometre"));
    actAfficherTemps->setIcon(QIcon("images/temps.svg"));
    connect(actAfficherTemps, SIGNAL(triggered()), this, SLOT(toggleTimer()));
    
    actPause = new QAction(tr("Pause"), this);
    actPause->setShortcut(tr("Ctrl+P"));
    actPause->setIcon(QIcon("images/stop.svg"));
    connect(actPause, SIGNAL(triggered()), this, SLOT(pause()));
    
    actAPropos = new QAction(tr("A propos de", "Le nom du programme sera ajoute automatiquement") + " " + __APP_NAME__, this);
    //connect(actAPropos, SIGNAL(triggered()), this, SLOT(apropos()));
    
    actAProposQt = new QAction(tr("A propos de QT"), this);
    connect(actAProposQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    
    btnDispos = new BoutonDispo[10];
    
    //Partie disponibles
    for (int i=0; i<10; i++) {
        if (i == 0)
            btnDispos[i].setText("X");
        else
            btnDispos[i].setText(QString((char)i+'0'));
        btnDispos[i].setValeur(i);
        connect(&btnDispos[i], SIGNAL(clique(int)), this, SLOT(entrerChiffre(int)));
    }
}

void Vue::creerMenus() {
    mnuFichier = new QMenu(tr("&Fichier"), this);
    mnuFichier->addAction(actNouveau);
    mnuFichier->addAction(actQuitter);
    
    mnuOptions = new QMenu(tr("&Options"), this);
    mnuOptions->addAction(actAfficherDispos);
    mnuOptions->addAction(actAfficherErreurs);
    mnuOptions->addAction(actAfficherTemps);
    
    mnuAide = new QMenu(tr("&Aide"), this);
    mnuAide->addAction(actAPropos);
    mnuAide->addAction(actAProposQt);
    
    //Ajout des menus a la barre
    menuBar()->addMenu(mnuFichier);
    menuBar()->addMenu(mnuOptions);
    menuBar()->addMenu(mnuAide);
}

void Vue::creerBarreOutils() {
    tbOptions = new QToolBar(this);
    tbOptions->setMovable(false);
    
    tbOptions->addAction(actAfficherDispos);
    tbOptions->addAction(actAfficherErreurs);
    tbOptions->addAction(actAfficherTemps);
    tbOptions->addAction(actPause);
    
    tbDispos = new QToolBar(this);
    tbDispos->setVisible(affDispos);
    
    for (int i=0; i<10; i++) {
        tbDispos->addAction(&btnDispos[i]);
    }
    
    addToolBar(Qt::TopToolBarArea, tbOptions);
    addToolBarBreak();
    addToolBar(Qt::TopToolBarArea, tbDispos);
}

void Vue::creerStatusBar() {
    std::stringstream ss;
    ss << __APP_NAME__ << " " << __SUDOKU_VERSION__;
    statusBar()->showMessage(ss.str().c_str());
}

void Vue::toggleErreurs() {
    affErr = actAfficherErreurs->isChecked();
    
    //Pour ne pas declencher le message
    //si la partie est deja finie
    if (!_grille->getNbCasesVides() == 0)
        majGrille();
}

void Vue::toggleDispos() {
    //TODO Ajuster la taille de la fenetre
    affDispos = actAfficherDispos->isChecked();
    tbDispos->setVisible(affDispos);
}

void Vue::toggleTimer() {
    affTemps = actAfficherTemps->isChecked();
    majStatusBar();
}

void Vue::pause() {
    if (timer->isActive()) {
        timer->stop();
        //On cache tout
        for (int i=0; i<81; i++) {
            cases[i].setEnabled(false);
            cases[i].setText("");
        }
        
        for (int i=0; i<10; i++)
            btnDispos[i].setEnabled(false);
    } else {
        timer->start();
        for (int i=0; i<81; i++)
            cases[i].setEnabled(true);
        for (int i=0; i<9; i++)
            btnDispos[i].setEnabled(true);
        afficherGrille();
        
        //On remet le curseur dans la premiere case libre
        for (int i=0; i<81; i++) {
            if (cases[i].text().length() == 0) {
                cases[i].setFocus(); //Va appeler majDispos
                break;
            }
        }
    }
}

void Vue::majTemps() {
    temps++;
    majStatusBar();
}

void Vue::majGrille() { //TODO: Optimisation de la fonction
    int tmp;
    for (int i=0; i<81; i++) {
        //S'assurer qu'un chiffre est entre
        tmp = cases[i].text().toInt();
        
        if(tmp < 1 || tmp > 9) { //Effacer le caractere si ce n'est pas un chiffre
            cases[i].setText("");
        }
        
        //Mettre la grille a jour
        _grille->setValeurCase(cases[i].getPosX(), cases[i].getPosY(), tmp);
    }
    
    //Application d'un stylesheet
    //Rouge pour les erreurs
    //Gras pour les cases fixes
    QString str("");

    for (int i=0; i<81; i++) {
        //Le style de base
        str += styleBase;
        
        if (_grille->getFixeCase(cases[i].getPosX(), cases[i].getPosY())) { //Les cases fixes en vert
            str += "color: #008000;"; 
        } else if (!_grille->verifCase(cases[i].getPosX(), cases[i].getPosY())) { //Les erreurs visibles en rouge
            str += "color: #800000;";
        } else {
            str += "color: #000000;";
        }
        
        //Separer les blocs de 3x3
        if ((cases[i].getPosX()/3 + cases[i].getPosY()/3)%2)
            str += "background-color: #ffffff;";
        else
            str += "background-color: #d8d8d8;";
    
        //Afficher les erreurs non visibles (en comparant
        //a la grille solution) en rouge
        //TODO
        
        cases[i].setStyleSheet(str);
    }
    
    //On verifie si la grille est complete et sans erreur
    if (_grille->getNbCasesVides() == 0 && _grille->getNbErreurs() == 0) {
        QMessageBox::information(this, tr("Bravo"), tr("Vous avez complete la grille"));
        
        //On arrete le timer
        timer->stop();
        
        //On bloque toutes les cases
        for (int i=0; i<81; i++) {
            cases[i].setReadOnly(true);
        }
    }
    
    majStatusBar();
}

void Vue::majDispos(int x, int y) {
    //On en profite pour ajuster caseX / caseY
    caseX = x;
    caseY = y;
    
    int dispos = _grille->getDisponibles(x, y);
    QString styleGris = styleBase + "color: grey;";
    
    btnDispos[0].setEnabled(!_grille->getFixeCase(x, y));
    
    btnDispos[1].setEnabled(dispos & UN);
    btnDispos[2].setEnabled(dispos & DEUX);
    btnDispos[3].setEnabled(dispos & TROIS);
    btnDispos[4].setEnabled(dispos & QUATRE);
    btnDispos[5].setEnabled(dispos & CINQ);
    btnDispos[6].setEnabled(dispos & SIX);
    btnDispos[7].setEnabled(dispos & SEPT);
    btnDispos[8].setEnabled(dispos & HUIT);
    btnDispos[9].setEnabled(dispos & NEUF);
}

void Vue::majStatusBar() {
    int min, sec;
    std::stringstream ss;
    
    ss << tr("Cases restantes : ").toStdString();
    ss << _grille->getNbCasesVides();
    
    if (affTemps) {
        min = temps / 60;
        sec = temps % 60;
        
        ss << tr("\tTemps : ").toStdString();
        if (min < 10)
            ss << "0";
        ss << min << ":";
        if (sec < 10)
            ss << "0";
        ss << sec;
    }
    
    statusBar()->showMessage(ss.str().c_str());
}

void Vue::entrerChiffre(int i) {
    std::stringstream ss;
    ss << i;
    cases[caseX*9 + caseY].setText(QString(ss.str().c_str()));
    
    majGrille(); //Avec setText, le signal textEdited n'est pas envoye
}

void Vue::afficherGrille() {
    int tmp;
    for (int i=0; i<81; i++) {
        tmp = _grille->getValeurCase(cases[i].getPosX(), cases[i].getPosY());
        if (tmp > 0) {
            std::stringstream ss;
            ss << tmp;
            cases[i].setText(ss.str().c_str());
        }
        
        if (_grille->getFixeCase(cases[i].getPosX(), cases[i].getPosY())) {
            cases[i].setReadOnly(true); //Empeche l'edition des cases du puzzle
        } else {
            cases[i].setReadOnly(false);
        }
    }
}
