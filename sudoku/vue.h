#ifndef __VUE_H__
#define __VUE_H__

#include <QMainWindow>

class QAction;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QMenu;
class QStatusBar;
class QTimer;
class QToolBar;
class QTranslator;
class QVBoxLayout;

class BoutonDispo;
class CaseGrille;
class Grille;

class Vue : public QMainWindow {
    Q_OBJECT
    
    public:
    Vue(QString lang = "", QWidget *parent = 0);
    ~Vue();
    
    //Methodes d'initialisation
    void qtInit(); // Initialise tout ce qui est en lien avec Qt
    void creerActions();
    void creerMenus();
    void creerBarreOutils();
    void creerStatusBar();
    
    void afficherGrille();
    
    public slots:
    void toggleErreurs();
    void toggleDispos();
    void toggleTimer();
    void pause();
    void majTemps();
    void majGrille();
    void majDispos(int x, int y);
    void majStatusBar();
    void entrerChiffre(int i);

    private:
    Grille *_grille;
    
    //Objets QT
    QTranslator *translator;
    QWidget *centralWidget;
    QHBoxLayout *centralLayout;
    QTimer *timer;
    
    //Partie menus / barre d'outils
    QMenu *mnuFichier;
    QMenu *mnuOptions;
    QMenu *mnuAide;
    
    QToolBar *tbOptions;
    QToolBar *tbDispos;
    
    QAction *actNouveau;
    QAction *actQuitter;
    QAction *actAfficherDispos;
    QAction *actAfficherErreurs;
    QAction *actAfficherTemps;
    QAction *actPause;
    QAction *actAPropos;
    QAction *actAProposQt;
    BoutonDispo *btnDispos;
    
    //Partie grille
    CaseGrille *cases;
    QGridLayout *gridGrille;
    
    //Options
    bool    affDispos;      //Afficher les possibilites
    bool    affErr;         //Afficher les erreurs
    bool    affTemps;       //Afficher le chronometre
    int     tailleCase;
    QString styleBase;      //Les parametres par defaut du stylesheet des QLineEdit
    QString langue;         //Contient le code de la langue
    
    //Attributs
    int     temps;
    char    caseX; //Contient le X de la case avec le focus
    char    caseY; //Contient le Y de la case avec le focus
};

#endif //__VUE_H__
