#! /usr/bin/env python
#-*- coding: utf-8 -*-

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
 ' Copyright (C) 2009 Lemay, Mathieu                                       '
 '                                                                         '
 ' This program is free software; you can redistribute it and/or modify    '
 ' it under the terms of the GNU General Public License as published by    '
 ' the Free Software Foundation; either version 2 of the License, or       '
 ' (at your option) any later version.                                     '
 '                                                                         '
 ' This program is distributed in the hope that it will be useful,         '
 ' but WITHOUT ANY WARRANTY; without even the implied warranty of          '
 ' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            '
 ' GNU General Public License for more details.                            '
 '                                                                         '
 ' You should have received a copy of the GNU General Public License along '
 ' with this program; if not, write to the Free Software Foundation, Inc., '
 ' 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             '
 '                                                                         '
 ' You can contact the original author at acidrain1@gmail.com              '
 '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSql import *
import re
import sys

import ajoutalbum
import database as db
#~ import customsqlmodel as csm

#Raccourcis
tr = QObject.tr

class VueAlbums(QMainWindow):
    def __init__(self, parent=None):
        super(VueAlbums, self).__init__(parent)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.setWindowTitle('Collection de musique')
        #~ self.setMinimumSize(800, 600)
        
        self.centralLayout = QVBoxLayout()
        
        #Ajout des boutons
        self.btnAll = QPushButton(tr(self, "Tout"))
        self.btnNbAlbums = QPushButton(tr(self, "Nb. Albums"))
        self.btnAdd = QPushButton(tr(self, "Ajout"))
        self.btnQuit = QPushButton(tr(self, "Quitter"))
        self.connect(self.btnAll, SIGNAL('clicked()'), self.showAll)
        self.connect(self.btnNbAlbums, SIGNAL('clicked()'), self.showNbAlbums)
        self.connect(self.btnAdd, SIGNAL('clicked()'), self.ajout)
        self.connect(self.btnQuit, SIGNAL('clicked()'), self, SLOT('close()'))
        
        self.layBoutons = QHBoxLayout()
        self.layBoutons.addWidget(self.btnAll)
        #~ self.layBoutons.addWidget(self.btnNbAlbums)
        self.layBoutons.addWidget(self.btnAdd)
        self.layBoutons.addWidget(self.btnQuit)
        
        self.centralLayout.addLayout(self.layBoutons)
        
        #Query initial
        self.query = "SELECT id, artist, album, year, genre, tracks, length FROM albums ORDER BY artist"
        
        #Setup du modele de la table (fixe)
        self.model = ModeleAlbums()
        self.model.setQuery(self.query)
        column_names = self.model.getColumnNames()
        for i in range(len(column_names)):
            self.model.setHeaderData(i, Qt.Horizontal, QVariant(column_names[i]))
        
        #Setup de la table (fixe)
        self.tabView = TableAlbums()
        self.tabView.setModel(self.model)
        self.tabView.setColumnHidden(0, True) #Pas besoin d'afficher l'Id
        self.tabView.resizeColumnsToContents()
        self.centralLayout.addWidget(self.tabView)
        
        self.centralWidget.setLayout(self.centralLayout)

    def showAll(self):
        self.query = "SELECT id, artist, album, year, genre, tracks, length FROM albums ORDER BY artist"
        self.model.setQuery(self.query)

    def showNbAlbums(self):
        pass
        #~ self.query = "SELECT id, artiste, count(*), annee, genre, sum(pistes), time(sum(duree), 'unixepoch') FROM collection GROUP BY artiste ORDER BY artiste"
        #~ self.model.setQuery(self.query)
    
    def ajout(self):
        pass
        f = ajoutalbum.AjoutAlbum()
        f.exec_()
        self.model.refresh()
        #~ self.query = "SELECT id, artiste, album, annee, genre, pistes, time(duree, 'unixepoch') FROM collection ORDER BY artiste"
        #~ self.model.setQuery(self.query)

class ModeleAlbums(QSqlQueryModel):
    def __init__(self, parent=None):
        super(ModeleAlbums, self).__init__(parent)
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
        return q.exec_()
    
    def getColumnNames(self):
        return self.column_names
    
    def parseTime(self, str):
        #Ameliorer pour prendre en charge une regex type "^([0-9]{1,2}:)?[0-9]{1,2}:[0-9]{1,2}$ ou un nombre de secondes"
        
        if (len(str) == 5):
            return '00:' + str
        else:
            return str
    
    def flags(self, index):
        return QSqlQueryModel.flags(self, index) | Qt.ItemIsEditable

class TableAlbums(QTableView):
    def __init__(self, parent=None):
        super(TableAlbums, self).__init__(parent)
    
    def sizeHint(self):
        return QSize(750, 600)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    qdb = db.Database()
    qdb.openSqlConnection('QSQLITE', 'albums.db')
    v = VueAlbums()
    v.show()
    app.exec_()
    qdb.closeSqlConnection()