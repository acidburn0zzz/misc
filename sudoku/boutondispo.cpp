#include "boutondispo.h"

BoutonDispo::BoutonDispo(QWidget *parent) : QAction(parent) {
    connect(this, SIGNAL(triggered()), this, SLOT(envoieSignalClique()));
}

void BoutonDispo::setValeur(int v) {
    valeur = v;
}

void BoutonDispo::envoieSignalClique() {
    emit clique(valeur);
}
