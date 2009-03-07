#ifndef __VUE_H__
#define __VUE_H__

#include <QMainWindow>

class QGridLayout;
class QHBoxLayout;
class QLabel;
class QMenu;
class QPushButton;
class QSqlQueryModel;
class QTableView;
class QVBoxLayout;
class QWidget;

class Controleur;
class CustomSqlModel;

class Vue : public QMainWindow {
    Q_OBJECT;
    
    public:
    Vue(Controleur *cont, QWidget *parent = 0);
    ~Vue();
    
    private:
    void init();
    
    QVBoxLayout *_centralLayout;
    QHBoxLayout *_layBoutons;
    
    QWidget *_centralWidget;
    
    QString _query;
    CustomSqlModel *_model;
    QTableView *_tabView;
    
    QPushButton *_btnList;
    QPushButton *_btnNbAlbums;
    QPushButton *_btnInfos;
    
    private slots:
    void showList();
    void showNbAlbums();
    void showInfos();
};

#endif //__VUE_H__
