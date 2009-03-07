#include <QtCore>
#include <QtGui>
#include <QtXml>

#include "xmlgridparser.h"

XmlGridParser::XmlGridParser(QString name) : QObject() {
    file.setFileName(name);
}

XmlGridParser::XmlGridParser(QFile f) : QObject() {
    file.setFileName(f.fileName());
}

XmlGridParser::~XmlGridParser() {
    delete [] grid;
}

bool XmlGridParser::parseXml() {
    QString errorStr;
    int errorLine;
    int errorColumn;
    bool ok;
    
    if (!xml.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("Parse error at line %1, column %2:\n%3")
            .arg(errorLine).arg(errorColumn).arg(errorStr));
        return false;
    }
    
    el_root = xml.documentElement();
    if (el_root.tagName() != "qpicross") {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("The file is not an QPicross file."));
        return false;
    }
    
    /* A puzzle can have no title */
    el_title = el_root.firstChildElement("title");
    if (!el_title.isNull()) {
        title = el_title.text();
    } else {
        title = "Untitled";
    }
        
    el_height = el_root.firstChildElement("height");
    if (el_height.isNull()) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("There is no height element in the file: %1")
            .arg(file.fileName()));
        return false;
    }
    height = el_height.text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("Unable to get height of the grid."));
        return false;
    }
    
    el_width = el_root.firstChildElement("width");
    if (el_width.isNull()) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("There is no width element in the file: %1")
            .arg(file.fileName()));
        return false;
    }
    width = el_width.text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("Unable to get width of the grid."));
        return false;
    }
    
    el_grid = el_root.firstChildElement("grid");
    if (el_grid.isNull()) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("There is no grid element in the file: %1")
            .arg(file.fileName()));
        return false;
    }
    
    /* Ugly but does the job */
    grid = new bool[height*width];
    
    QDomElement line = el_grid.firstChildElement("line");
    int curLine = 0;
    while (!line.isNull()) {
        QDomElement cell = line.firstChildElement("cell");
        int curCell = 0;
        while (!cell.isNull()) {
            if (cell.text() == "1")
                grid[curLine*width+curCell] = true;
            else
                grid[curLine*width+curCell] = false;
            curCell++;
            cell = cell.nextSiblingElement("cell");
        }
        
        if (curCell != width) {
            QMessageBox::warning(NULL, tr("QPicross"),
                tr("The number of cells in line #%1 doesn't match the declared width.").arg(curLine));
            return false;
        }
        curLine++;
        line = line.nextSiblingElement("line");
    }
    
    if (curLine != height) {
        QMessageBox::warning(NULL, tr("QPicross"),
            tr("The number of lines in the grid doesn't match the declared height."));
        return false;
    }
    
    return true;
}

QString XmlGridParser::getTitle() {
    return title;
}

int XmlGridParser::getHeight() {
    return height;
}

int XmlGridParser::getWidth() {
    return width;
}

bool* XmlGridParser::getGrid() {
    return grid;
}

void XmlGridParser::fillGridValues(bool *gv) {
    for (int i=0; i<height*width; i++) {
        gv[i] = grid[i];
    }
}
