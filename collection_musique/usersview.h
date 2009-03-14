#ifndef __USERSVIEW_H__
#define __USERSVIEW_H__

#include <QDialog>

class QGridLayout;
class QHBoxLayout;
class QPushButton;
class QVBoxLayout;
class QWidget;

class UsersModel;
class DefaultTableView;

class UsersView : public QDialog {
    Q_OBJECT;
    
    public:
        UsersView(QWidget *parent = 0);
        ~UsersView();
    
    private:
        void init();
        
        QVBoxLayout *_centralLayout;
        //~ QGridLayout *_layUser;
        QHBoxLayout *_layBoutons;
    
        DefaultTableView *_tableView;
        UsersModel *_model;
        
        QPushButton *_btnAdd;
        QPushButton *_btnClose;
        
    private slots:
        void addUser();
};

#endif //__USERSVIEW_H__
