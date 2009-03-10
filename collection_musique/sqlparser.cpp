#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <ctime>

#include "sqlparser.h"

SqlParser::SqlParser(QWidget *parent) {
    _parent = parent;
}

bool SqlParser::import() {
    QString filename = QFileDialog::getOpenFileName(_parent, tr("Open File"),
        "", tr("SQL Files (*.sql *.txt)"));
    
    if (filename == "") {
        return false;
    }
    
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    
    char buffer[1024];
    int line=1, count=0;
    
    #ifdef DEBUG
    time_t t = time(NULL);
    #endif
    
    /*Bloquer la fenetre tant que le query s'execute*/
    _parent->setEnabled(false);
    while (f.readLine(buffer, sizeof(buffer)) != -1) {
        QString s = QString::fromUtf8(buffer);
        
        if (s.at(0) == '#')
            continue;
        
        QSqlQuery q;
        if (q.exec(s)) {
            count++;
        }
        #ifdef DEBUG
        else {
            qDebug() << q.lastError().text();
        }
        #endif
        line++;
    }
    
    #ifdef DEBUG
    qDebug() << "SQL import done:" << count << "lines added in" << time(NULL) - t << "sec.";
    #endif
    _parent->setEnabled(true);
    return true;
}
