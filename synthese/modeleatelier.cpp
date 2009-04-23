#include <QtSql>

#include "modeleatelier.h"

ModeleAtelier::ModeleAtelier() {
    /* On va chercher le no du prochain atelier */
    _query.exec("SELECT seq FROM sqlite_sequence WHERE name='p_atelier'");
    _query.first();
    _noAtel = _query.value(0).toInt() + 1;
    
    //~ fillListes();
}

ModeleAtelier::ModeleAtelier(int noAtel) {
    _noAtel = noAtel;
    
    /* On remplit les infos */
    QString q;
    q = "SELECT a.titre Titre, ";
    q += "strftime('%w', a.dateatel) Jour, ";
    q += "strftime('%H', a.dateatel) Heure, ";
    q += "a.duree Duree, ";
    q += "a.nolocal Local, ";
    q += "a.langue Langue, ";
    q += "a.notype Type, ";
    q += "a.nocategorie Categorie, ";
    q += "a.nbmaximum Max, ";
    q += "a.coutRegulier CoutAdulte, ";
    q += "a.coutEtudiant CoutEnfant, ";
    q += "a.noExposant Exposant ";
    q += "FROM p_atelier a ";
    q += "WHERE a.noatel = " + QString::number(noAtel);
    
    //~ qDebug()<< q;
    _query.exec(q);
    _query.first();
    
    _titre = _query.value(0).toString();
    _jour = _query.value(1).toInt();
    _heure = _query.value(2).toInt();
    _duree = _query.value(3).toInt();
    _noLocal = _query.value(4).toInt();
    _langue = _query.value(5).toString();
    _type = _query.value(6).toInt();
    _noCat = _query.value(7).toInt();
    _coutAdulte = _query.value(8).toInt();
    _coutEnfant = _query.value(9).toInt();
    _nbMax = _query.value(10).toInt();
    _noExpo = _query.value(11).toInt();
    
    fillListes();
}

void ModeleAtelier::fillListes() {
    /* Types */
    _query.exec("SELECT nomtype FROM p_type ORDER BY notype");
    _query.first();
    
    while (_query.isValid()) {
        _types.append(_query.value(0).toString());
        _query.next();
    }
    
    /* Exposants */
    _query.exec("SELECT prenom || ' ' || nom FROM p_exposant ORDER BY noexposant");
    _query.first();
    
    while (_query.isValid()) {
        _exposants.append(_query.value(0).toString());
        _query.next();
    }
    
    /* Categories */
    _query.exec("SELECT nom FROM p_categorie ORDER BY nocategorie");
    _query.first();
    
    while (_query.isValid()) {
        _categories.append(_query.value(0).toString());
        _query.next();
    }
    
    /*
    QStringList _jours;
    QStringList _heures;
    QStringList _durees;
    */
    
    /* Locaux */
    _query.exec("SELECT nolocal || ' (' || capacite || ')' FROM p_local ORDER BY idlocal");
    _query.first();
    
    while (_query.isValid()) {
        _locaux.append(_query.value(0).toString());
        _query.next();
    }
    
    /* Jours */
    _jours << tr("Dimanche") << tr("Lundi") << tr("Mardi") << tr("Mercredi") << tr("Jeudi") << tr("Vendredi") << tr("Samedi");
    
    /* Heures */
    for (int i=10; i<=15; i++)
        _heures.append(QString::number(i) + "h00");
    
    /* Durees */
    _durees << "30" << "45" << "60" << "90";
}

int ModeleAtelier::getNoAtel() {
    return _noAtel;
}

QString ModeleAtelier::getTitre() {
    return _titre;
}

void ModeleAtelier::setTitre(QString titre) {
    _titre = titre;
}

int ModeleAtelier::getType() {
    return _type;
}

void ModeleAtelier::setType(int type) {
    _type = type;
}

int ModeleAtelier::getNoExpo() {
    return _noExpo;
}

void ModeleAtelier::setNoExpo(int noExpo) {
    _noExpo = noExpo;
}

int ModeleAtelier::getNoCat() {
    return _noCat;
}

void ModeleAtelier::setNoCat(int noCat) {
    _noCat = noCat;
}

int ModeleAtelier::getNbMax() {
    return _nbMax;
}

void ModeleAtelier::setNbMax(int nbMax) {
    _nbMax = nbMax;
}

QString ModeleAtelier::getLangue() {
    return _langue;
}

void ModeleAtelier::setLangue(QString langue) {
    _langue = langue;
}

int ModeleAtelier::getNoLocal() {
    return _noLocal;
}

void ModeleAtelier::setNoLocal(int noLocal) {
    _noLocal = noLocal;
}

int ModeleAtelier::getJour() {
    return _jour;
}

void ModeleAtelier::setJour(int jour) {
    _jour = jour;
}

int ModeleAtelier::getHeure() {
    return _heure;
}

void ModeleAtelier::setHeure(int heure) {
    _heure = heure;
}

int ModeleAtelier::getDuree() {
    return _duree;
}

void ModeleAtelier::setDuree(int duree) {
    _duree = duree;
}

int ModeleAtelier::getCoutAdulte() {
    return _coutAdulte;
}

void ModeleAtelier::setCoutAdulte(int cout) {
    _coutAdulte = cout;
}

int ModeleAtelier::getCoutEnfant() {
    return _coutEnfant;
}

void ModeleAtelier::setCoutEnfant(int cout) {
    _coutEnfant = cout;
}

QStringList ModeleAtelier::getTypes() {
    return _types;
}

QStringList ModeleAtelier::getExposants() {
    return _exposants;
}

QStringList ModeleAtelier::getCategories() {
    return _categories;
}

QStringList ModeleAtelier::getLocaux() {
    return _locaux;
}

QStringList ModeleAtelier::getJours() {
    return _jours;
}

QStringList ModeleAtelier::getHeures() {
    return _heures;
}

QStringList ModeleAtelier::getDurees() {
    return _durees;
}
