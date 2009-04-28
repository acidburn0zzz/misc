#ifndef __DEFAULTTABLEVIEW_H__
#define __DEFAULTTABLEVIEW_H__

#include <QModelIndex>
#include <QTableView>

class QWidget;

class DefaultTableView : public QTableView {
    Q_OBJECT

    public:
        DefaultTableView(QWidget *parent = 0);

    public slots:
        void setSelectedIndex(const QModelIndex & index);
        void refresh();
};

#endif //__DEFAULTTABLEVIEW_H__
