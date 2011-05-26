#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>

#include <string>
#include <sstream>

#include "board.h"
#include "boardgl.h"
#include "controlleur.h"
#include "vue.h"

Vue::Vue(Controlleur *cont, QWidget *parent) : QMainWindow(parent) {
    _cont = cont;
    init();
}

void Vue::init() {
    /*if (showMessage("Graphics", "Do you want OpenGL?", MSG_QUESTION) == QMessageBox::Yes)
        _openGL = true;
    else*/
        _openGL = false;

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    setWindowTitle("Daleks");

    btnNew = new QPushButton(tr("&New game"));
    btnTeleport = new QPushButton(tr("&Teleport"));
    btnTeleport->setEnabled(false);
    btnRun = new QPushButton(tr("&Run"));
    btnRun->setEnabled(false);
    btnZap = new QPushButton(tr("&Zapper"));
    btnZap->setEnabled(false);

    if (_openGL) {
        boardGL = new BoardGL(_NB_CASES);
        boardGL->setEnabled(false);
    } else {
        board = new Board(_NB_CASES);
        board->setEnabled(false);
    }

    QHBoxLayout *hlayButtons = new QHBoxLayout;
    hlayButtons->addWidget(btnNew);
    hlayButtons->addWidget(btnTeleport);
    hlayButtons->addWidget(btnRun);
    hlayButtons->addWidget(btnZap, 1, Qt::AlignLeft);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayButtons, 0);
    if (_openGL)
        layout->addWidget(boardGL);
    else
        layout->addWidget(board);

    connect(btnNew, SIGNAL(clicked()), this, SLOT(newGame()));
    connect(btnTeleport, SIGNAL(clicked()), this, SLOT(teleport()));
    connect(btnRun, SIGNAL(clicked()), this, SLOT(run()));
    connect(btnZap, SIGNAL(clicked()), this, SLOT(zap()));

    if (_openGL) {
        connect(boardGL, SIGNAL(move(int)), this, SLOT(move(int)));
    } else {
        connect(board, SIGNAL(move(int)), this, SLOT(move(int)));
        connect(board, SIGNAL(imagesMissing()), this, SLOT(imagesMissing()));
    }

    createActions();
    createMenus();
    updateStatusBar();

    centralWidget->setLayout(layout);
}

void Vue::createActions() {
    actNewGame = new QAction(tr("&New Game"), this);
    actNewGame->setShortcut(tr("Ctrl+N"));
    actNewGame->setStatusTip(tr("Start a new game"));
    connect(actNewGame, SIGNAL(triggered()), this, SLOT(newGame()));

    actQuit = new QAction(tr("&Quit"), this);
    actQuit->setShortcut(tr("Ctrl+Q"));
    actQuit->setStatusTip(tr("Quit game"));
    connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    actToggleGrid = new QAction(tr("Toggle &Grid"), this);
    actToggleGrid->setShortcut(tr("Ctrl+G"));
    actToggleGrid->setStatusTip(tr("Toggle the board grid"));
    if (_openGL)
        connect(actToggleGrid, SIGNAL(triggered()), boardGL, SLOT(toggleGrid()));
    else
        connect(actToggleGrid, SIGNAL(triggered()), board, SLOT(toggleGrid()));

    actControls = new QAction(tr("Show &Controls"), this);
    actControls->setShortcut(tr("Ctrl+C"));
    actControls->setStatusTip("Show controls");
    connect(actControls, SIGNAL(triggered()), this, SLOT(showControls()));

    actAbout = new QAction(tr("&About"), this);
    actAbout->setShortcut(tr("Ctrl+A"));
    actAbout->setStatusTip("About Daleks");
    connect(actAbout, SIGNAL(triggered()), this, SLOT(showAbouDialog()));

    actAboutQt = new QAction(tr("About &Qt"), this);
    actAboutQt->setStatusTip(tr("About Qt"));
    connect(actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Vue::createMenus() {
    mnuFile = new QMenu(tr("&File"));
    mnuFile->addAction(actNewGame);
    mnuFile->addAction(actQuit);
    menuBar()->addMenu(mnuFile);

    mnuOptions = new QMenu(tr("&Options"));
    mnuOptions->addAction(actToggleGrid);
    menuBar()->addMenu(mnuOptions);

    mnuHelp = new QMenu(tr("&Help"));
    mnuHelp->addAction(actControls);
    mnuHelp->addAction(actAbout);
    mnuHelp->addAction(actAboutQt);
    menuBar()->addMenu(mnuHelp);
}

void Vue::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_0 || event->key() == Qt::Key_R) {
        event->accept();
        run();
    } else if (event->key() == Qt::Key_1) {
        event->accept();
        move(1);
    } else if (event->key() == Qt::Key_2) {
        event->accept();
        move(2);
    } else if (event->key() == Qt::Key_3) {
        event->accept();
        move(3);
    } else if (event->key() == Qt::Key_4) {
        event->accept();
        move(4);
    } else if (event->key() == Qt::Key_5) {
        event->accept();
        move(5);
    } else if (event->key() == Qt::Key_6) {
        event->accept();
        move(6);
    } else if (event->key() == Qt::Key_7) {
        event->accept();
        move(7);
    } else if (event->key() == Qt::Key_8) {
        event->accept();
        move(8);
    } else if (event->key() == Qt::Key_9) {
        event->accept();
        move(9);
    } else if (event->key() == Qt::Key_G) {
        event->accept();
        board->toggleGrid();
    } else if (event->key() == Qt::Key_N) {
        event->accept();
        newGame();
    } else if (event->key() == Qt::Key_T) {
        event->accept();
        teleport();
    } else if (event->key() == Qt::Key_Period) {
        event->accept();
        zap();
    } else {
        event->ignore();
    }
}

int Vue::getNbCases() {
    return _NB_CASES;
}

void Vue::newGame() {
    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;
        btnTeleport->setEnabled(true);
        btnRun->setEnabled(true);
        btnZap->setEnabled(true);

        if (_openGL)
            boardGL->setEnabled(true);
        else
            board->setEnabled(true);

        _cont->nouvellePartie();
        drawGame();
    } else {
        askNewGame();
    }
}

void Vue::drawGame() {
    if (_openGL)
        boardGL->updateCharacters(_cont->getListeDaleks(), _cont->getNbDaleks(), _cont->getDoc());
    else
        board->updateCharacters(_cont->getListeDaleks(), _cont->getNbDaleks(), _cont->getDoc());
    updateStatusBar();
}

//TODO: Changer ca pour des labels en haut du plateau
void Vue::updateStatusBar() {
    static bool firstTime = true;
    QString qs;

    if (firstTime) {
        firstTime = false;
        qs = tr("Welcome to Daleks");
    } else {
        std::stringstream s;
        qs.append(tr("Level: ")).append(QString::number(_cont->getLevel()))
            .append("\t\t").append(tr("Points: ")).append(QString::number(_cont->getPts()))
            .append("\t\t\t").append(tr("Zapper: ")).append(QString::number(_cont->getZaps()));
    }

    statusBar()->showMessage(qs);
}

void Vue::imagesMissing() {
    showMessage(tr("Error"), tr("Images not found"), MSG_CRITICAL);
    exit(0);
}

void Vue::move(int i) {
    switch (i) {
        case 1:
            move(-1, 1);
            break;
        case 2:
            move(0, 1);
            break;
        case 3:
            move(1, 1);
            break;
        case 4:
            move(-1, 0);
            break;
        case 5:
            move(0, 0);
            break;
        case 6:
            move(1, 0);
            break;
        case 7:
            move(-1, -1);
            break;
        case 8:
            move(0, -1);
            break;
        case 9:
            move(1, -1);
    }
}

void Vue::move(int x, int y) {
    if (_cont->gameIsStarted()) {
        _cont->moveDoc(x, y);
    }
}

void Vue::run() {
    if (_cont->gameIsStarted())
        _cont->run();
}

void Vue::teleport() {
    if (_cont->gameIsStarted())
        _cont->teleport();
}

void Vue::zap() {
    if (_cont->gameIsStarted())
        _cont->zap();
}

int Vue::showMessage(QString title, QString message, int type) {
    if (type == MSG_ABOUT)
        QMessageBox::about(this, title, message);
    else if (type == MSG_INFORMATION)
        QMessageBox::information(this, title, message);
    else if (type == MSG_CRITICAL)
        QMessageBox::critical(this, title, message);
    else if (type == MSG_QUESTION)
        return QMessageBox::question(this, title, message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    return 0;
}

void Vue::showAbouDialog() {
    showMessage(tr("About Daleks"), tr("Daleks, Version 0.9.6"), MSG_ABOUT);
}

void Vue::showControls() {
    showMessage(tr("Controls"), tr("Numpad : Move\nN : New game\nG : Toggle grid\nT : Teleport\n0 : Run\n. : Zap"), MSG_INFORMATION);
}

void Vue::askNewGame() {
    if (showMessage(tr("Abort"), tr("Game running\nDo you want to start a new game?"), MSG_QUESTION) == QMessageBox::Yes) {
        _cont->nouvellePartie();
        drawGame();
    }
}

void Vue::askNewGameAfterLose() {
    if (showMessage(tr("You lose!"), tr("Play again?"), MSG_QUESTION) == QMessageBox::Yes) {
        _cont->nouvellePartie();
        drawGame();
    } else {
        exit(0);
    }
}
