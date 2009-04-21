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

class VueAtelier : public QDialog {
    Q_OBJECT;
    
    public:
    VueAtelier(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VueAtelier();

    private:
    void init();
    
    QLabel *_lblHeader;
    QLabel *_lblNoAtel;
    QLabel *_lblTitre;
    QLabel *_lblType;
    QLabel *_lblNomExpo;
    QLabel *_lblNomCat;
    QLabel *_lblNbMax;
    QLabel *_lblLangue;
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
    QCheckBox *_chkOrdinateur;
    
    QPushButton *_btnAnnuler;
    QPushButton *_btnTerminer;
    
    QGridLayout *_layCentral;
    QHBoxLayout *_layOptions;
    QHBoxLayout *_layBoutons;
    
    private slots:
};

#endif //__VUEATELIER_H__
