/********
AJOUTER
support acetate, retro et ordi
********/

#ifndef __MODELEATELIER_H__
#define __MODELEATELIER_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSqlQuery>

class ModeleAtelier : public QObject {
    Q_OBJECT;
    
    public:
    ModeleAtelier();
    ModeleAtelier(int noAtel);
    //~ ~ModeleAtelier();
    
    int     getNoAtel();
    QString getTitre();
    int     getType();
    int     getNoExpo();
    int     getNoCat();
    int     getNbMax();
    QString getLangue();
    int     getNoLocal();
    int     getJour();
    int     getHeure();
    int     getDuree();
    int     getCoutAdulte();
    int     getCoutEnfant();
    
    void setNoAtel(int noAtel);
    void setTitre(QString titre);
    void setType(int type);
    void setNoExpo(int noExpo);
    void setNoCat(int noCat);
    void setNbMax(int nbMax);
    void setLangue(QString langue);
    void setNoLocal(int noLocal);
    void setJour(int jour);
    void setHeure(int heure);
    void setDuree(int duree);
    void setCoutAdulte(int cout);
    void setCoutEnfant(int cout);
    
    QStringList getTypes();
    QStringList getExposants();
    QStringList getCategories();
    QStringList getLocaux();
    QStringList getJours();
    QStringList getHeures();
    QStringList getDurees();
    
    private:
    void fillListes();
    
    QSqlQuery _query;
    
    int     _noAtel;
    QString _titre;
    int     _type;
    int     _noExpo;
    int     _noCat;
    int     _nbMax;
    QString _langue;
    int     _noLocal;
    int     _jour;
    int     _heure;
    int     _duree;
    int     _coutAdulte;
    int     _coutEnfant;
    
    QStringList _types;
    QStringList _exposants;
    QStringList _categories;
    QStringList _locaux;
    QStringList _jours;
    QStringList _heures;
    QStringList _durees;
};

#endif //__MODELEATELIER_H__
