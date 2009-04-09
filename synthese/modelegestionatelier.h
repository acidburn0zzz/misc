#ifndef __MODELEGESTIONATELIER_H__
#define __MODELEGESTIONATELIER_H__

#include "defaultsqlmodel.h"

class ModeleGestionAtelier : public DefaultSqlModel {
    Q_OBJECT
    
    //~ enum cols {ID, ARTIST, ALBUM, YEAR, GENRE, TRACKS, LENGTH};

    public:
    ModeleGestionAtelier(QObject *parent = 0);

    void init();
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
};

#endif //__MODELEGESTIONATELIER_H__
