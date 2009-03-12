#ifndef __ALBUMMODEL_H__
#define __ALBUMMODEL_H__

#include "defaultsqlmodel.h"

class AlbumModel : public DefaultSqlModel {
    Q_OBJECT
    
    enum cols {ID, ARTIST, ALBUM, YEAR, GENRE, TRACKS, LENGTH};

    public:
        AlbumModel(QObject *parent = 0);

        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
};

#endif //__ALBUMMODEL_H__
