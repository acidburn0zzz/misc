#ifndef VUE_H_INCLUDED
#define VUE_H_INCLUDED

#include <QMainWindow>

class Board;
class BoardGL;
class Buttons;
class Controlleur;

class QAction;
class QKeyEvent;
class QMenu;
class QPushButton;

enum { MSG_ABOUT, MSG_CRITICAL, MSG_INFORMATION, MSG_QUESTION };

class Vue : public QMainWindow {
    Q_OBJECT

public:
    Vue(Controlleur *cont, QWidget *parent = 0);

    int getNbCases();
    int getNbDaleks();
    int showMessage(char *title, char *message, int type);
    void askNewGame();
    void askNewGameAfterLose();
    void drawGame();
    void move(int x, int y);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void imagesMissing();
    void move(int i);
    void newGame();
    void run();
    void showAbouDialog();
    void showControls();
    void teleport();
    void zap();

private:
    Controlleur *_cont;
    const static int _NB_CASES = 30;
    bool _openGL;

    QMenu *mnuFile;
    QAction *actNewGame;
    QAction *actQuit;
    QMenu *mnuOptions;
    QAction *actToggleGrid;
    QMenu *mnuHelp;
    QAction *actControls;
    QAction *actAbout;
    QAction *actAboutQt;

    QPushButton *btnNew;
    QPushButton *btnTeleport;
    QPushButton *btnRun;
    QPushButton *btnZap;

    Board *board;
    BoardGL *boardGL;

    void init();
    void createActions();
    void createMenus();
    void updateStatusBar();
};

#endif // VUE_H_INCLUDED
