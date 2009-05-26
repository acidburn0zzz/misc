#! /usr/bin/env python
#-*- coding: UTF-8 -*-

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtSql import *

import database as db

import time
import sys

#Raccourcis
tr = QObject.tr

class AjoutAlbum(QDialog):
    def __init__(self, parent=None):
        super(AjoutAlbum, self).__init__(parent)
        self.setWindowTitle(tr(self, 'Add album'))
        
        #tableaux pour les artistes et genres
        self.artists = []
        self.genres = []
        self.getArtists()
        self.getGenres()
        
        #Elements de la fenetre
        self.lblArtist = QLabel(tr(self, 'Artist'))
        self.cmbArtist = QComboBox()
        self.cmbArtist.setEditable(True)
        self.lblAlbum = QLabel(tr(self, 'Album'))
        self.txtAlbum = QLineEdit()
        self.lblYear = QLabel(tr(self, 'Year'))
        self.txtYear = QLineEdit()
        self.lblGenre = QLabel(tr(self, 'Genre'))
        self.cmbGenre = QComboBox()
        self.cmbGenre.setEditable(True)
        self.lblTracks = QLabel(tr(self, 'Tracks'))
        self.txtTracks = QLineEdit()
        self.lblLength = QLabel(tr(self, 'Length'))
        self.txtLength = QLineEdit()
        
        #Validateurs
        cYear = int(time.strftime('%Y'))
        self.txtYear.setValidator(QIntValidator(1000, cYear, self))
        self.txtTracks.setValidator(QIntValidator(0, 99, self))
        self.txtLength.setValidator(QRegExpValidator(QRegExp("^([0-9]{2}:)?[0-9]{2}:[0-9]{2}$"), self))
        
        self.btnAdd = QPushButton(tr(self, 'Ajouter'))
        self.btnCancel = QPushButton(tr(self, 'Annuler'))
        self.connect(self.btnAdd, SIGNAL('clicked()'), self.save)
        self.connect(self.btnCancel, SIGNAL('clicked()'), self.cancel)
        
        #Remplissage des ComboBox
        for i in self.artists:
            self.cmbArtist.addItem(i)
        for i in self.genres:
            self.cmbGenre.addItem(i)
        
        layInfos = QGridLayout()
        layInfos.addWidget(self.lblArtist, 0, 0)
        layInfos.addWidget(self.cmbArtist, 0, 1)
        layInfos.addWidget(self.lblAlbum , 1, 0)
        layInfos.addWidget(self.txtAlbum , 1, 1)
        layInfos.addWidget(self.lblYear  , 2, 0)
        layInfos.addWidget(self.txtYear  , 2, 1)
        layInfos.addWidget(self.lblGenre , 3, 0)
        layInfos.addWidget(self.cmbGenre , 3, 1)
        layInfos.addWidget(self.lblTracks, 4, 0)
        layInfos.addWidget(self.txtTracks, 4, 1)
        layInfos.addWidget(self.lblLength, 5, 0)
        layInfos.addWidget(self.txtLength, 5, 1)
        
        layButtons = QHBoxLayout()
        layButtons.addWidget(self.btnAdd)
        layButtons.addWidget(self.btnCancel)
        
        cLayout = QVBoxLayout()
        cLayout.addLayout(layInfos)
        cLayout.addLayout(layButtons)
        
        self.setLayout(cLayout)

    def getArtists(self):
        q = QSqlQuery()
        q.exec_('SELECT DISTINCT artist FROM albums ORDER BY artist;')
        
        while (q.next()):
            self.artists.append(q.value(0).toString())
    
    def getGenres(self):
        q = QSqlQuery()
        q.exec_('SELECT DISTINCT genre FROM albums ORDER BY genre;')
        
        while (q.next()):
            self.genres.append(q.value(0).toString())
    
    def save(self):
        q = QSqlQuery()
        
        try:
            if not self.txtAlbum.hasAcceptableInput():
                raise UnacceptableInputException
            if not self.txtYear.hasAcceptableInput():
                raise UnacceptableInputException
            if not self.txtTracks.hasAcceptableInput():
                raise UnacceptableInputException
            if not self.txtLength.hasAcceptableInput():
                raise UnacceptableInputException
        except UnacceptableInputException:
            print tr(self, "Erreur: Les cases sont mal remplies")
        except:
            print tr(self, "Erreur inconnue")
        else:
            duree = self.txtLength.text()
            if (len(duree) == 5):
                duree = '00:' + duree
            
            q.prepare("INSERT INTO albums (artist, album, year, genre, tracks, length) " + \
                "VALUES (?,?,?,?,?,?)")
            q.addBindValue(QVariant(self.cmbArtist.currentText()))
            q.addBindValue(QVariant(self.txtAlbum.text()))
            q.addBindValue(QVariant(self.txtYear.text()))
            q.addBindValue(QVariant(self.cmbGenre.currentText()))
            q.addBindValue(QVariant(self.txtTracks.text()))
            q.addBindValue(QVariant(duree))
            
            q.exec_()
            self.accept()
    
    def cancel(self):
        self.reject()

class UnacceptableInputException(Exception):
    pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    qdb = db.Database()
    qdb.openSqlConnection('QSQLITE', 'albums.db')
    a = AjoutAlbum()
    a.show()
    app.exec_()
    qdb.closeSqlConnection()
    