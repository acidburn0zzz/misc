#include <QApplication>
#include <QtGui>
#include <QtXml>

#include "vue_test.h"
#include "xmlgridparser.h"

#include <iostream>
using namespace std;

Form::Form(QWidget *parent) : QMainWindow(parent, Qt::Window) {
    init();
}

Form::~Form() {
    delete _lblTitre;
    delete _lblSize;
    delete _lblValues;
    
    delete _txtTitre;
    delete _txtSize;
    delete _txtValues;
    
    delete _layInfos;
    delete _centralLayout;
    delete _centralWidget;
}

void Form::init() {
    this->setWindowTitle("Test du XmlGridParser");
    
    _centralWidget = new QWidget();
    setCentralWidget(_centralWidget);
    
    _lblTitre = new QLabel(tr("Titre"));
    _lblSize = new QLabel(tr("Size"));
    _lblValues = new QLabel(tr("Values"));
    
    _txtTitre = new QLineEdit();
    _txtSize = new QLineEdit();
    _txtValues = new QLineEdit();
    
    _layInfos = new QGridLayout();
    _layInfos->addWidget(_lblTitre, 0, 0);
    _layInfos->addWidget(_txtTitre, 0, 1);
    _layInfos->addWidget(_lblSize, 1, 0);
    _layInfos->addWidget(_txtSize, 1, 1);
    _layInfos->addWidget(_lblValues, 2, 0);
    _layInfos->addWidget(_txtValues, 2, 1);
    
    //~ _centralLayout = new QVBoxLayout();
    //~ _centralLayout->addLayout(_layInfos);
    
    //~ _centralWidget->setLayout(_centralLayout);
    _centralWidget->setLayout(_layInfos);
    
    XmlGridParser *xgp = new XmlGridParser("dummy_grid.xml");
    if (!xgp->parseXml())
        exit(-1);
    
    int height = xgp->getHeight();
    int width = xgp->getWidth();
    bool *grid = xgp->getGrid();
    
    QString values("");
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            if (grid[i*width+j]) {
                values += "1 ";
            } else {
                values += "0 ";
            }
        }
    }
    
    _txtTitre->setText(xgp->getTitle());
    _txtSize->setText(QString::number(height) + "x" + QString::number(width));
    _txtValues->setText(values);
    
    this->show();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    new Form();
    
    return app.exec();
}
