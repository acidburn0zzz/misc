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
OPT_WEBZ, OPT_ELEC, OPT_MURET = range(3)
WEBZ_RM, WEBZ_500K, WEBZ_2M, WEBZ_5M = range(4)
ELEC_RM, ELEC_ADD = range(2)
MUR_RM, MUR_HG, MUR_HD, MUR_DH, MUR_DB, MUR_BD, MUR_BG, MUR_GB, MUR_GH = range(9)

class VuePlancher(QMainWindow):
    def __init__(self, id, parent=None):
        super(VuePlancher, self).__init__(parent)
        self.idExposant = id
        
        self.setWindowTitle("Plancher")
        self.wdCentral = QWidget()
        self.setCentralWidget(self.wdCentral)
        
        self.lblExposant = QLabel(self.getExposant(), self)
        
        #Options de choix
        self.radWebz = QRadioButton("Internet")
        self.radWebz.setChecked(True)
        self.radElectricite = QRadioButton("Prises electriques")
        self.radMurets = QRadioButton("Murets")
        
        self.grpOptions = QButtonGroup(self)
        self.grpOptions.addButton(self.radWebz, OPT_WEBZ)
        self.grpOptions.addButton(self.radElectricite, OPT_ELEC)
        self.grpOptions.addButton(self.radMurets, OPT_MURET)
        
        self.layOptions = QHBoxLayout()
        self.layOptions.setAlignment(Qt.AlignLeft)
        self.layOptions.addWidget(self.radWebz)
        self.layOptions.addWidget(self.radElectricite)
        self.layOptions.addWidget(self.radMurets)
        
        self.connect(self.grpOptions, SIGNAL('buttonClicked(int)'), self.changerOptions)
        
        #Options de webz
        self.radWebzRemove = QRadioButton("Enlever")
        self.radWebzRemove.setChecked(True)
        self.radWebz500K = QRadioButton("500 kbps")
        self.radWebz2M = QRadioButton("2 Mbps")
        self.radWebz5M = QRadioButton("5 Mbps")
        
        self.grpOtpWebz = QButtonGroup(self)
        self.grpOtpWebz.addButton(self.radWebzRemove, WEBZ_RM)
        self.grpOtpWebz.addButton(self.radWebz500K, WEBZ_500K)
        self.grpOtpWebz.addButton(self.radWebz2M, WEBZ_2M)
        self.grpOtpWebz.addButton(self.radWebz5M, WEBZ_5M)
        
        self.wdWebz = QWidget()
        self.layWebz = QHBoxLayout(self.wdWebz)
        self.layWebz.setAlignment(Qt.AlignLeft)
        self.layWebz.addWidget(self.radWebzRemove)
        self.layWebz.addWidget(self.radWebz500K)
        self.layWebz.addWidget(self.radWebz2M)
        self.layWebz.addWidget(self.radWebz5M)
        
        #Options d'electricite
        self.radOptElecRm = QRadioButton("Enlever")
        self.radOptElecRm.setChecked(True)
        self.radOptElecAdd = QRadioButton("Ajouter")
        
        self.grpOtpElectricite = QButtonGroup(self)
        self.grpOtpElectricite.addButton(self.radOptElecRm, ELEC_RM)
        self.grpOtpElectricite.addButton(self.radOptElecAdd, ELEC_ADD)
        
        self.wdElec = QWidget()
        self.layElec = QHBoxLayout(self.wdElec)
        self.layElec.setAlignment(Qt.AlignLeft)
        self.layElec.addWidget(self.radOptElecRm)
        self.layElec.addWidget(self.radOptElecAdd)
        
        #Options de murets
        self.radOptMurRM = QRadioButton("Enlever")
        self.radOptMurRM.setChecked(True)
        #~ self.radOptMurHG = QRadioButton("Haut-Gauche")
        #~ self.radOptMurHD = QRadioButton("Haut-Droite")
        #~ self.radOptMurDH = QRadioButton("Droite-Haut")
        #~ self.radOptMurDB = QRadioButton("Droite-Bas")
        #~ self.radOptMurBD = QRadioButton("Bas-Droit")
        #~ self.radOptMurBG = QRadioButton("Bas-Gauche")
        #~ self.radOptMurGB = QRadioButton("Gauche-Bas")
        #~ self.radOptMurGH = QRadioButton("Gauche-Haut")
        self.radOptMurHG = QRadioButton("H-G")
        self.radOptMurHD = QRadioButton("H-D")
        self.radOptMurDH = QRadioButton("D-H")
        self.radOptMurDB = QRadioButton("D-B")
        self.radOptMurBD = QRadioButton("B-D")
        self.radOptMurBG = QRadioButton("B-G")
        self.radOptMurGB = QRadioButton("G-B")
        self.radOptMurGH = QRadioButton("G-H")
        
        self.grpOtpMurets = QButtonGroup(self)
        self.grpOtpMurets.addButton(self.radOptMurRM, MUR_RM)
        self.grpOtpMurets.addButton(self.radOptMurHG, MUR_HG)
        self.grpOtpMurets.addButton(self.radOptMurHD, MUR_HD)
        self.grpOtpMurets.addButton(self.radOptMurDH, MUR_DH)
        self.grpOtpMurets.addButton(self.radOptMurDB, MUR_DB)
        self.grpOtpMurets.addButton(self.radOptMurBD, MUR_BD)
        self.grpOtpMurets.addButton(self.radOptMurBG, MUR_BG)
        self.grpOtpMurets.addButton(self.radOptMurGB, MUR_GB)
        self.grpOtpMurets.addButton(self.radOptMurGH, MUR_GH)
        
        self.wdMuret = QWidget()
        self.layMuret = QHBoxLayout(self.wdMuret)
        self.layMuret.setAlignment(Qt.AlignLeft)
        self.layMuret.addWidget(self.radOptMurRM)
        self.layMuret.addWidget(self.radOptMurHG)
        self.layMuret.addWidget(self.radOptMurHD)
        self.layMuret.addWidget(self.radOptMurDH)
        self.layMuret.addWidget(self.radOptMurDB)
        self.layMuret.addWidget(self.radOptMurBD)
        self.layMuret.addWidget(self.radOptMurBG)
        self.layMuret.addWidget(self.radOptMurGB)
        self.layMuret.addWidget(self.radOptMurGH)
        
        #Layout central
        self.layChoix = QStackedLayout()
        self.layChoix.addWidget(self.wdWebz)
        self.layChoix.addWidget(self.wdElec)
        self.layChoix.addWidget(self.wdMuret)
        self.wdPlancher = WidgetPlancher(id, self)
        
        self.layCentral = QVBoxLayout()
        self.layCentral.addWidget(self.lblExposant)
        self.layCentral.addLayout(self.layOptions)
        self.layCentral.addLayout(self.layChoix)
        self.layCentral.addWidget(self.wdPlancher)
        self.wdCentral.setLayout(self.layCentral)
        
        self.creerMenus()
    
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
    
    def changerOptions(self, id):
        self.layChoix.setCurrentIndex(id)
        #~ if (id == OPT_WEBZ):
            #~ pass
        #~ elif (id == OPT_ELEC):
            #~ pass
        #~ elif (id == OPT_MURET):
            #~ pass

class ModelePlancher(object):
    def __init__(self):
        self.xml = xmlparser.XmlParser('expo.xml')
        
        self.zones = []
        for i in range(self.getLargeur()):
            tmp = []
            for j in range(self.getHauteur()):
                z = zone.Zone(self.isZone(j, i))
                tmp.append(z)
            self.zones.append(tmp)
    
    def getLargeur(self):
        return self.xml.getLargeur()
        
    def getHauteur(self):
        return self.xml.getHauteur()
    
    def isZone(self, x, y):
        return self.xml.isZone(x, y)

class WidgetPlancher(QWidget):
    def __init__(self, id = 0, parent=None):
        super(WidgetPlancher, self).__init__(parent)
        self.model = ModelePlancher()
        self.id = id
        self.currentZone = [0, 0]
    
    def sizeHint(self):
        return QSize(TAILLE_CASE * self.model.getLargeur()+1, TAILLE_CASE * self.model.getHauteur()+1)
        
    def mousePressEvent(self, event):
        x = event.x() / TAILLE_CASE
        y = event.y() / TAILLE_CASE
        if (self.model.zones[x][y].isZone()):
            self.currentZone = [x, y]
            self.model.zones[x][y].setProprio(100)
            self.update(QRect(x*TAILLE_CASE, y*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE))
    
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
        
        #Couleur de fond d'une case
        if (self.model.zones[x][y].getProprio() != 0):
            painter.setBrush(QBrush(QColor(128, 0, 0)))
        elif (self.model.zones[x][y].isZone()):
            painter.setBrush(QBrush(QColor(0, 128, 0)))
        else:
            painter.setBrush(QBrush(Qt.transparent))
        painter.drawRect(rectZone)
        
        #~ painter.setBrush(QBrush(QColor(0, 0, 0)))
        #~ painter.setPen(QPen(Qt.NoPen))
        #~ rectMur = QRect(x*TAILLE_CASE + 40, y*TAILLE_CASE + 25, 10, 25)
        #~ painter.drawRect(rectMur)

if __name__ == '__main__':
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    z = VuePlancher(100)
    z.show()
    app.exec_()
    qdb.closeSqlConnection()
