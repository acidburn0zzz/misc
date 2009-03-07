#ifndef __BOUTON_DISPO__
#define __BOUTON_DISPO__

#include <QAction>

/*************************************
 * Cette classe n'a pour utilite     *
 * que de fournir un QAction avec    *
 * un constructeur vide et d'envoyer *
 * un signal clique avec sa valeur   *
 *************************************/
class BoutonDispo : public QAction {
    Q_OBJECT
 
    public:
    BoutonDispo(QWidget *parent = 0);
    void setValeur(int v);
    
    public slots:
    void envoieSignalClique();
    
    signals:
    void clique(int i);
    
    private:
    int valeur;
};

#endif //__BOUTON_DISPO__
