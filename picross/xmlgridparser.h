#ifndef __XML_GRID_PARSER_H__
#define __XML_GRID_PARSER_H__

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QString>

class XmlGridParser : public QObject {
    Q_OBJECT;
    
    private:
        QFile file;
        QDomDocument xml;
        QDomElement el_root;
        QDomElement el_grid;
        QDomElement el_title;
        QDomElement el_bestTime;
        QDomElement el_height;
        QDomElement el_width;
        
        QString title;
        int height;
        int width;
        int bestTime;
        bool *grid;
    
    public:
        XmlGridParser(QString name);
        XmlGridParser(QFile f);
        ~XmlGridParser();
    
        bool parseXml();
    
        QString getTitle();
        int getHeight();
        int getWidth();
        int getBestTime();
        bool* getGrid();
        void fillGridValues(bool *gv);
};

#endif //__XML_GRID_PARSER_H__
