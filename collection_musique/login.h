#ifndef __LOGIN_H__
#define __LOGIN_H__

#include <QDialog>

class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QWidget;

class Login : public QDialog {
    Q_OBJECT;
    
    public:
        Login(QWidget *parent = 0);
        ~Login();
    
    private:
        void init();
        
        QVBoxLayout *_centralLayout;
        QGridLayout *_layUser;
        QHBoxLayout *_layBoutons;
        
        QLabel *_lblUser;
        QLabel *_lblPass;
        QLineEdit *_txtUser;
        QLineEdit *_txtPass;
        
        QPushButton *_btnOk;
        QPushButton *_btnQuit;
        
    private slots:
        void checkUser();
};

#endif //__LOGIN_H__
