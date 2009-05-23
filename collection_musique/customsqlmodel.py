#! /usr/bin/env python
#-*- coding: UTF-8 -*-

from PyQt4.QtCore import *
from PyQt4.QtGui import QBrush, QColor, QFont, QApplication
from PyQt4.QtSql import *
import sys
import re

import vue

class CustomSqlModel(QSqlQueryModel):
    def __init__(self, parent=None):
        super(CustomSqlModel, self).__init__(parent)
        self.columns = ['id', 'artist', 'album', 'year', 'genre', 'tracks', 'length']
        self.column_names = ['Id', 'Artist', 'Album', 'Year', 'Genre', 'Tracks', 'Length']
    
    def data(self, index, role):
        value = QSqlQueryModel.data(self, index, role)
        #~ if (value.isValid() and role == Qt.DisplayRole):
            #~ if (index.column() == 1):
                #~ return QVariant(value.toString().prepend("#"))
            #~ if (index.column() == 2):
                #~ return QVariant(value.toString().toUpper())
        
        if (role == Qt.BackgroundRole and index.row()%2 == 0):
            return QVariant(QBrush(QColor(224, 224, 224)))
        if (role == Qt.ForegroundRole):
            return QVariant(QBrush(QColor(Qt.black)))
        
        #Cases modifiees
        #~ if (role == Qt.FontRole):
            #~ f = QFont()
            #~ f.setItalic(True)
            #~ return QVariant(f)
        
        return value
    
    def flags(self, index):
        return QSqlQueryModel.flags(self, index) | Qt.ItemIsEditable
    
    def setData(self, index, value, role):
        primaryKeyIndex = QSqlQueryModel.index(self, index.row(), 0)
        
        #data retourne la valeur et un bool
        id = QSqlQueryModel.data(self, primaryKeyIndex).toInt()[0]

        QSqlQueryModel.clear(self)
        
        ok = self.setValue(index.column(), id, value.toString())
        
        self.refresh()
        return ok

    def refresh(self):
        self.setQuery("SELECT id, artist, album, year, genre, tracks, length FROM albums ORDER BY artist")
        for i in range(len(self.column_names)):
            self.setHeaderData(i, Qt.Horizontal, QVariant(self.column_names[i]))
    
    def setValue(self, col, id, value):
        if (col == 1 or col == 2 or col == 4):  #Artist, album, genre
            pattern = "^[a-zA-Z0-9.,:!? ]+$"
        elif (col == 3):    #year
            pattern = "^[0-9]{4}$"
        elif (col == 5):    #tracks
            pattern = "^[0-9]{1,2}$"
        elif (col == 6):    #length
            pattern = "^([0-9]{2}:)?[0-9]{2}:[0-9]{2}$"
        
        m = re.search(pattern, value.__str__())
        if not m:
            return False
    
        q = QSqlQuery()
        q.prepare("UPDATE albums SET " + self.columns[col] + " = ? WHERE id = ?")
        if (col == 6):
            q.addBindValue(QVariant(self.parseTime(value)))
        else:
            q.addBindValue(QVariant(value))
        q.addBindValue(QVariant(id))
        print q.lastError().text()
        return q.exec_()
    
    def getColumnNames(self):
        return self.column_names
    
    def parseTime(self, str):
        #Ameliorer pour prendre en charge une regex type "^([0-9]{1,2}:)?[0-9]{1,2}:[0-9]{1,2}$"
        
        if (len(str) == 5):
            return '00:' + str
        else:
            return str
        
if __name__ == '__main__':
    import vue
    app = QApplication(sys.argv)
    a = vue.Vue(None)
    a.show()
    app.exec_()
