#ifndef __VUEGESTIONATELIER_H__
#define __VUEGESTIONATELIER_H__

#include <QModelIndex>
#include <QWidget>

class ModeleGestionAtelier;
class TableGestionAtelier;

class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;

class VueGestionAtelier : public QWidget {
    Q_OBJECT;
    
    public:
    VueGestionAtelier(QWidget *parent = 0);
    ~VueGestionAtelier();
    
    void init();
    
    private:
    TableGestionAtelier *_table;
    ModeleGestionAtelier *_modele;
    
    QPushButton *_btnNouveau;
    QPushButton *_btnModifier;
    QPushButton *_btnSupprimer;
    QPushButton *_btnInscription;
    QPushButton *_btnX;
    QPushButton *_btnQuitter;
    
    QLabel *_lblNbAteliers;
    QLineEdit *_txtNbAteliers;
    
    QHBoxLayout *_layBoutons;
    QHBoxLayout *_layNbAteliers;
    QVBoxLayout *_layCentral;
    
    public slots:
    void updateNbAteliers(const QModelIndex & parent, int start, int end);
    void ajouterAtelier();
    void modifierAtelier();
    void supprimerAtelier();
    void cancelQuit();
    void commitQuit();
};

#endif //__VUEGESTIONATELIER_H__
