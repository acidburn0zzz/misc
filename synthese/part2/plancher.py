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

import database as db
import xmlparser
import zone

TAILLE_CASE = 50

#Constantes d'options
NB_OPTS = 17
OPT_LOC, OPT_WEBZ, OPT_ELEC, OPT_MURET = range(4)
LOC_ADD, LOC_RM, WEBZ_500K, WEBZ_2M, WEBZ_5M, WEBZ_RM, ELEC_ADD, ELEC_RM, \
MUR_HG, MUR_HD, MUR_DH, MUR_DB, MUR_BD, MUR_BG, MUR_GB, MUR_GH, MUR_RM = range(NB_OPTS)

#Correspondance entre plancher.MUR_* et zone.MUR_*
VAL_MURS = {}
VAL_MURS[MUR_HG] = zone.MUR_HG
VAL_MURS[MUR_HD] = zone.MUR_HD
VAL_MURS[MUR_DH] = zone.MUR_DH
VAL_MURS[MUR_DB] = zone.MUR_DB
VAL_MURS[MUR_BD] = zone.MUR_BD
VAL_MURS[MUR_BG] = zone.MUR_BG
VAL_MURS[MUR_GB] = zone.MUR_GB
VAL_MURS[MUR_GH] = zone.MUR_GH

class VuePlancher(QMainWindow):
    def __init__(self, id, parent=None):
        super(VuePlancher, self).__init__(parent)
        self.idExposant = id
        
        self.setWindowTitle("Plancher")
        self.wdCentral = QWidget()
        self.setCentralWidget(self.wdCentral)
        self.layCentral = QVBoxLayout(self.wdCentral)
        
        self.lblExposant = QLabel(self.getExposant(), self)
        self.layCentral.addWidget(self.lblExposant)
        
        #Choix de categorie
        self.radLoc = QRadioButton("Location")
        self.radLoc.setChecked(True)
        self.radWebz = QRadioButton("Internet")
        self.radElectricite = QRadioButton("Prises electriques")
        self.radMurets = QRadioButton("Murets")
        
        self.grpCat = QButtonGroup(self)
        self.grpCat.addButton(self.radLoc, OPT_LOC)
        self.grpCat.addButton(self.radWebz, OPT_WEBZ)
        self.grpCat.addButton(self.radElectricite, OPT_ELEC)
        self.grpCat.addButton(self.radMurets, OPT_MURET)
        
        self.layCat = QHBoxLayout()
        self.layCat.setAlignment(Qt.AlignLeft)
        self.layCentral.addLayout(self.layCat)
        self.layCat.addWidget(self.radLoc)
        self.layCat.addWidget(self.radWebz)
        self.layCat.addWidget(self.radElectricite)
        self.layCat.addWidget(self.radMurets)
        
        self.connect(self.grpCat, SIGNAL('buttonClicked(int)'), self.changerCategorie)
        
        #Options
        self.grpOptions = QButtonGroup()
        self.layOptions = QHBoxLayout()
        self.layOptions.setAlignment(Qt.AlignLeft)
        self.layCentral.addLayout(self.layOptions)
        self.radOptions = []
        
        self.connect(self.grpOptions, SIGNAL('buttonClicked(int)'), self.changerOption)
        
        #Options de location
        self.radOptions.append(QRadioButton("Louer"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        #Options de webz
        self.radOptions.append(QRadioButton("500 kbps"))
        self.radOptions.append(QRadioButton("2 Mbps"))
        self.radOptions.append(QRadioButton("5 Mbps"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        #Options d'electricite
        self.radOptions.append(QRadioButton("Ajouter"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        #Options de murets
        #~ self.radOptMurHG = QRadioButton("Haut-Gauche")
        #~ self.radOptMurHD = QRadioButton("Haut-Droite")
        #~ self.radOptMurDH = QRadioButton("Droite-Haut")
        #~ self.radOptMurDB = QRadioButton("Droite-Bas")
        #~ self.radOptMurBD = QRadioButton("Bas-Droit")
        #~ self.radOptMurBG = QRadioButton("Bas-Gauche")
        #~ self.radOptMurGB = QRadioButton("Gauche-Bas")
        #~ self.radOptMurGH = QRadioButton("Gauche-Haut")
        self.radOptions.append(QRadioButton("H-G"))
        self.radOptions.append(QRadioButton("H-D"))
        self.radOptions.append(QRadioButton("D-H"))
        self.radOptions.append(QRadioButton("D-B"))
        self.radOptions.append(QRadioButton("B-D"))
        self.radOptions.append(QRadioButton("B-G"))
        self.radOptions.append(QRadioButton("G-B"))
        self.radOptions.append(QRadioButton("G-H"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        for i in range(NB_OPTS):
            self.grpOptions.addButton(self.radOptions[i], i)
            self.layOptions.addWidget(self.radOptions[i])
        
        self.wdPlancher = WidgetPlancher(id, self)
        self.layCentral.addWidget(self.wdPlancher)
        
        self.creerMenus()
        self.radLoc.click()
    
    def getExposant(self):
        cie = db.getValeur("exposants", "nom", self.idExposant).toString()
        nom = db.getValeur("exposants", "resp_nom", self.idExposant).toString()
        prenom = db.getValeur("exposants", "resp_prenom", self.idExposant).toString()
        
        return prenom + " " + nom + " (" + cie + ")"
    
    def creerMenus(self):
        self.actQuitter = QAction("&Quitter", self)
        self.actQuitter.setShortcut("Ctrl+Q");
        self.actQuitter.setStatusTip("Quitter le logiciel");
        self.connect(self.actQuitter, SIGNAL('triggered()'), qApp, SLOT('quit()'));
        
        self.mnuFile = QMenu("&Fichier", self)
        self.mnuFile.addAction(self.actQuitter)
        
        self.menuBar().addMenu(self.mnuFile)
    
    def changerCategorie(self, id):
        for i in range(NB_OPTS):
            self.radOptions[i].setVisible(False)
        
        if (id == OPT_LOC):
            self.radOptions[LOC_ADD].setVisible(True)
            self.radOptions[LOC_ADD].click()
            self.radOptions[LOC_RM].setVisible(True)
        elif (id == OPT_WEBZ):
            self.radOptions[WEBZ_500K].setVisible(True)
            self.radOptions[WEBZ_500K].click()
            self.radOptions[WEBZ_2M].setVisible(True)
            self.radOptions[WEBZ_5M].setVisible(True)
            self.radOptions[WEBZ_RM].setVisible(True)
        elif (id == OPT_ELEC):
            self.radOptions[ELEC_ADD].setVisible(True)
            self.radOptions[ELEC_ADD].click()
            self.radOptions[ELEC_RM].setVisible(True)
        elif (id == OPT_MURET):
            for i in range(MUR_HG, MUR_RM + 1):
                self.radOptions[i].setVisible(True)
            self.radOptions[MUR_HG].click()
    
    def changerOption(self, id):
        self.wdPlancher.setOption(id)

class ModelePlancher(object):
    def __init__(self):
        self.xml = xmlparser.XmlParser('expo.xml')
        
        self.zones = []
        for i in range(self.getLargeur()):
            tmp = []
            for j in range(self.getHauteur()):
                z = zone.Zone(self.xml.isZone(j, i))
                tmp.append(z)
            self.zones.append(tmp)
    
    def getLargeur(self):
        return self.xml.getLargeur()
        
    def getHauteur(self):
        return self.xml.getHauteur()
    
    def isZone(self, x, y):
        return self.zones[x][y].isZone()

class WidgetPlancher(QWidget):
    def __init__(self, id = 0, parent=None):
        super(WidgetPlancher, self).__init__(parent)
        self.model = ModelePlancher()
        self.id = id
        self.option = -1
        self.currentZone = [0, 0]
    
    def updateZone(self, x, y):
        if not self.model.zones[x][y].isZone():
            return
        
        self.currentZone = [x, y]
        
        #TEMPORAIRE, FAIRE PASSER PAR LE MODELE#
        #VERIF SI DEJA OCCUPEE#
        if (self.option == LOC_ADD):
            self.model.zones[x][y].setProprio(self.id)
        elif (self.option == LOC_RM):
            self.model.zones[x][y].setProprio(-1)
        elif (self.option == WEBZ_500K):
            self.model.zones[x][y].setInterwebz(zone.WEBZ_500K)
        elif (self.option == WEBZ_2M):
            self.model.zones[x][y].setInterwebz(zone.WEBZ_2M)
        elif (self.option == WEBZ_5M):
            self.model.zones[x][y].setInterwebz(zone.WEBZ_5M)
        elif (self.option == WEBZ_RM):
            self.model.zones[x][y].setInterwebz(zone.WEBZ_RM)
        elif (self.option == ELEC_ADD):
            if not self.model.zones[x][y].addPriseElectrique():
                QMessageBox.warning(self, 'Erreur', "Impossible d'ajouter une prise electrique")
        elif (self.option == ELEC_RM):
            if not self.model.zones[x][y].rmPriseElectrique():
                QMessageBox.warning(self, 'Erreur', "Il n'y a pas de prise a enlever")
        elif (self.option >= MUR_HG and self.option <= MUR_GH):
            if not self.model.zones[x][y].addMuret(VAL_MURS[self.option]):
                QMessageBox.warning(self, 'Erreur', "Impossible d'ajouter un muret")
        elif (self.option == MUR_RM):
            self.model.zones[x][y].viderMurets()
            
        self.model.zones[x][y].getMurets()
        
        self.update(QRect(x*TAILLE_CASE, y*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE))
    
    def setOption(self, id):
        self.option = id
    
    def afficherInfo(self, x, y):
        QMessageBox.information(self, 'Information', "Zone %d:%d" % (x, y))
        
    def mousePressEvent(self, event):
        x = event.x() / TAILLE_CASE
        y = event.y() / TAILLE_CASE
        
        if (event.button() == Qt.LeftButton):
            self.updateZone(x, y)
        elif (event.button() == Qt.RightButton):
            self.afficherInfo(x, y)
    
    def paintEvent(self, event):
        #On redessinne le widget au complet
        if (event.rect().width() > TAILLE_CASE and event.rect().width() > TAILLE_CASE):
            for i in range(self.model.getLargeur()):
                for j in range(self.model.getHauteur()):
                    self.paintZone(i, j)
        #On redessinne seulement la case necessaire
        else:
            x, y = self.currentZone
            self.paintZone(x, y)
    
    def paintZone(self, x, y,):
        painter = QPainter(self)
        rectZone = QRect(x*TAILLE_CASE, y*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE)
        
        if not self.model.zones[x][y].isZone():
            painter.setBrush(QBrush(Qt.transparent))
            painter.drawRect(rectZone)
            return
        
        #~ #Couleur de fond d'une case
        if (self.model.zones[x][y].getProprio() == self.id):
            painter.setBrush(QBrush(QColor(0, 0, 128)))
        elif (self.model.zones[x][y].getProprio() > 0):
            painter.setBrush(QBrush(QColor(128, 0, 0)))
        else:
            painter.setBrush(QBrush(QColor(0, 128, 0)))
        painter.drawRect(rectZone)
        
        #Dessin des murets
        murets = self.model.zones[x][y].getMurets()
        painter.setBrush(QBrush(QColor(0, 0, 0)))
        painter.setPen(QPen(Qt.NoPen))
        
        if (murets & zone.MUR_HG):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE, 25, 5)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_HD):
            rectMur = QRect(x*TAILLE_CASE+25, y*TAILLE_CASE, 25, 5)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_DH):
            rectMur = QRect(x*TAILLE_CASE+45, y*TAILLE_CASE, 5, 25)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_DB):
            rectMur = QRect(x*TAILLE_CASE+45, y*TAILLE_CASE+25, 5, 25)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_BD):
            rectMur = QRect(x*TAILLE_CASE+25, y*TAILLE_CASE+45, 25, 5)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_BG):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE+45, 25, 5)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_GB):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE+25, 5, 25)
            painter.drawRect(rectMur)
        if (murets & zone.MUR_GH):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE, 5, 25)
            painter.drawRect(rectMur)
    
    def sizeHint(self):
        return QSize(TAILLE_CASE * self.model.getLargeur()+1, TAILLE_CASE * self.model.getHauteur()+1)

if __name__ == '__main__':
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    z = VuePlancher(100)
    z.show()
    app.exec_()
    qdb.closeSqlConnection()
