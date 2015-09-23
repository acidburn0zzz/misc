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
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>

#include "game.h"
#include "sramfile.h"
#include "strings.h"
#include "gui.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    _init();
}

MainWindow::MainWindow(const char *fn, QWidget *parent) : QMainWindow(parent) {
    _init();
    open(QString(fn));
}

MainWindow::~MainWindow() {
    if (_sramFile) delete _sramFile;
    if (_game) delete _game;
}

void MainWindow::_init() {
    _sramFile = NULL;
    _game = NULL;

    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    setWindowTitle("Chrono Trigger Save Game Editor");

    // Menu bar
    actOpen = new QAction(tr("&Open"), this);
    actOpen->setShortcut(tr("Ctrl+O"));
    actOpen->setStatusTip(tr("Open a save file"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));

    actSave = new QAction(tr("&Save"), this);
    actSave->setShortcut(tr("Ctrl+S"));
    actSave->setStatusTip(tr("Save the save file"));
    connect(actSave, SIGNAL(triggered()), this, SLOT(save()));

    actQuit = new QAction(tr("&Quit"), this);
    actQuit->setShortcut(tr("Ctrl+Q"));
    actQuit->setStatusTip(tr("Quit the application"));
    connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    mnuFile = new QMenu(tr("&File"));
    mnuFile->addAction(actOpen);
    mnuFile->addAction(actSave);
    mnuFile->addAction(actQuit);
    menuBar()->addMenu(mnuFile);

    actAbout = new QAction(tr("&About Chrono Editor"), this);
    actAbout->setShortcut(tr("Ctrl+A"));

    actAboutQt = new QAction(tr("About &QT"), this);
    actAboutQt->setShortcut(tr("Ctrl+T"));
    connect(actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    mnuHelp = new QMenu(tr("&Help"));
    mnuHelp->addAction(actAbout);
    mnuHelp->addAction(actAboutQt);
    menuBar()->addMenu(mnuHelp);

    centralLayout = new QVBoxLayout();
    tabCharacters = new QTabWidget(centralWidget);

    //Initialisation des tableaux d'objets pour les stats
    wCharacters = new QWidget[7];

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
        _fillCharacterValues(i);

        //Pourrait enlever ca pour utiliser chaque 1 fois

        QPixmap img(_imageFile);
        img = img.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding);
        lblImage[i].setPixmap(img);

        lblLevel[i].setText(tr("Level"));
        lblLevel[i].setParent(&wCharacters[i]);
        lblExp[i].setText(tr("Experience"));
        lblExp[i].setParent(&wCharacters[i]);
        lblCurrentHP[i].setText("Current HP");
        lblCurrentHP[i].setParent(&wCharacters[i]);
        lblMaxHP[i].setText("Max HP");
        lblMaxHP[i].setParent(&wCharacters[i]);
        lblCurrentMP[i].setText("Current MP");
        lblCurrentMP[i].setParent(&wCharacters[i]);
        lblMaxMP[i].setText("Max MP");
        lblMaxMP[i].setParent(&wCharacters[i]);
        lblPower[i].setText(tr("Power"));
        lblPower[i].setParent(&wCharacters[i]);
        lblStamina[i].setText(tr("Stamina"));
        lblStamina[i].setParent(&wCharacters[i]);
        lblSpeed[i].setText(tr("Speed"));
        lblSpeed[i].setParent(&wCharacters[i]);
        lblMagic[i].setText(tr("Magic"));
        lblMagic[i].setParent(&wCharacters[i]);
        lblHit[i].setText(tr("Hit"));
        lblHit[i].setParent(&wCharacters[i]);
        lblEvade[i].setText(tr("Evade"));
        lblEvade[i].setParent(&wCharacters[i]);
        lblMagicDef[i].setText("Magic Def");
        lblMagicDef[i].setParent(&wCharacters[i]);
        lblHelmet[i].setText(tr("Helmet"));
        lblHelmet[i].setParent(&wCharacters[i]);
        lblArmor[i].setText(tr("Armor"));
        lblArmor[i].setParent(&wCharacters[i]);
        lblWeapon[i].setText(tr("Weapon"));
        lblWeapon[i].setParent(&wCharacters[i]);
        lblRelic[i].setText(tr("Relic"));
        lblRelic[i].setParent(&wCharacters[i]);

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
        gridItems[i].addWidget(&lblWeapon[i], 0, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbWeapon[i], 1, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblArmor[i], 2, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbArmor[i], 3, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblHelmet[i], 4, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbHelmet[i], 5, 0, Qt::AlignTop);
        gridItems[i].addWidget(&lblRelic[i], 6, 0, Qt::AlignBottom);
        gridItems[i].addWidget(&cmbRelic[i], 7, 0, Qt::AlignTop);

        //Ajouter les 3 layouts au layout du widget du perso
        hlayPerso[i].addLayout(&gridStatsBase[i]);
        hlayPerso[i].addLayout(&gridStats[i]);
        hlayPerso[i].addLayout(&gridItems[i]);

        //Ajouter le layout au widget du perso
        wCharacters[i].setLayout(&hlayPerso[i]);

        //Ajouter le widget du perso au Tab
        tabCharacters->addTab(&wCharacters[i], _name);
    }

    //Layout principal
    centralLayout->addWidget(tabCharacters, 1);

    centralWidget->setLayout(centralLayout);
    this->setMinimumSize(400, 300);
}

QString MainWindow::selectFile() {
    return QFileDialog::getOpenFileName(0, tr("Choose a file"), QString(), tr("SRAM file (*.srm);;All files (*.*)"));
}

void MainWindow::_fillCharacterValues(int charId) {
    switch (charId) {
        case CRONO:
            _name = "Crono";
            _imageFile = "images/crono.png";
            for (int i=0x01; i<=0x10; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            cmbWeapon[charId].addItem(itemList[0x4f], 0x4f);
            for (int i=0x53; i<=0x55; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case MARLE:
            _name = "Marle";
            _imageFile = "images/marle.png";
            for (int i=0x11; i<=0x1a; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case LUCCA:
            _name = "Lucca";
            _imageFile = "images/lucca.png";
            for (int i=0x1f; i<=0x29; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case ROBO:
            _name = "Robo";
            _imageFile = "images/robo.png";
            for (int i=0x2e; i<=0x39; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case FROG:
            _name = "Frog";
            _imageFile = "images/frog.png";
            for (int i=0x3b; i<=0x43; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            for (int i=0x50; i<=0x52; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case AYLA:
            _name = "Ayla";
            _imageFile = "images/ayla.png";
            for (int i=0x44; i<=0x48; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
        case MAGUS:
            _name = "Magus";
            _imageFile = "images/magus.png";
            for (int i=0x4b; i<=0x4e; i++)
                cmbWeapon[charId].addItem(itemList[i], i);
            break;
    }
}

void MainWindow::selectItems(int charId) {
    //Helmet
    for (int i=0; i<cmbHelmet[charId].count(); i++) {
        if (cmbHelmet[charId].itemData(i) == _game->getCharacter(charId).getHelmet()) {
            cmbHelmet[charId].setCurrentIndex(i);
            break;
        }
    }
    //Armor
    for (int i=0; i<cmbArmor[charId].count(); i++) {
        if (cmbArmor[charId].itemData(i) == _game->getCharacter(charId).getArmor()) {
            cmbArmor[charId].setCurrentIndex(i);
            break;
        }
    }
    //Weapon
    for (int i=0; i<cmbWeapon[charId].count(); i++) {
        if (cmbWeapon[charId].itemData(i) == _game->getCharacter(charId).getWeapon()) {
            cmbWeapon[charId].setCurrentIndex(i);
            break;
        }
    }
    //Relic
    for (int i=0; i<cmbRelic[charId].count(); i++) {
        if (cmbRelic[charId].itemData(i) == _game->getCharacter(charId).getRelic()) {
            cmbRelic[charId].setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::open(QString fn) {
    if (fn.isEmpty())
        fn = selectFile();

    if (fn.isEmpty())
        return;

    if (_sramFile) delete _sramFile;
    if (_game) delete _game;
    _sramFile = new SRAMFile(fn.toStdString());
    _sramFile->read();

    _game = _sramFile->getGame(0);

    afficherInformations();
}

void MainWindow::save() {
    if (!_sramFile)
        return;

    updateGame();
    _sramFile->write();
}

void MainWindow::selectGame(int n) {
    if (_game) {
        updateGame();
    }/* else {
        _game = new Game();
    }*/

    _game = _sramFile->getGame(n);
}

void MainWindow::afficherInformations() {
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

void MainWindow::updateGame() {
    if (!_game)
        return;

    for (int i=0; i<7; i++) {
        Character c = _game->getCharacter(i);

        c.setLevel(sbLevel[i].value());
        c.setExp(sbExp[i].value());
        c.setCurrentHP(sbCurrentHP[i].value());
        c.setMaxHP(sbMaxHP[i].value());
        c.setCurrentMP(sbCurrentMP[i].value());
        c.setMaxMP(sbMaxMP[i].value());
        c.setBasePower(sbPower[i].value());
        c.setBaseStamina(sbStamina[i].value());
        c.setBaseSpeed(sbSpeed[i].value());
        c.setBaseMagic(sbMagic[i].value());
        c.setBaseHit(sbHit[i].value());
        c.setBaseEvade(sbEvade[i].value());
        c.setBaseMagicDef(sbMagicDef[i].value());
        c.setWeapon((u8) cmbWeapon[i].itemData(cmbWeapon[i].currentIndex()).toInt());
        c.setArmor((u8) cmbArmor[i].itemData(cmbArmor[i].currentIndex()).toInt());
        c.setHelmet((u8) cmbHelmet[i].itemData(cmbHelmet[i].currentIndex()).toInt());
        c.setRelic((u8) cmbRelic[i].itemData(cmbRelic[i].currentIndex()).toInt());
    }
}
