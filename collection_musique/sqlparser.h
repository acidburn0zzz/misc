#ifndef __SQLPARSER_H__
#define __SQLPARSER_H__

#include <QObject>

class QWidget;

class SqlParser : public QObject {
    Q_OBJECT;
    
    public:
        SqlParser(QWidget *parent = 0);
        bool import();
    
    private:
        QWidget *_parent;
};

#endif //__SQLPARSER_H__
