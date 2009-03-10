#ifndef __CSVPARSER_H__
#define __CSVPARSER_H__

#include <QObject>

class QWidget;

class CsvParser : public QObject {
    Q_OBJECT;
    
    public:
        CsvParser(QWidget *parent = 0);
        bool import();
    
    private:
        QWidget *_parent;
};

#endif //__CSVPARSER_H__
