//TODO: Rendre le tout plus general

#include <QtSql>

#include "modeleatelier.h"

ModeleAtelier::ModeleAtelier() {
    /* On va chercher le no du prochain atelier */
    _query.exec("SELECT seq FROM sqlite_sequence WHERE name='p_atelier'");
    _query.first();
    _noAtel = _query.value(0).toInt() + 1;

    fillListes();
}

ModeleAtelier::ModeleAtelier(int noAtel) {
    _noAtel = noAtel;

    /* On remplit les infos */
    QString q;
    q = "SELECT titre Titre, ";
    q += "strftime('%w', dateatel) Jour, ";
    q += "strftime('%H', dateatel) Heure, ";
    q += "duree Duree, ";
    q += "nolocal Local, ";
    q += "langue Langue, ";
    q += "notype Type, ";
    q += "nocategorie Categorie, ";
    q += "nbmaximum Max, ";
    q += "coutRegulier CoutAdulte, ";
    q += "coutEtudiant CoutEnfant, ";
    q += "noExposant Exposant, ";
    q += "acetate_elec Acetate, ";
    q += "retro Retro, ";
    q += "portable Portable ";
    q += "FROM p_atelier ";
    q += "WHERE noatel = " + QString::number(_noAtel);

    _query.exec(q);
    _query.first();

    _titre = _query.value(0).toString();
    _jour = _query.value(1).toInt();
    _heure = _query.value(2).toInt();
    _duree = _query.value(3).toInt();
    _noLocal = _query.value(4).toInt();
    _langue = _query.value(5).toString();
    _noType = _query.value(6).toInt();
    _noCat = _query.value(7).toInt();
    _coutAdulte = _query.value(8).toInt();
    _coutEnfant = _query.value(9).toInt();
    _nbMax = _query.value(10).toInt();
    _noExpo = _query.value(11).toInt();
    _acetate = _query.value(12).toInt();
    _retro = _query.value(13).toInt();
    _ordi = _query.value(14).toInt();

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

bool ModeleAtelier::addAtelier() {
    QSqlQuery q;
    QString sQuery;

    //TODO: ajouter la date
    sQuery  = "UPDATE p_atelier SET ";
    sQuery += "titre=:titre, ";
//    sQuery += "dateatel=:dateatel, ";
    sQuery += "duree=:duree, ";
    sQuery += "nolocal=:nolocal, ";
    sQuery += "langue=:langue, ";
    sQuery += "notype=:notype, ";
    sQuery += "nocategorie=:nocat, ";
    sQuery += "nbmaximum=:nbmax, ";
    sQuery += "coutRegulier=:couta, ";
    sQuery += "coutEtudiant=:coute, ";
    sQuery += "noExposant=:noexpo, ";
    sQuery += "acetate_elec=:acetate, ";
    sQuery += "portable=:portable, ";
    sQuery += "retro=:retro ";
    sQuery += "WHERE noatel=:noatel";

    q.bindValue(":titre", _titre);
//    q.bindValue(":dateatel", );
    q.bindValue(":duree", _duree);
    q.bindValue(":nolocal", _noLocal);
    q.bindValue(":langue", _langue);
    q.bindValue(":notype", _noType);
    q.bindValue(":nocat", _noCat);
    q.bindValue(":nbmax", _nbMax);
    q.bindValue(":couta", _coutAdulte);
    q.bindValue(":coute", _coutEnfant);
    q.bindValue(":noexpo", _noExpo);
    q.bindValue(":acetate", _acetate);
    q.bindValue(":portable", _ordi);
    q.bindValue(":retro", _retro);
    q.bindValue(":noatel", _noAtel);

    return q.exec();
}

bool ModeleAtelier::updateAtelier() {
    QSqlQuery q;
    QString sQuery;
    bool ret;

    //TODO: ajouter la date
    sQuery  = "UPDATE p_atelier SET ";
    sQuery += "titre=:titre, ";
//    sQuery += "dateatel=:dateatel, ";
    sQuery += "duree=:duree, ";
    sQuery += "nolocal=:nolocal, ";
    sQuery += "langue=:langue, ";
    sQuery += "notype=:notype, ";
    sQuery += "nocategorie=:nocat, ";
    sQuery += "nbmaximum=:nbmax, ";
    sQuery += "coutRegulier=:couta, ";
    sQuery += "coutEtudiant=:coute, ";
    sQuery += "noExposant=:noexpo, ";
    sQuery += "acetate_elec=:acetate, ";
    sQuery += "portable=:portable, ";
    sQuery += "retro=:retro ";
    sQuery += "WHERE noatel=:noatel";

    q.prepare(sQuery);
    q.bindValue(":titre", _titre);
//    q.bindValue(":dateatel", );
    q.bindValue(":duree", _duree);
    q.bindValue(":nolocal", _noLocal);
    q.bindValue(":langue", _langue);
    q.bindValue(":notype", _noType);
    q.bindValue(":nocat", _noCat);
    q.bindValue(":nbmax", _nbMax);
    q.bindValue(":couta", _coutAdulte);
    q.bindValue(":coute", _coutEnfant);
    q.bindValue(":noexpo", _noExpo);
    q.bindValue(":acetate", _acetate);
    q.bindValue(":portable", _ordi);
    q.bindValue(":retro", _retro);
    q.bindValue(":noatel", _noAtel);

    ret = q.exec();
    if (!ret)
        qDebug() << "Fail!";
    qDebug() << _noAtel;

    return ret;
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
    return _noType;
}

void ModeleAtelier::setType(int type) {
    _noType = type;
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

int ModeleAtelier::getAcetate() {
    return _acetate;
}

void ModeleAtelier::setAcetate(int acetate) {
    _acetate = acetate;
}

int ModeleAtelier::getRetro() {
    return _retro;
}

void ModeleAtelier::setRetro(int retro) {
    _retro = retro;
}

int ModeleAtelier::getOrdi() {
    return _ordi;
}

void ModeleAtelier::setOrdi(int ordi) {
    _ordi = ordi;
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
