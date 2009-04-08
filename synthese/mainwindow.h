#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>

class QGridLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT;
    
    public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    private:
    void init();
    
    QPushButton *_btnAtelier;
    QPushButton *_btnExposant;
    QPushButton *_btnCategorie;
    QPushButton *_btnEcole;
    QPushButton *_btnAuditeurs;
    
    QLabel *_lblTitre;
    
    QWidget *_wdCentral;
    
    QVBoxLayout *_layCentral;
    QHBoxLayout *_layBoutons;
    
    private slots:
    void inDev();
};

#endif //__MAINWINDOW_H__
