#include <QtGui>

#include "modeleatelier.h"
#include "vueatelier.h"

VueAtelier::VueAtelier(bool isModif, int noAtel, QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags) {
    _isModif = isModif;
    _noAtel = noAtel;

    init();
    this->setModal(true);

    if (_isModif) {
        _model = new ModeleAtelier(_noAtel);
    } else {
        _model = new ModeleAtelier();

        /* Valeur par defaut pour un nouvel enregistrement */
        _radFrancais->setChecked(true);

        _txtCoutAdulte->setText(QString::number(10));
        _txtCoutEnfant->setText(QString::number(10));
        _txtNbMax->setText(QString::number(10));
        _txtTitre->setText("java");
    }

    _txtTitre->setFocus();

    fillInfosAtel();
}

VueAtelier::~VueAtelier() {
    QLayoutItem *child;

    for (int i=_layOptions->count() - 1; i>=0; i--) {
        child = _layOptions->takeAt(i);
        delete child;
    }
    delete _layOptions;

    for (int i=_layBoutons->count() - 1; i>=0; i--) {
        child = _layBoutons->takeAt(i);
        delete child;
    }
    delete _layBoutons;

    for (int i=_layCentral->count() - 1; i>=0; i--) {
        child = _layCentral->takeAt(i);
        delete child;
    }
    delete _layCentral;

    delete _model;
}

void VueAtelier::init() {
    if (_isModif)
        this->setWindowTitle(tr("Modification d'un atelier"));
    else
        this->setWindowTitle(tr("Ajout d'un atelier"));

    //Affichage
    _lblHeader = new QLabel(tr("Information sur l'atelier"));
    _lblNoAtel = new QLabel(tr("No d'atelier"));
    _lblTitre = new QLabel(tr("Titre"));
    _lblType = new QLabel(tr("Type"));
    _lblNomExpo = new QLabel(tr("Nom d'exposant"));
    _lblNomCat = new QLabel(tr("Nom Categorie"));
    _lblNbMax = new QLabel(tr("Nb Maximum"));
    _lblNoLocal = new QLabel(tr("No Local"));
    _lblJour = new QLabel(tr("Jour"));
    _lblHeure = new QLabel(tr("Heure"));
    _lblDuree = new QLabel(tr("Duree"));
    _lblCoutAdulte = new QLabel(tr("Cout Adulte"));
    _lblCoutEnfant = new QLabel(tr("Cout Enfant"));

    _txtNoAtel = new QLineEdit();
    _txtNoAtel->setEnabled(false);
    _txtTitre = new QLineEdit();
    _txtTitre->setValidator(new QRegExpValidator(QRegExp("[\\w\\d\\s-]+"), this));
    _txtNbMax = new QLineEdit();
    _txtNbMax->setValidator(new QIntValidator(0, 999, this));
    _txtCoutAdulte = new QLineEdit();
    _txtCoutAdulte->setValidator(new QIntValidator(0, 999, this));
    _txtCoutEnfant = new QLineEdit();
    _txtCoutEnfant->setValidator(new QIntValidator(0, 999, this));

    _cmbType = new QComboBox();
    _cmbNomExpo = new QComboBox();
    _cmbNomCat = new QComboBox();
    _cmbNoLocal = new QComboBox();
    _cmbJour = new QComboBox();
    _cmbHeure = new QComboBox();
    _cmbDuree = new QComboBox();

    _radFrancais = new QRadioButton(tr("Francais"));
    _radAnglais = new QRadioButton(tr("Anglais"));
    _chkAcetate = new QCheckBox(tr("Acetate Electronique"));
    _chkRetro = new QCheckBox(tr("Retroprojecteur"));
    _chkOrdi = new QCheckBox(tr("Ordinateur"));

    _btnAnnuler = new QPushButton(tr("Annuler"));
    _btnTerminer = new QPushButton(tr("Terminer"));

    _layOptions = new QHBoxLayout();
    _layOptions->addWidget(_radFrancais);
    _layOptions->addWidget(_radAnglais);
    _layOptions->addWidget(_chkAcetate);
    _layOptions->addWidget(_chkRetro);
    _layOptions->addWidget(_chkOrdi);

    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnAnnuler);
    _layBoutons->addWidget(_btnTerminer);

    _layCentral = new QGridLayout(this);
    _layCentral->addWidget(_lblHeader, 0, 0, 1, 4, Qt::AlignCenter);
    _layCentral->addWidget(_lblNoAtel, 1, 0);
    _layCentral->addWidget(_txtNoAtel, 1, 1);
    _layCentral->addWidget(_lblNoLocal, 1, 2);
    _layCentral->addWidget(_cmbNoLocal, 1, 3);
    _layCentral->addWidget(_lblTitre, 2, 0);
    _layCentral->addWidget(_txtTitre, 2, 1);
    _layCentral->addWidget(_lblJour, 2, 2);
    _layCentral->addWidget(_cmbJour, 2, 3);
    _layCentral->addWidget(_lblType, 3, 0);
    _layCentral->addWidget(_cmbType, 3, 1);
    _layCentral->addWidget(_lblHeure, 3, 2);
    _layCentral->addWidget(_cmbHeure, 3, 3);
    _layCentral->addWidget(_lblNomExpo, 4, 0);
    _layCentral->addWidget(_cmbNomExpo, 4, 1);
    _layCentral->addWidget(_lblDuree, 4, 2);
    _layCentral->addWidget(_cmbDuree, 4, 3);
    _layCentral->addWidget(_lblNomCat, 5, 0);
    _layCentral->addWidget(_cmbNomCat, 5, 1);
    _layCentral->addWidget(_lblCoutAdulte, 5, 2);
    _layCentral->addWidget(_txtCoutAdulte, 5, 3);
    _layCentral->addWidget(_lblNbMax, 6, 0);
    _layCentral->addWidget(_txtNbMax, 6, 1);
    _layCentral->addWidget(_lblCoutEnfant, 6, 2);
    _layCentral->addWidget(_txtCoutEnfant, 6, 3);
    _layCentral->addLayout(_layOptions, 7, 0, 1, 4);
    _layCentral->addLayout(_layBoutons, 8, 0, 1, 4, Qt::AlignRight);

    connect(_btnAnnuler, SIGNAL(clicked()), this, SLOT(reject()));
    connect(_btnTerminer, SIGNAL(clicked()), this, SLOT(valider()));

    this->setLayout(_layCentral);
}

void VueAtelier::fillInfosAtel() {
    _txtNoAtel->setText(QString::number(_model->getId()));

    _cmbType->insertItems(0, _model->getTypes());
    _cmbNomExpo->insertItems(0, _model->getExposants());
    _cmbNomCat->insertItems(0, _model->getCategories());
    _cmbNoLocal->insertItems(0, _model->getLocaux());
    _cmbJour->insertItems(0, _model->getJours());
    _cmbHeure->insertItems(0, _model->getHeures());
    _cmbDuree->insertItems(0, _model->getDurees());

    if (_isModif) {
        _txtTitre->setText(_model->getValue(ModeleAtelier::TITRE).toString());
        _txtNbMax->setText(_model->getValue(ModeleAtelier::NBMAXIMUM).toString());
        _txtCoutAdulte->setText(_model->getValue(ModeleAtelier::COUT_A).toString());
        _txtCoutEnfant->setText(_model->getValue(ModeleAtelier::COUT_E).toString());

        _cmbType->setCurrentIndex(_model->getValue(ModeleAtelier::NOTYPE).toInt() - 1);
        _cmbNomExpo->setCurrentIndex(_model->getValue(ModeleAtelier::NOEXPOSANT).toInt() - 1);
        _cmbNomCat->setCurrentIndex(_model->getValue(ModeleAtelier::NOCATEGORIE).toInt() - 1);
        _cmbNoLocal->setCurrentIndex(_model->getValue(ModeleAtelier::NOLOCAL).toInt() - 1);

        _model->parseDate();
        switch (_model->getJour()) {
            case 0: //Dimanche
                _cmbJour->setCurrentIndex(2);
                break;
            case 5: //Vendredi
                _cmbJour->setCurrentIndex(0);
                break;
            case 6: //Samedi
                _cmbJour->setCurrentIndex(1);
                break;
            default:
                _cmbJour->setCurrentIndex(0);
        }

        _cmbHeure->setCurrentIndex(_model->getHeure() - 10); /* 10h est a l'index 0 */

        switch (_model->getValue(ModeleAtelier::DUREE).toInt()) {
            case 30:
                _cmbDuree->setCurrentIndex(0);
                break;
            case 45:
                _cmbDuree->setCurrentIndex(1);
                break;
            case 60:
                _cmbDuree->setCurrentIndex(2);
                break;
            case 90:
                _cmbDuree->setCurrentIndex(3);
                break;
        }

        if (_model->getValue(ModeleAtelier::LANGUE).toString().toLower() == "f")
            _radFrancais->setChecked(true);
        else
            _radAnglais->setChecked(true);

        if (_model->getValue(ModeleAtelier::ACETATE).toInt() != 0)
            _chkAcetate->setChecked(true);
        else
            _chkAcetate->setChecked(false);

        if (_model->getValue(ModeleAtelier::RETRO).toInt() != 0)
            _chkRetro->setChecked(true);
        else
            _chkRetro->setChecked(false);

        if (_model->getValue(ModeleAtelier::PORTABLE).toInt() != 0)
            _chkOrdi->setChecked(true);
        else
            _chkOrdi->setChecked(false);
    }
}

void VueAtelier::valider() {
    if (!_txtTitre->hasAcceptableInput()) {
        QMessageBox::information(this, tr("Titre invalide"), tr("Veuillez entrer un titre d'atelier valide"));
        _txtTitre->setFocus();
        return;
    } else {
        _model->setValue(QVariant::fromValue(_txtTitre->text()), ModeleAtelier::TITRE);
    }

    _model->setValue(QVariant::fromValue(_cmbType->currentIndex() + 1), ModeleAtelier::NOTYPE);
    _model->setValue(QVariant::fromValue(_cmbNomExpo->currentIndex() + 1), ModeleAtelier::NOEXPOSANT);
    _model->setValue(QVariant::fromValue(_cmbNomCat->currentIndex() + 1), ModeleAtelier::NOCATEGORIE);

    if (!_txtNbMax->hasAcceptableInput()) {
        QMessageBox::information(this, tr("Nombre Maximum invalide"), tr("Veuillez entrer un nombre d'auditeurs maximum valide (0-999)"));
        _txtNbMax->setFocus();
        return;
    } else {
        _model->setValue(QVariant::fromValue(_txtNbMax->text().toInt()), ModeleAtelier::NBMAXIMUM);
        _model->setValue(QVariant::fromValue(_cmbNoLocal->currentIndex() + 1), ModeleAtelier::NOLOCAL);
    }

    // TODO: Valider l'heure en fct du local
    int heure = _cmbHeure->currentIndex() + 10; //Index 0 represente 10h
    int jour = _cmbJour->currentIndex();
    _model->setValue(QVariant::fromValue(_model->genererDate(heure, jour)), ModeleAtelier::DATE);

    switch (_cmbDuree->currentIndex()) {
        case 0:
            _model->setValue(QVariant::fromValue(30), ModeleAtelier::DUREE);
            break;
        case 1:
            _model->setValue(QVariant::fromValue(45), ModeleAtelier::DUREE);
            break;
        case 2:
            _model->setValue(QVariant::fromValue(60), ModeleAtelier::DUREE);
            break;
        case 3:
            _model->setValue(QVariant::fromValue(90), ModeleAtelier::DUREE);
            break;
    }

    if (!_txtCoutAdulte->hasAcceptableInput()) {
        QMessageBox::information(this, tr("Cout Adulte invalide"), tr("Veuillez entrer un cout adulte valide (0-999)"));
        _txtCoutAdulte->setFocus();
        return;
    } else {
        _model->setValue(QVariant::fromValue(_txtCoutAdulte->text().toInt()), ModeleAtelier::COUT_A);
    }
    if (!_txtCoutEnfant->hasAcceptableInput()) {
        QMessageBox::information(this, tr("Cout Enfant invalide"), tr("Veuillez entrer un cout enfant valide (0-999)"));
        _txtCoutEnfant->setFocus();
        return;
    } else {
        _model->setValue(QVariant::fromValue(_txtCoutEnfant->text().toInt()), ModeleAtelier::COUT_E);
    }

    if (_radAnglais->isChecked())
        _model->setValue(QVariant::fromValue(QString("A")), ModeleAtelier::LANGUE);
    else
        _model->setValue(QVariant::fromValue(QString("F")), ModeleAtelier::LANGUE);

    if (_chkAcetate->isChecked())
        _model->setValue(QVariant::fromValue(1), ModeleAtelier::ACETATE);
    else
        _model->setValue(QVariant::fromValue(0), ModeleAtelier::ACETATE);

    if (_chkRetro->isChecked())
        _model->setValue(QVariant::fromValue(1), ModeleAtelier::RETRO);
    else
        _model->setValue(QVariant::fromValue(0), ModeleAtelier::RETRO);

    if (_chkOrdi->isChecked())
        _model->setValue(QVariant::fromValue(1), ModeleAtelier::PORTABLE);
    else
        _model->setValue(QVariant::fromValue(0), ModeleAtelier::PORTABLE);

    /* Validations des donnees par rapport a la BDD */
    if (!_model->capaciteValide()) {
        QMessageBox::information(this, tr("Local surcharge"), tr("Veuillez choisir un local plus grand ou diminuer le maximum d'auditeurs de l'atelier"));
        _txtNbMax->setFocus();
        return;
    }

    if (_model->titreExisteDeja()) {
        QMessageBox::information(this, tr("Titre invalide"), tr("Ce titre existe deja, veuillez en choisir un autre"));
        _txtTitre->setFocus();
        return;
    }

    if (_model->conflitHoraire()) {
        QMessageBox::information(this, tr("Conflit d'horaire"), tr("L'atelier en chevauche un autre, veuillez changer de local ou d'heure"));
        _cmbNoLocal->setFocus();
        return;
    }

    if (_isModif)
        _model->updateEntry();
    else
        _model->addEntry();

    this->accept();
}
