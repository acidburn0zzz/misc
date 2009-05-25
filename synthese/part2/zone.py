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

#Murs complets
MUR_HAUT = 0x01
MUR_DROITE = 0x02
MUR_GAUCHE = 0x04
MUR_BAS = 0x08

#TEST#
def bin(x):
    return ''.join(x & (1 << i) and '1' or '0' for i in range(7,-1,-1))
#TEST#

class Zone(object):
    def __init__(self, posX, posY, isZone):
        self.posX = posX
        self.posY = posY
        self.zone = isZone
        self.modif = False
        
        #Servira a identifier une zone qui a ete de-louee afin de
        #la sauvegarder dans la bdd si on travaille avec historique
        #~ self.ancienProprio = -1
        
        self.getInfo()
    
    def getInfo(self):
        query = "SELECT proprio, webz, router, electricite, murets, murs FROM zones WHERE posX=" + str(self.posX) + " AND posY=" + str(self.posY)
        #Si on travaille avec un historique de chaque zone, decommenter la ligne suivante
        query += " ORDER BY id DESC LIMIT 1"
        q = QSqlQuery()
        q.exec_(query)
        if (q.first()):
            self.proprio    = q.value(0).toInt()[0]
            self.interwebz  = q.value(1).toInt()[0]
            self.router     = q.value(2).toInt()[0]
            self.nbprises   = q.value(3).toInt()[0]
            self.murets     = q.value(4).toInt()[0]
            self.murs       = q.value(5).toInt()[0]
        else:
            self.proprio    = -1 #Inoccupe
            self.interwebz  = WEBZ_NONE
            self.router     = 0
            self.nbprises   = 0
            self.murets     = 0
            self.murs       = 0
    
    def sauvegarder(self):
        query = "INSERT INTO zones (posX, posY, proprio, webz, router, electricite, murets, murs)" + \
            " VALUES (" + str(self.posX) + ", " + str(self.posY) + ", " + str(self.proprio) + ", " + str(self.interwebz) + \
            ", " + str(self.router) + ", " + str(self.nbprises) + ", " + str(self.murets)  + ", " + str(self.murs) + ")"
        q = QSqlQuery()
        if q.exec_(query):
            self.modif = False
    
    def setModified(self, modif=True):
        self.modif = modif
    
    def wasModified(self):
        return self.modif
    
    def getPosX(self):
        return self.posX
    
    def getPosY(self):
        return self.posY
    
    def isZone(self):
        return self.zone
    
    def getPrixOptions(self):
        webz = {
            WEBZ_NONE : 0,
            WEBZ_500K : 20,
            WEBZ_2M : 50,
            WEBZ_5M : 75
        }
        
        prix  = webz[self.interwebz]
        prix += self.router * 60
        prix += self.nbprises * 50
        
        #Je ne charge pas le prix d'un muret si un 
        #gros mur a ete place par dessus
        murets = self.getMurets()
        murs = self.getMurs()
        
        if ((murets & MUR_HD) and not (murs & MUR_HAUT)):
            prix += 100
        if ((murets & MUR_HG) and not (murs & MUR_HAUT)):
            prix += 100
        if ((murets & MUR_BD) and not (murs & MUR_BAS)):
            prix += 100
        if ((murets & MUR_BG) and not (murs & MUR_BAS)):
            prix += 100
        if ((murets & MUR_DH) and not (murs & MUR_DROITE)):
            prix += 100
        if ((murets & MUR_DB) and not (murs & MUR_DROITE)):
            prix += 100
        if ((murets & MUR_GH) and not (murs & MUR_GAUCHE)):
            prix += 100
        if ((murets & MUR_GB) and not (murs & MUR_GAUCHE)):
            prix += 100
        
        return prix
    
    def setProprio(self, proprio):
        if (proprio >= 100 and proprio % 10 == 0):
            self.proprio = proprio
        else:   # "De-louage" de la zone
            #~ self.ancienProprio = self.proprio
            self.proprio = -1
            self.interwebz = WEBZ_NONE
            self.router = 0
            self.nbprises = 0
            self.murets = 0
            self.murs = 0
    
    def getProprio(self):
        return self.proprio
    
    def getAncienProprio(self):
        return self.ancienProprio
    
    def setInterwebz(self, interwebz):
        if (interwebz == WEBZ_500K or interwebz == WEBZ_2M or interwebz == WEBZ_5M):
            self.interwebz = interwebz
        else:
            self.interwebz = WEBZ_NONE
    
    def getInterwebz(self):
        return self.interwebz
    
    def addRouter(self):
        if (self.router == 0):
            self.router = 1
            return True
        else:
            return False
    
    def rmRouter(self):
        if (self.router == 1):
            self.router = 0
            return True
        else:
            return False
    
    def getRouter(self):
        return self.router
    
    def addPriseElectrique(self):
        if (self.nbprises < 2):
            self.nbprises = self.nbprises + 1
            return True
        else:
            return False
    
    def rmPriseElectrique(self):
        if (self.nbprises > 0):
            self.nbprises = self.nbprises - 1
            return True
        else:
            return False
    
    def getNbPrisesElectriques(self):
        return self.nbprises
    
    def addMuret(self, muret):
        if (muret >= 0 and muret < 255):
            tmp = self.murets | muret
            
            if (tmp == 255):    #Murs tout autour
                return False
            else:
                self.murets = tmp
                return True
    
    def rmMuret(self, muret):   #Non utilisee pour le moment
        if (muret >= 0 and muret < 255):
            muret = muret ^ 0xff                #Inverser pour avoir un byte contenant 7 '1', ex: 0b11101111
            self.murets = self.murets & muret   #Garder le byte comme avant sauf pour le muret a enlever
    
    def viderMurets(self):
        self.murets = 0
    
    def getNbMurets(self):
        return str(bin(self.murets)).count('1')
    
    def getMurets(self):
        return self.murets
    
    def addMur(self, mur):
        if (mur >= 0 and mur < 16):
            tmp = self.murs | mur
            
            if (tmp == 16):    #Murs tout autour
                return False
            else:
                self.murs = tmp
                return True
    
    def rmMur(self, mur):
        if (mur >= 0 and mur < 16):
            mur = mur ^ 0xff              #Inverser pour avoir un byte contenant 7 '1', ex: 0b11101111
            self.murs = self.murs & mur   #Garder le byte comme avant sauf pour le mur a enlever
    
    def viderMurs(self):
        self.murs = 0
    
    def getMurs(self):
        return self.murs

if __name__ == '__main__':
    import plancher
    import database as db
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    z = plancher.VuePlancher(100)
    z.show()
    app.exec_()
    qdb.closeSqlConnection()