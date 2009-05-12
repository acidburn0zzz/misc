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

import zone
import xmlparser

TAILLE_CASE = 50

class VuePlancher(QMainWindow):
    def __init__(self, id, parent=None):
        super(VuePlancher, self).__init__(parent)
        self.idExposant = id
        
        self.setWindowTitle("Plancher")
        self.wdCentral = QWidget()
        self.setCentralWidget(self.wdCentral)
        
        self.wdPlancher = WidgetPlancher()
        
        self.layCentral = QVBoxLayout()
        self.layCentral.addWidget(self.wdPlancher)
        self.wdCentral.setLayout(self.layCentral)

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
    def __init__(self, parent=None):
        super(WidgetPlancher, self).__init__(parent)
        self.model = ModelePlancher()
    
    def sizeHint(self):
        return QSize(TAILLE_CASE * self.model.getLargeur()+1, TAILLE_CASE * self.model.getHauteur()+1)
        
    def mousePressEvent(self, event):
        x = event.x() / TAILLE_CASE
        y = event.y() / TAILLE_CASE
        self.model.zones[x][y].setOccupee(not self.model.zones[x][y].isOccupee())
        self.update()
    
    def paintEvent(self, event):
        print "paint", event.rect().topLeft(), event.rect().bottomRight()
        painter = QPainter(self)
        
        for i in range(self.model.getLargeur()):
            for j in range(self.model.getHauteur()):
                if (self.model.zones[i][j].isOccupee()):
                    painter.setBrush(QBrush(QColor(128, 0, 0)))
                elif (self.model.zones[i][j].isZone()):
                    painter.setBrush(QBrush(QColor(0, 128, 0)))
                else:
                    painter.setBrush(QBrush(Qt.transparent))
                zoneRect = QRect(i*TAILLE_CASE, j*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE)
                painter.drawRect(zoneRect)

if __name__ == '__main__':
    app = QApplication([])
    z = VuePlancher(100)
    z.show()
    app.exec_()
