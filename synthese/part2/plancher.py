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

class VuePlancher(QMainWindow):
    def __init__(self, id, parent=None):
        super(VuePlancher, self).__init__(parent)
        self.idExposant = id
        
        self.setWindowTitle("Plancher")
        self.wdCentral = QWidget()
        self.setCentralWidget(self.wdCentral)
        
        self.lblExposant = QLabel(self.getExposant())
        
        self.wdPlancher = WidgetPlancher(id, self)
        
        self.layCentral = QVBoxLayout()
        self.layCentral.addWidget(self.lblExposant)
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
