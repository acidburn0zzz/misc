#include <QtGui>

#include "modelegestionatelier.h"
#include "tablegestionatelier.h"
#include "vuegestionatelier.h"
#include "vueatelier.h"

VueGestionAtelier::VueGestionAtelier(QWidget *parent) : QWidget(parent) {
    init();
}

VueGestionAtelier::~VueGestionAtelier() {
    QLayoutItem *child;

    for (int i=_layBoutons->count() - 1; i>=0; i--) {
        child = _layBoutons->takeAt(i);
        delete child;
    }
    delete _layBoutons;

    for (int i=_layNbAteliers->count() - 1; i>=0; i--) {
        child = _layNbAteliers->takeAt(i);
        delete child;
    }
    delete _layNbAteliers;

    for (int i=_layCentral->count() - 1; i>=0; i--) {
        child = _layCentral->takeAt(i);
        delete child;
    }
    delete _layCentral;

    delete _modele;
}

void VueGestionAtelier::init() {
    _table = new TableGestionAtelier();
    _modele = new ModeleGestionAtelier();
    _table->setModel(_modele);
    _table->resizeColumnsToContents();

    _btnNouveau = new QPushButton(tr("&Nouveau"));
    _btnModifier = new QPushButton(tr("&Modifier"));
    _btnSupprimer = new QPushButton(tr("&Supprimer"));
    _btnInscription = new QPushButton(tr("&Inscription"));
    _btnQuitter = new QPushButton(tr("&Quitter"));
    _btnX = new QPushButton(tr("&X"));
    _btnX->setMaximumWidth(25);

    _lblNbAteliers = new QLabel(tr("Nombre total d'ateliers"));
    _txtNbAteliers = new QLineEdit();
    _txtNbAteliers->setMaximumWidth(40);
    _txtNbAteliers->setReadOnly(true);
    _txtNbAteliers->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _txtNbAteliers->setText(QString::number(_modele->rowCount()));

    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnNouveau);
    _layBoutons->addWidget(_btnModifier);
    _layBoutons->addWidget(_btnSupprimer);
    _layBoutons->addWidget(_btnInscription);
    _layBoutons->addStretch(this->width());
    _layBoutons->addWidget(_btnQuitter);
    _layBoutons->addWidget(_btnX);

    _layNbAteliers = new QHBoxLayout();
    _layNbAteliers->addStretch(this->width());
    _layNbAteliers->addWidget(_lblNbAteliers);
    _layNbAteliers->addWidget(_txtNbAteliers);

    _layCentral = new QVBoxLayout(this);
    _layCentral->addLayout(_layBoutons);
    _layCentral->addWidget(_table);
    _layCentral->addLayout(_layNbAteliers);

    this->setLayout(_layCentral);

    connect(_modele, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(updateNbAteliers(const QModelIndex &, int, int)));
    connect(_modele, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(updateNbAteliers(const QModelIndex &, int, int)));

    connect(_btnNouveau, SIGNAL(clicked()), this, SLOT(ajouterAtelier()));
    connect(_btnModifier, SIGNAL(clicked()), this, SLOT(modifierAtelier()));
    connect(_btnSupprimer, SIGNAL(clicked()), this, SLOT(supprimerAtelier()));

    connect(_btnQuitter, SIGNAL(clicked()), this, SLOT(commitQuit()));
    connect(_btnX, SIGNAL(clicked()), this, SLOT(cancelQuit()));
}

void VueGestionAtelier::updateNbAteliers(const QModelIndex & parent, int start, int end) {
    _txtNbAteliers->setText(QString::number(_modele->rowCount()));
}

void VueGestionAtelier::ajouterAtelier() {
    QModelIndex index = _table->currentIndex();

    VueAtelier v;
    if (v.exec()) {
        _table->refresh(&index);
    }
}

void VueGestionAtelier::modifierAtelier() {
    QModelIndex index = _table->currentIndex();
    int row = (index.row() == -1)? 0 : index.row();
    int id = _modele->data(_modele->index(row, 0), Qt::DisplayRole).toInt();

    VueAtelier v(true, id);
    if (v.exec()) {
        _table->refresh(&index);
    }
}

void VueGestionAtelier::supprimerAtelier() {
    int row = _modele->getSelectedRow();
    _modele->removeRows(row, 1);

    QModelIndex index = _table->currentIndex();
    _table->refresh(&index);
}

void VueGestionAtelier::commitQuit() {
    _modele->commit();
    qApp->quit();
}

void VueGestionAtelier::cancelQuit() {
    _modele->rollback();
    qApp->quit();
}
