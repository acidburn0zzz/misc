#ifndef __VUEATELIER_H__
#define __VUEATELIER_H__

#include <QDialog>

class QCheckBox;
class QComboBox;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QVBoxLayout;

class ModeleAtelier;

class VueAtelier : public QDialog {
    Q_OBJECT;
    
    public:
    VueAtelier(bool isModif = false, int noAtel = 0, QWidget *parent = 0, Qt::WindowFlags flags = Qt::Dialog);
    ~VueAtelier();

    private:
    void init();
    void fillInfosAtel();
    
    bool _isModif;
    int _noAtel;
    
    ModeleAtelier *_model;
    
    QLabel *_lblHeader;
    QLabel *_lblNoAtel;
    QLabel *_lblTitre;
    QLabel *_lblType;
    QLabel *_lblNomExpo;
    QLabel *_lblNomCat;
    QLabel *_lblNbMax;
    QLabel *_lblNoLocal;
    QLabel *_lblJour;
    QLabel *_lblHeure;
    QLabel *_lblDuree;
    QLabel *_lblCoutAdulte;
    QLabel *_lblCoutEnfant;
    
    QLineEdit *_txtNoAtel;
    QLineEdit *_txtTitre;
    QLineEdit *_txtNbMax;
    QLineEdit *_txtCoutAdulte;
    QLineEdit *_txtCoutEnfant;
    
    QComboBox *_cmbType;
    QComboBox *_cmbNomExpo;
    QComboBox *_cmbNomCat;
    QComboBox *_cmbNoLocal;
    QComboBox *_cmbJour;
    QComboBox *_cmbHeure;
    QComboBox *_cmbDuree;
    
    QRadioButton *_radFrancais;
    QRadioButton *_radAnglais;
    
    QCheckBox *_chkAcetate;
    QCheckBox *_chkRetro;
    QCheckBox *_chkOrdi;
    
    QPushButton *_btnAnnuler;
    QPushButton *_btnTerminer;
    
    QGridLayout *_layCentral;
    QHBoxLayout *_layOptions;
    QHBoxLayout *_layBoutons;
    
    private slots:
    void valider();
};

#endif //__VUEATELIER_H__
