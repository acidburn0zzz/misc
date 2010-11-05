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

from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtSql import *
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

        self.centralLayout = QVBoxLayout()

        #Menus
        self.mnuFile = QMenu("&Fichier", self)

        #~ self.actImport = QAction("&Importer", self)
        #~ self.actImport.setShortcut("Ctrl+I");
        #~ self.actImport.setStatusTip("Importer un fichier SQL");
        #~ self.connect(self.actImport, SIGNAL('triggered()'), self.importSql)
        #~ self.mnuFile.addAction(self.actImport)

        self.actExport = QAction("&Exporter", self)
        self.actExport.setShortcut("Ctrl+S");
        self.actExport.setStatusTip("Exporter un fichier SQL");
        self.connect(self.actExport, SIGNAL('triggered()'), self.exportSql)
        self.mnuFile.addAction(self.actExport)

        self.actQuitter = QAction("&Quitter", self)
        self.actQuitter.setShortcut("Ctrl+Q");
        self.actQuitter.setStatusTip("Quitter le logiciel");
        self.connect(self.actQuitter, SIGNAL('triggered()'), quit)
        self.mnuFile.addAction(self.actQuitter)

        self.menuBar().addMenu(self.mnuFile)

        #Textbox de recherche
        self.txtSearch = QLineEdit()
        self.txtSearch.setMaximumWidth(200)
        self.connect(self.txtSearch, SIGNAL('cursorPositionChanged(int, int)'), self.updateSearch)

        #Ajout des boutons
        self.btnAll = QPushButton(tr(self, "Tout"))
        self.btnNbAlbums = QPushButton(tr(self, "Nb. Albums"))
        self.btnAdd = QPushButton(tr(self, "Ajout"))
        #~ self.btnQuit = QPushButton(tr(self, "Quitter"))
        self.connect(self.btnAll, SIGNAL('clicked()'), self.txtSearch, SLOT('clear()'))
        self.connect(self.btnAll, SIGNAL('clicked()'), self.refresh)
        self.connect(self.btnNbAlbums, SIGNAL('clicked()'), self.showNbAlbums)
        self.connect(self.btnAdd, SIGNAL('clicked()'), self.ajout)

        #~ self.connect(self.btnQuit, SIGNAL('clicked()'), self, SLOT('close()'))

        self.layBoutons = QHBoxLayout()
        self.layBoutons.addWidget(self.txtSearch)
        self.layBoutons.addWidget(self.btnAll)
        self.layBoutons.addWidget(self.btnNbAlbums)
        self.layBoutons.addWidget(self.btnAdd)
        #~ self.layBoutons.addWidget(self.btnQuit)

        self.centralLayout.addLayout(self.layBoutons)

        #Setup du modele de la table (fixe)
        self.model = ModeleAlbums()
        self.refresh()
        #~ self.model.sort(1, Qt.AscendingOrder)

        column_names = self.model.getColumnNames()
        for i in range(len(column_names)):
            self.model.setHeaderData(i, Qt.Horizontal, column_names[i])

        #Setup de la table (fixe)
        self.tabView = TableAlbums()
        self.tabView.setModel(self.model)
        self.tabView.setColumnHidden(0, True) #Pas besoin d'afficher l'Id
        self.tabView.resizeColumnsToContents()
        self.centralLayout.addWidget(self.tabView)

        self.centralWidget.setLayout(self.centralLayout)

    def refresh(self):
        #~ self.query = "SELECT id, artist, album, year, genre, tracks, length FROM albums"
        #~ self.model.setCurrentQuery(self.query)
        #~ self.model.setCurrentQuery = "SELECT id, artist, album, year, genre, tracks, length FROM albums"
        self.model.refresh()

    def showNbAlbums(self):
        self.query = "SELECT id, artist, count(*), year, genre, sum(tracks), time(sum(strftime('%s', length)), 'unixepoch') FROM albums GROUP BY artist ORDER BY artist"
        self.model.setCurrentQuery(self.query)

    def ajout(self):
        f = ajoutalbum.AjoutAlbum()
        f.exec_()
        self.refresh()

    def updateSearch(self):
        self.model.setSearch(self.txtSearch.text())
        self.refresh()

    def importSql(self):
        filter = QStringList()
        filter.append("SQL Files (*.sql)")
        filter.append("Any Files (*)")

        chooser = QFileDialog(self)
        chooser.setAcceptMode(QFileDialog.AcceptOpen)
        chooser.setFileMode(QFileDialog.ExistingFile)
        chooser.setNameFilters(filter)
        if chooser.exec_():
            file = chooser.selectedFiles()[0]
            #~ self.model.importSql(file)

    def exportSql(self):
        filter = QStringList()
        filter.append("SQL Files (*.sql)")
        filter.append("Any Files (*)")

        chooser = QFileDialog(self)
        chooser.setAcceptMode(QFileDialog.AcceptSave)
        chooser.setFileMode(QFileDialog.AnyFile)
        chooser.setNameFilters(filter)
        if chooser.exec_():
            file = chooser.selectedFiles()[0]
            self.model.exportSql(file)

class ModeleAlbums(QSqlQueryModel):
    def __init__(self, parent=None):
        super(ModeleAlbums, self).__init__(parent)
        self.currentQuery = ""
        self.currentSortCol = 1
        self.currentSortAsc = True
        self.columns = ['id', 'artist', 'album', 'year', 'genre', 'tracks', 'length']
        self.column_names = ['Id', 'Artist', 'Album', 'Year', 'Genre', 'Tracks', 'Length']
        self.search = ""

    def data(self, index, role):
        value = QSqlQueryModel.data(self, index, role)
        #~ if (value.isValid() and role == Qt.DisplayRole):
            #~ if (index.column() == 1):
                #~ return QVariant(value.toString().prepend("#"))
            #~ if (index.column() == 2):
                #~ return QVariant(value.toString().toUpper())

        if (role == Qt.BackgroundRole and index.row()%2 == 0):
            return QBrush(QColor(224, 224, 224))
        if (role == Qt.ForegroundRole):
            return QBrush(QColor(Qt.black))

        #Cases modifiees
        #~ if (role == Qt.FontRole):
            #~ f = QFont()
            #~ f.setItalic(True)
            #~ return QVariant(f)

        return value

    def setData(self, index, value, role):
        primaryKeyIndex = QSqlQueryModel.index(self, index.row(), 0)

        #data retourne la valeur et un bool
        id = QSqlQueryModel.data(self, primaryKeyIndex)

        QSqlQueryModel.clear(self)

        ok = self.setValue(index.column(), id, value)

        self.refresh()
        return ok

    def refresh(self):
        if (self.currentSortAsc):
            self.sort(self.currentSortCol, Qt.AscendingOrder)
        else:
            self.sort(self.currentSortCol, Qt.DescendingOrder)
        for i in range(len(self.column_names)):
            self.setHeaderData(i, Qt.Horizontal, self.column_names[i])

    def setCurrentQuery(self, q):
        self.currentQuery = q
        self.setQuery(q)

    def setValue(self, col, id, value):
        if (col == 1 or col == 2 or col == 4):  #Artist, album, genre
            pattern = "^[a-zA-Z0-9.,':!?()&\- ]+$"
        elif (col == 3):    #year
            pattern = "^[0-9]{4}$"
        elif (col == 5):    #tracks
            pattern = "^[0-9]{1,2}$"
        elif (col == 6):    #length
            pattern = "^([0-9]{1,2}[:-])?[0-9]{1,2}[:-][0-9]{1,2}$"

        m = re.search(pattern, value.__str__())
        print value
        if not m:
            return False

        q = QSqlQuery()
        q.prepare("UPDATE albums SET " + self.columns[col] + " = ? WHERE id = ?")
        if (col == 6):
            q.addBindValue(db.formatterDuree(value))
        else:
            q.addBindValue(value)
        q.addBindValue(id)
        return q.exec_()

    def sort(self, column, order):
        sQuery = "SELECT id, artist, album, year, genre, tracks, length FROM albums"
        if (len(self.search) > 0):
            sQuery += " WHERE artist LIKE '%" + self.search + "%'"

        sQuery += " ORDER BY " + self.columns[column];

        if (order == Qt.AscendingOrder):
            self.currentSortAsc = True
            sQuery += " ASC";
        else:
            self.currentSortAsc = False
            sQuery += " DESC";

        self.setQuery(sQuery);

    def getColumnNames(self):
        return self.column_names

    def flags(self, index):
        return QSqlQueryModel.flags(self, index) | Qt.ItemIsEditable

    def setSearch(self, search):
        self.search = search

    def exportSql(self, file):
        q = QSqlQuery()
        q.exec_("SELECT id, artist, album, year, genre, tracks, length FROM albums")
        q.first()

        out_sql = open(file, 'w')
        out_sql.write("BEGIN TRANSACTION;\n\n")
        out_sql.write("DROP TABLE IF EXISTS albums;\n")
        out_sql.write("CREATE TABLE albums (id INTEGER PRIMARY KEY, artist TEXT, album TEXT, year INTEGER, genre TEXT, tracks INTEGER, length TEXT);\n")
        out_sql.write("CREATE UNIQUE INDEX album_id ON albums (artist, album);\n\n")

        while q.isValid():
            id = q.value(0).toString()
            artist = q.value(1).toString().replace("'", "''").toUtf8()
            album = q.value(2).toString().replace("'", "''").toUtf8()
            year = q.value(3).toString()
            genre = q.value(4).toString()
            tracks = q.value(5).toString()
            length = q.value(6).toString()
            query = "INSERT INTO albums (id, artist, album, year, genre, tracks, length)" + \
                " VALUES (" + id + ",'" + artist + "', '" + album + "', " + year + ", '" + genre + \
                "', " + tracks + ", '" + length + "');\n"

            out_sql.write(query)
            q.next()

        out_sql.write("\nCOMMIT;\n")
        out_sql.close()

class TableAlbums(QTableView):
    def __init__(self, parent=None):
        super(TableAlbums, self).__init__(parent)
        self.setSortingEnabled(True);

    def sizeHint(self):
        return QSize(900, 750)

if __name__ == '__main__':
    import main
    main.run()
