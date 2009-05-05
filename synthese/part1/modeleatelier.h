#ifndef __MODELEATELIER_H__
#define __MODELEATELIER_H__

#include <QStringList>

#include "modeleajoutupdate.h"

class ModeleAtelier : public ModeleAjoutUpdate {
    Q_OBJECT;

    public:
        ModeleAtelier(int id = -1);

        void parseDate();
        QString genererDate(int heure, int jour);
        int getHeure();
        int getJour();

        /*Verification par rapport a l'ajout ou modif*/
        bool capaciteValide();
        bool nbMaxValide();
        bool titreExisteDeja();
        bool conflitHoraire();
        bool termineTropTard();
        bool dejaDesInscriptions();

        QStringList getTypes();
        QStringList getExposants();
        QStringList getCategories();
        QStringList getLocaux();
        QStringList getJours();
        QStringList getHeures();
        QStringList getDurees();

        /* Ordre des colonnes */
        enum cols {TITRE, NOTYPE, NOEXPOSANT, NOCATEGORIE, NBMAXIMUM, NOLOCAL,
            DATE, DUREE, COUT_A, COUT_E, LANGUE, ACETATE, RETRO, PORTABLE};

    private:
        void fillListes();

        QStringList _types;
        QStringList _exposants;
        QStringList _categories;
        QStringList _locaux;
        QStringList _jours;
        QStringList _heures;
        QStringList _durees;

        int _heure;
        int _jour;
};

#endif //__MODELEATELIER_H__
