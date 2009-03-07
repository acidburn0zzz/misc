#ifndef __FORM_H__
#define __FORM_H__

#include <QMainWindow>

class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QWidget;

class Form : public QMainWindow {
    Q_OBJECT;
    
    public:
        Form(QWidget *parent=0);
        ~Form();
    
    private:
        void init();
        
        QWidget *_centralWidget;
        QVBoxLayout *_centralLayout;
        QGridLayout *_layInfos;
        
        QLabel *_lblTitre;
        QLabel *_lblSize;
        QLabel *_lblValues;
        
        QLineEdit *_txtTitre;
        QLineEdit *_txtSize;
        QLineEdit *_txtValues;
};

#endif //__FORM_H__
