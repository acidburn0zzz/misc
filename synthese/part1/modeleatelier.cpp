#include <QtSql>
#include <ctime>
#include <cstdio>

#include "modeleatelier.h"

ModeleAtelier::ModeleAtelier(int id) : ModeleAjoutUpdate() {
    _tableName = "p_atelier";
    _tableId = "noAtel";

    fillListes();

    if (id != -1)
        init(id);
    else
        init();
}

void ModeleAtelier::fillListes() {
    /* Liste de valeurs unique a cette table */
    _columnTypes.push_back("str");    _columnNames.push_back("titre");
    _columnTypes.push_back("int");    _columnNames.push_back("notype");
    _columnTypes.push_back("int");    _columnNames.push_back("noexposant");
    _columnTypes.push_back("int");    _columnNames.push_back("nocategorie");
    _columnTypes.push_back("int");    _columnNames.push_back("nbmaximum");
    _columnTypes.push_back("int");    _columnNames.push_back("nolocal");
//    _columnTypes.push_back("int");    _columnNames.push_back("strftime('%w', dateatel)");
//    _columnTypes.push_back("int");    _columnNames.push_back("strftime('%H', dateatel)");
    _columnTypes.push_back("str");    _columnNames.push_back("dateatel");
    _columnTypes.push_back("int");    _columnNames.push_back("duree");
    _columnTypes.push_back("int");    _columnNames.push_back("coutregulier");
    _columnTypes.push_back("int");    _columnNames.push_back("coutetudiant");
    _columnTypes.push_back("str");    _columnNames.push_back("langue");
    _columnTypes.push_back("int");    _columnNames.push_back("acetate_elec");
    _columnTypes.push_back("int");    _columnNames.push_back("retro");
    _columnTypes.push_back("int");    _columnNames.push_back("portable");

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
    _jours << tr("Vendredi") << tr("Samedi") << tr("Dimanche");

    /* Heures */
    for (int i=10; i<=15; i++)
        _heures.append(QString::number(i) + "h00");

    /* Durees */
    _durees << "30" << "45" << "60" << "90";
}

void ModeleAtelier::parseDate() {
    int yy, mm, dd, hour, min, sec;
    struct tm *s_time;
    time_t rawtime;
    QString date = _values[DATE].toString();

    sscanf(date.toStdString().c_str(), "%d-%d-%d %d:%d:%d", &yy, &mm, &dd, &hour, &min, &sec);

    time(&rawtime);
//    s_time = localtime(&rawtime);
    s_time = gmtime(&rawtime);

    s_time->tm_year = yy-1900;
    s_time->tm_mon = mm-1;
    s_time->tm_mday = dd;
    s_time->tm_hour = hour;
    s_time->tm_min = min;
    s_time->tm_sec = sec;

    /* Genere la journee de la semaine */
    mktime(s_time);

    _heure = hour; /* Pour une raison inconnue, l'heure n'est pas toujours valide dans s_time */
    _jour = s_time->tm_wday;
}

QString ModeleAtelier::genererDate(int heure, int jour) {
    //Mise a jour des attributs du modele
    _heure = heure;
    switch (jour) {
        case 0: //vendredi
            _jour = 5;
            break;
        case 1: //samedi
            _jour = 6;
            break;
        case 2: //dimanche
            _jour = 0;
            break;
    }
    //Genere une date bidon avec pour reference
    //vendredi = 01/05/2009
    char date[255];
    sprintf(date, "2009-05-%.2d %.2d:00:00", 1 + jour, heure);

    return QString(date);
}

int ModeleAtelier::getHeure() {
    return _heure;
}

int ModeleAtelier::getJour() {
    return _jour;
}

bool ModeleAtelier::capaciteValide() {
    int capLocal;
    _query.exec("SELECT capacite FROM p_local WHERE idlocal=" + _values[NOLOCAL].toString());
    _query.first();

    if (_query.isValid()) {
        capLocal = _query.value(0).toInt();
    }

    //Verification de la capacite du local
    if (capLocal < _values[NBMAXIMUM].toInt())
        return false;
    else
        return true;
}

bool ModeleAtelier::nbMaxValide() {
    //Verification du NbMax en fonction des inscriptions
    _query.exec("SELECT count(*) FROM p_inscription WHERE noatel=" + QString::number(_id));
    _query.first();

    if (_values[NBMAXIMUM].toInt() < _query.value(0).toInt())
        return false;
    else
        return true;
}

bool ModeleAtelier::titreExisteDeja() {
    _query.exec("SELECT count(*) FROM p_atelier WHERE titre LIKE '" + _values[TITRE].toString() + "'");
    _query.first();

    /* Pour une raison etrange, il ajoute 1 au count */
    if (_query.value(0).toInt() - 1 > 0)
        return true;
    else
        return false;
}

bool ModeleAtelier::conflitHoraire() {
    double hrDebutA, hrFinA, hrDebutB, hrFinB;

    hrDebutA = (double)_heure;
    hrFinA = hrDebutA + _values[DUREE].toDouble() / 60.0;

    //On va chercher tous les autre ateliers dans ce local la meme journee
    QString sQuery = "SELECT strftime('%H', dateatel), duree FROM p_atelier WHERE nolocal=" + _values[NOLOCAL].toString() +
        " AND strftime('%w', dateatel)='" + QString::number(_jour) + "' AND noatel!=" + QString::number(_id);

    _query.exec(sQuery);
    _query.first();

    while (_query.isValid()) {
        hrDebutB = _query.value(0).toDouble();
        hrFinB = hrDebutB + _query.value(1).toDouble() / 60.0;

        if ((hrDebutA >= hrDebutB && hrDebutA < hrFinB) || (hrDebutB >= hrDebutA && hrDebutB < hrFinA))
            return true;
        _query.next();
    }

    return false;
}

bool ModeleAtelier::termineTropTard() {
    if ((_heure + _values[DUREE].toInt() / 60.0) > 16.0)
        return true;
    else
        return false;
}

bool ModeleAtelier::dejaDesInscriptions() {
    _query.exec("SELECT count(*) FROM p_inscription WHERE noatel=" + QString::number(_id));
    _query.first();

    if ( _query.value(0).toInt() > 0)
        return true;
    else
        return false;
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
