#! /usr/bin/env python
#-*- coding: UTF-8 -*-

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSql import *
import sys

import ajoutalbum
import connexion
import customsqlmodel as csm

#Raccourcis
tr = QObject.tr

class Vue(QMainWindow):
    def __init__(self, cont, parent=None):
        super(Vue, self).__init__(parent)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.setWindowTitle('Collection de musique')
        self.setMinimumSize(800, 600)
        
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
        
        #Connexion a la base et query initial
        connexion.openSqlConnection('QSQLITE', 'albums.db')
        self.query = "SELECT id, artist, album, year, genre, tracks, length FROM albums ORDER BY artist"
        
        #Setup du modele de la table (fixe)
        self.model = csm.CustomSqlModel()
        self.model.setQuery(self.query)
        column_names = self.model.getColumnNames()
        for i in range(len(column_names)):
            self.model.setHeaderData(i, Qt.Horizontal, QVariant(column_names[i]))
        
        #Setup de la table (fixe)
        self.tabView = QTableView()
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

if __name__ == '__main__':
    app = QApplication(sys.argv)
    a = Vue(None)
    a.show()
    app.exec_()
