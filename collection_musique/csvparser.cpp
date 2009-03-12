#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <ctime>

#include "csvparser.h"

CsvParser::CsvParser(QWidget *parent) {
    _parent = parent;
}

bool CsvParser::import() {
    QString filename = QFileDialog::getOpenFileName(_parent, tr("Open File"),
        "", tr("CSV Files (*.csv *.txt)"));
    
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
        QStringList sl = s.split('|');
        if (sl.size() == 6) {
            QSqlQuery q;
            q.prepare("INSERT INTO collection (artiste, album, annee, genre, pistes, duree) VALUES (?,?,?,?,?,?)");
            q.addBindValue(QVariant(sl.at(0)));
            q.addBindValue(QVariant(sl.at(1)));
            q.addBindValue(QVariant(sl.at(2).toInt()));
            q.addBindValue(QVariant(sl.at(3)));
            q.addBindValue(QVariant(sl.at(4).toInt()));
			
			QStringList len = sl.at(5).split(":");
			if (len.size() != 3) {
				continue;
			}
			int length = len.at(0).toInt() * 3600 + len.at(1).toInt() * 60 + len.at(2).toInt();
			q.addBindValue(length);
			
            if (q.exec()) {
                count++;
            }
            #ifdef DEBUG
            else {
                qDebug() << q.lastError().text();
            }
            #endif
        } else {
            qDebug() << "CSV Import: Ligne" << line << "invalide.";
        }
        line++;
    }
    
    #ifdef DEBUG
    qDebug() << "CSV import done:" << count << "lines added in" << time(NULL) - t << "sec.";
    #endif
    _parent->setEnabled(true);
    return true;
}
