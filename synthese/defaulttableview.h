#ifndef __DEFAULTTABLEVIEW_H__
#define __DEFAULTTABLEVIEW_H__

#include <QTableView>

class QWidget;

class DefaultTableView : public QTableView {
    Q_OBJECT

    public:
        DefaultTableView(QWidget *parent = 0);
        
    public slots:
        void setSelectedIndex(const QModelIndex & index);
};

#endif //__DEFAULTTABLEVIEW_H__
