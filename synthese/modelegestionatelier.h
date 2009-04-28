#ifndef __MODELEGESTIONATELIER_H__
#define __MODELEGESTIONATELIER_H__

#include "defaultsqlmodel.h"

#include <QStringList>

class ModeleGestionAtelier : public DefaultSqlModel {
    Q_OBJECT
    
    enum cols {NOATEL, TITRE, JOUR, HEURE, LOCAL, LANGUE, TYPE, CAT, DISPO};

    public:
    ModeleGestionAtelier(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    
    private:
    QStringList _listeJours;
};

#endif //__MODELEGESTIONATELIER_H__
