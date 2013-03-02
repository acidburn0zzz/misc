/***************************************************************************
 * Copyright (C) 2010 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/

#include <QApplication>
#include <QAction>
#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>
#include <QString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <cstring>

#include "game.h"
#include "sramfile.h"
#include "vue.h"

Vue::Vue(QWidget *parent) : QMainWindow(parent) {
    init();
}

Vue::Vue(const char *fn, QWidget *parent) : QMainWindow(parent) {
    init();
    open(QString(fn));
}

Vue::~Vue() {
    if (_sramFile) delete _sramFile;
    if (_game) delete _game;
}

void Vue::init() {
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    setWindowTitle("Chrono Trigger Save Game Editor");

    centralLayout = new QVBoxLayout();
    tabPersos = new QTabWidget(centralWidget);

    //Creation des menus
    creerMenus();

    //Initialisation des tableaux d'objets pour les stats
    wPersos = new QWidget[7];

    hlayPerso = new QHBoxLayout[7];
    gridStatsBase = new QGridLayout[7];
    gridStats = new QGridLayout[7];
    gridItems = new QGridLayout[7];

    lblImage = new QLabel[7];
    lblLevel = new QLabel[7];
    lblExp = new QLabel[7];
    lblCurrentHP = new QLabel[7];
    lblMaxHP = new QLabel[7];
    lblCurrentMP = new QLabel[7];
    lblMaxMP = new QLabel[7];
    lblPower = new QLabel[7];
    lblStamina = new QLabel[7];
    lblSpeed = new QLabel[7];
    lblMagic = new QLabel[7];
    lblHit = new QLabel[7];
    lblEvade = new QLabel[7];
    lblMagicDef = new QLabel[7];
    lblHelmet = new QLabel[7];
    lblArmor = new QLabel[7];
    lblWeapon = new QLabel[7];
    lblRelic = new QLabel[7];

    sbLevel = new QSpinBox[7];
    sbExp = new QSpinBox[7];
    sbCurrentHP = new QSpinBox[7];
    sbMaxHP = new QSpinBox[7];
    sbCurrentMP = new QSpinBox[7];
    sbMaxMP = new QSpinBox[7];
    sbPower = new QSpinBox[7];
    sbStamina = new QSpinBox[7];
    sbSpeed = new QSpinBox[7];
    sbMagic = new QSpinBox[7];
    sbHit = new QSpinBox[7];
    sbEvade = new QSpinBox[7];
    sbMagicDef = new QSpinBox[7];
    cmbHelmet = new QComboBox[7];
    cmbArmor = new QComboBox[7];
    cmbWeapon = new QComboBox[7];
    cmbRelic = new QComboBox[7];

    //Initialisation des objets
    for (int i=0; i<7; i++) {
        //Options specifiques au perso
        fillSpecificPerso(i);

        //Pourrait enlever ca pour utiliser chaque 1 fois

        QPixmap img(_imageFile);
        img = img.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding);
        lblImage[i].setPixmap(img);

        lblLevel[i].setText("Level");
        lblLevel[i].setParent(&wPersos[i]);
        lblExp[i].setText("Experience");
        lblExp[i].setParent(&wPersos[i]);
        lblCurrentHP[i].setText("Current HP");
        lblCurrentHP[i].setParent(&wPersos[i]);
        lblMaxHP[i].setText("Max HP");
        lblMaxHP[i].setParent(&wPersos[i]);
        lblCurrentMP[i].setText("Current MP");
        lblCurrentMP[i].setParent(&wPersos[i]);
        lblMaxMP[i].setText("Max MP");
        lblMaxMP[i].setParent(&wPersos[i]);
        lblPower[i].setText("Power");
        lblPower[i].setParent(&wPersos[i]);
        lblStamina[i].setText("Stamina");
        lblStamina[i].setParent(&wPersos[i]);
        lblSpeed[i].setText("Speed");
        lblSpeed[i].setParent(&wPersos[i]);
        lblMagic[i].setText("Magic");
        lblMagic[i].setParent(&wPersos[i]);
        lblHit[i].setText("Hit");
        lblHit[i].setParent(&wPersos[i]);
        lblEvade[i].setText("Evade");
        lblEvade[i].setParent(&wPersos[i]);
        lblMagicDef[i].setText("Magic Def");
        lblMagicDef[i].setParent(&wPersos[i]);
        lblHelmet[i].setText("Helmet");
        lblHelmet[i].setParent(&wPersos[i]);
        lblArmor[i].setText("Armor");
        lblArmor[i].setParent(&wPersos[i]);
        lblWeapon[i].setText("Weapon");
        lblWeapon[i].setParent(&wPersos[i]);
        lblRelic[i].setText("Relic");
        lblRelic[i].setParent(&wPersos[i]);

        sbLevel[i].setRange(0, 99);
        sbExp[i].setRange(0, 9999999);
        sbCurrentHP[i].setRange(0, 999);
        sbMaxHP[i].setRange(0, 999);
        sbCurrentMP[i].setRange(0, 99);
        sbMaxMP[i].setRange(0, 99);
        sbPower[i].setRange(0, 99);
        sbStamina[i].setRange(0, 99);
        sbSpeed[i].setRange(0, 16);
        sbMagic[i].setRange(0, 99);
        sbHit[i].setRange(0, 99);
        sbEvade[i].setRange(0, 99);
        sbMagicDef[i].setRange(0, 99);

        //Items communs a tous les persos
        for (int j=0x7c; j<=0x93; j++)
            cmbHelmet[i].addItem(itemList[j], j);
        for (int j=0x5b; j<=0x7a; j++)
            cmbArmor[i].addItem(itemList[j], j);
        for (int j=0x95; j<=0xbb; j++)
            cmbRelic[i].addItem(itemList[j], j);

        //Partie de gauche (Infos principales)
        gridStatsBase[i].addWidget(&lblImage[i], 0, 0, 4, 1, Qt::AlignVCenter);
        gridStatsBase[i].addWidget(&lblLevel[i], 0, 1, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbLevel[i], 1, 1, Qt::AlignTop);
        gridStatsBase[i].addWidget(&lblExp[i], 2, 1, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbExp[i], 3, 1, Qt::AlignTop);
        gridStatsBase[i].addWidget(&lblCurrentHP[i], 4, 0, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbCurrentHP[i], 5, 0, Qt::AlignTop);
        gridStatsBase[i].addWidget(&lblMaxHP[i], 4, 1, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbMaxHP[i], 5, 1, Qt::AlignTop);
        gridStatsBase[i].addWidget(&lblCurrentMP[i], 6, 0, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbCurrentMP[i], 7, 0, Qt::AlignTop);
        gridStatsBase[i].addWidget(&lblMaxMP[i], 6, 1, Qt::AlignBottom);
        gridStatsBase[i].addWidget(&sbMaxMP[i], 7, 1, Qt::AlignTop);
        //gridStatsBase[i].addWidget();

        //Partie du milieu (Stats)
        gridStats[i].addWidget(&lblPower[i], 0, 0, Qt::AlignBottom);
        gridStats[i].addWidget(&sbPower[i], 1, 0, Qt::AlignTop);
        gridStats[i].addWidget(&lblStamina[i], 2, 0, Qt::AlignBottom);
        gridStats[i].addWidget(&sbStamina[i], 3, 0, Qt::AlignTop);
        gridStats[i].addWidget(&lblSpeed[i], 4, 0, Qt::AlignBottom);
        gridStats[i].addWidget(&sbSpeed[i], 5, 0, Qt::AlignTop);
        gridStats[i].addWidget(&lblMagic[i], 0, 1, Qt::AlignBottom);
        gridStats[i].addWidget(&sbMagic[i], 1, 1, Qt::AlignTop);
        gridStats[i].addWidget(&lblHit[i], 2, 1, Qt::AlignBottom);
        gridStats[i].addWidget(&sbHit[i], 3, 1, Qt::AlignTop);
        gridStats[i].addWidget(&lblEvade[i], 4, 1, Qt::AlignBottom);
        gridStats[i].addWidget(&sbEvade[i], 5, 1, Qt::AlignTop);
        gridStats[i].addWidget(&lblMagicDef[i], 6, 0, Qt::AlignBottom);
        gridStats[i].addWidget(&sbMagicDef[i], 7, 0, Qt::AlignTop);

        //Partie de gauche (Items)
        gridItems[i].addWidget(&lblHelmet[i], 0, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbHelmet[i], 1, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblArmor[i], 2, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbArmor[i], 3, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblWeapon[i], 4, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbWeapon[i], 5, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblRelic[i], 6, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbRelic[i], 7, 0, Qt::AlignTop);

        //Ajouter les 3 layouts au layout du widget du perso
        hlayPerso[i].addLayout(&gridStatsBase[i]);
        hlayPerso[i].addLayout(&gridStats[i]);
        hlayPerso[i].addLayout(&gridItems[i]);

        //Ajouter le layout au widget du perso
        wPersos[i].setLayout(&hlayPerso[i]);

        //Ajouter le widget du perso au Tab
        tabPersos->addTab(&wPersos[i], _name);
    }

    //Layout principal
    centralLayout->addWidget(tabPersos, 1);

    centralWidget->setLayout(centralLayout);
    this->setMinimumSize(400, 300);
}

QString Vue::selectFile() {
    return QFileDialog::getOpenFileName(0, tr("Ouvrir un fichier"), QString(), tr("Fichier SRAM (*.srm);;Tous (*.*)"));
}

void Vue::creerMenus() {
    actOuvrir = new QAction(tr("&Ouvrir"), this);
    actOuvrir->setShortcut(tr("Ctrl+O"));
    actOuvrir->setStatusTip(tr("Ouvrir une Save State"));
    connect(actOuvrir, SIGNAL(triggered()), this, SLOT(open()));

    actSauvegarder = new QAction(tr("&Enregistrer"), this);
    actSauvegarder->setShortcut(tr("Ctrl+S"));
    actSauvegarder->setStatusTip(tr("Enregistrer la Save State"));
    connect(actSauvegarder, SIGNAL(triggered()), this, SLOT(save()));

    actQuitter = new QAction(tr("&Quitter"), this);
    actQuitter->setShortcut(tr("Ctrl+Q"));
    actQuitter->setStatusTip(tr("Quitter"));
    connect(actQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    mnuFichier = new QMenu(tr("&Fichier"));
    mnuFichier->addAction(actOuvrir);
    mnuFichier->addAction(actSauvegarder);
    mnuFichier->addAction(actQuitter);
    menuBar()->addMenu(mnuFichier);

    actAbout = new QAction(tr("À &Propos de Chrono Editor"), this);
    actAbout->setShortcut(tr("Ctrl+A"));

    actAboutQt = new QAction(tr("À Propos de &Qt"), this);
    actAboutQt->setShortcut(tr("Ctrl+T"));
    connect(actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    mnuAide = new QMenu(tr("&Aide"));
    mnuAide->addAction(actAbout);
    mnuAide->addAction(actAboutQt);
    menuBar()->addMenu(mnuAide);
}

void Vue::fillSpecificPerso(int perso) {
    switch (perso) {
        case CRONO:
            _name = "Crono";
            _imageFile = "images/crono.png";
            for (int i=0x01; i<=0x10; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            cmbWeapon[perso].addItem(itemList[0x4f], 0x4f);
            for (int i=0x53; i<=0x55; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case MARLE:
            _name = "Marle";
            _imageFile = "images/marle.png";
            for (int i=0x11; i<=0x1a; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case LUCCA:
            _name = "Lucca";
            _imageFile = "images/lucca.png";
            for (int i=0x1f; i<=0x29; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case ROBO:
            _name = "Robo";
            _imageFile = "images/robo.png";
            for (int i=0x2e; i<=0x39; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case FROG:
            _name = "Frog";
            _imageFile = "images/frog.png";
            for (int i=0x3b; i<=0x43; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            for (int i=0x50; i<=0x52; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case AYLA:
            _name = "Ayla";
            _imageFile = "images/ayla.png";
            for (int i=0x44; i<=0x48; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
        case MAGUS:
            _name = "Magus";
            _imageFile = "images/magus.png";
            for (int i=0x4b; i<=0x4e; i++)
                cmbWeapon[perso].addItem(itemList[i], i);
            break;
    }
}

void Vue::selectItems(int perso) {
    //Helmet
    for (int i=0; i<cmbHelmet[perso].count(); i++) {
        if (cmbHelmet[perso].itemData(i) == _game->getCharacter(perso).getHelmet()) {
            cmbHelmet[perso].setCurrentIndex(i);
        }
    }
    //Armor
    for (int i=0; i<cmbArmor[perso].count(); i++) {
        if (cmbArmor[perso].itemData(i) == _game->getCharacter(perso).getArmor()) {
            cmbArmor[perso].setCurrentIndex(i);
        }
    }
    //Weapon
    for (int i=0; i<cmbWeapon[perso].count(); i++) {
        if (cmbWeapon[perso].itemData(i) == _game->getCharacter(perso).getWeapon()) {
            cmbWeapon[perso].setCurrentIndex(i);
        }
    }
    //Relic
    for (int i=0; i<cmbRelic[perso].count(); i++) {
        if (cmbRelic[perso].itemData(i) == _game->getCharacter(perso).getRelic()) {
            cmbRelic[perso].setCurrentIndex(i);
        }
    }
}

void Vue::open(QString fn) {
    if (fn.isEmpty())
        fn = selectFile();

    if (fn.isEmpty())
        return;

    if (_sramFile) delete _sramFile;
    _sramFile = new SRAMFile(fn.toStdString());
    _sramFile->read();

    selectGame(0);

    afficherInformations();
}

void Vue::save() {
    if (!_sramFile)
        return;

    updateGame();
    _sramFile->write();
}

void Vue::selectGame(int n) {
    if (_game) {
        updateGame();
    }/* else {
        _game = new Game();
    }*/

    _game = _sramFile->getGame(n);
}

void Vue::afficherInformations() {
    if (!_game)
        return;

    for (int i=0; i<7; i++) {
        Character c = _game->getCharacter(i);

        sbLevel[i].setValue(c.getLevel());
        sbExp[i].setValue(c.getExp());
        sbCurrentHP[i].setValue(c.getCurrentHP());
        sbMaxHP[i].setValue(c.getMaxHP());
        sbCurrentMP[i].setValue(c.getCurrentMP());
        sbMaxMP[i].setValue(c.getMaxMP());
        sbPower[i].setValue(c.getBasePower());
        sbStamina[i].setValue(c.getBaseStamina());
        sbSpeed[i].setValue(c.getBaseSpeed());
        sbMagic[i].setValue(c.getBaseMagic());
        sbHit[i].setValue(c.getBaseHit());
        sbEvade[i].setValue(c.getBaseEvade());
        sbMagicDef[i].setValue(c.getBaseMagicDef());

        selectItems(i);
    }
}

void Vue::updateGame() {
    if (!_game)
        return;

    for (int i=0; i<7; i++) {
        _game->getCharacter(i).setLevel(sbLevel[i].value());
        _game->getCharacter(i).setExp(sbExp[i].value());
        _game->getCharacter(i).setCurrentHP(sbCurrentHP[i].value());
        _game->getCharacter(i).setMaxHP(sbMaxHP[i].value());
        _game->getCharacter(i).setCurrentMP(sbCurrentMP[i].value());
        _game->getCharacter(i).setMaxMP(sbMaxMP[i].value());
        _game->getCharacter(i).setBasePower(sbPower[i].value());
        _game->getCharacter(i).setBaseStamina(sbStamina[i].value());
        _game->getCharacter(i).setBaseSpeed(sbSpeed[i].value());
        _game->getCharacter(i).setBaseMagic(sbMagic[i].value());
        _game->getCharacter(i).setBaseHit(sbHit[i].value());
        _game->getCharacter(i).setBaseEvade(sbEvade[i].value());
        _game->getCharacter(i).setBaseMagicDef(sbMagicDef[i].value());
    }

    _sramFile->setGame(_game, _game->getSlot());
}
