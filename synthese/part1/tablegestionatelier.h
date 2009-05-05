#ifndef __TABLEGESTIONATELIER_H__
#define __TABLEGESTIONATELIER_H__

#include "defaulttableview.h"

class QWidget;

class TableGestionAtelier : public DefaultTableView {
    Q_OBJECT

    public:
        TableGestionAtelier(QWidget *parent = 0);
};

#endif //__TABLEGESTIONATELIER_H__
