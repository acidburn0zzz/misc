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

#Enumerations (sort-of)
WEBZ_NONE, WEBZ_500K, WEBZ_2M, WEBZ_5M = range(4)

#Murets D:Droite, G:Gauche, H:Haut, B:Bas
#Enumeres dans le sens des aiguilles d'une montre
MUR_HG = 0x01
MUR_HD = 0x02
MUR_DH = 0x04
MUR_DB = 0x08
MUR_BD = 0x10
MUR_BG = 0x20
MUR_GB = 0x40
MUR_GH = 0x80

class Zone(object):
    def __init__(self, isZone):
        self.zone = isZone
        self.proprio = 0 #Inoccupe
        self.interwebz = WEBZ_NONE
        self.nbprises = 0
        self.murets = 0
    
    def isZone(self):
        return self.zone
    
    def setProprio(self, proprio):
        self.proprio = proprio
    
    def getProprio(self):
        return self.proprio
    
    def setInterwebz(self, interwebz):
        if (interwebz == WEBZ_500K or interwebz == WEBZ_2M or interwebz == WEBZ_5M):
            self.interwebz = interwebz
        else:
            self.interwebz = WEBZ_NONE
    
    def getInterwebz(self):
        return self.interwebz
    
    def setNbPrisesElectriques(self, nbprises):
        if (nbprises == 1 or nbprises == 2):
            self.nbprises = nbprises
        else:
            self.nbprises = 0
    
    def getNbPrisesElectriques(self):
        return self.nbprises
    
    def setMurets(self, murets):
        #255 represente 8 murets, ce qui est invalide
        if (murets >= 0 and murets < 255):
            self.murets = murets
        else:
            murets = 0

if __name__ == '__main__':
    import plancher
    app = QApplication([])
    z = plancher.VuePlancher(100)
    z.show()
    app.exec_()