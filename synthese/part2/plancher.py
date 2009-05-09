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

class VuePlancher(QMainWindow):
    def __init__(self, id, parent=None):
        super(VuePlancher, self).__init__(parent)
        self.model = ModelePlancher()
        self.idExposant = id
        
        self.setWindowTitle("Plancher")
        self.wdCentral = QWidget()
        self.setCentralWidget(self.wdCentral)
        
        self.layPlancher = QGridLayout()
        self.layPlancher.setSpacing(0)
        self.zones = []

        for i in range(self.model.getLargeur()):
            tmp = []
            for j in range(self.model.getHauteur()):
                z = zone.Zone(self.model.isZone(j, i), self.wdCentral)
                tmp.append(z)
                self.layPlancher.addWidget(z, j, i)
            self.zones.append(zone)
        
        #Widget qui contient toutes les zones
        self.wdPlancher = WidgetPlancher(self)
        self.wdPlancher.setGeometry(0, 0, 50*self.model.getLargeur(), 50*self.model.getHauteur())
        self.wdPlancher.setLayout(self.layPlancher)
        
        self.layCentral = QVBoxLayout()
        self.layCentral.addWidget(self.wdPlancher)
        self.wdCentral.setLayout(self.layCentral)

class ModelePlancher(object):
    def __init__(self):
        self.xml = xmlparser.XmlParser('expo.xml')
    
    def getLargeur(self):
        return self.xml.getLargeur()
        
    def getHauteur(self):
        return self.xml.getHauteur()
    
    def isZone(self, x, y):
        return self.xml.isZone(x, y)

class WidgetPlancher(QWidget):
    def __init__(self, parent=None, flags=0):
        super(WidgetPlancher, self).__init__(parent)
        self.setPalette(QPalette(QColor(0, 128, 0)));
        
    def mousePressEvent(self, event):
        print "WidgetPlancher: ", event.x(), event.y()
    
    def minimumSizeHint(self):
        return QSize(50*11, 50*9)

    def sizeHint(self):
        return QSize(50*11, 50*9)

if __name__ == '__main__':
    app = QApplication([])
    z = VuePlancher(100)
    z.show()
    app.exec_()
