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

import database as db
import zone

#~ class VueExposant(QDialog):
class VueExposant(QLineEdit):
    def __init__(self, id, zones, parent=None):
        super(VueExposant, self).__init__(parent)
        self.model = ModeleExposant(id, zones)
        
        self.setText(str(self.model.getCoutTotal()))

class ModeleExposant(object):
    def __init__(self, id, zones):
        self.id = id
        self.zones = zones
        
        self.idDomaine = db.getValeur("exposants", "domaine", self.id).toInt()[0]
        self.domaine = db.getValeur("domaines", "nom", self.idDomaine).toString()
        
        print self.getCoutTotal()
    
    def getCoutTotal(self):
        nbZones = 0
        nbKiosques = self.getNbKiosques()
        
        #Cout initial de l'inscription
        if (self.domaine == "Enseignement"):
            cout = 4000
        else:
            cout = 5000
        
        #On ajoute le prix des options de chaque zone
        for row in self.zones:
            for z in row:
                if (z.getProprio() == self.id):
                    cout += z.getPrixOptions()
                    nbZones += 1
        
        #On charge 1500 par kiosque (moins un inclus dans le prix de depart)
        cout += 1500 * (nbKiosques - 1)
        
        #~ On charge 1000 par zone (moins une zone inclue dans le prix de chaque kiosque)
        cout += 1000 * (nbZones - nbKiosques)
        
        return cout
    
    def getNbKiosques(self):
        zonesDejaComptees = []
        nbKiosques = 0
        
        largeur = len(self.zones)
        hauteur = len(self.zones[0])
        
        for x in range(largeur):
            for y in range(hauteur):
                #On ajoute la zone courante a celles deja comptees
                zonesDejaComptees.append([x, y])
                
                #On parcourt les zones entourant celle en cours
                #Des qu'on tombe sur une deja comptee, on passe a la suivante
                #Sinon, on ajoute un kiosque
                if (self.zones[x][y].getProprio() == self.id):
                    #Case en haut
                    if (y > 0 and self.zones[x][y-1].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x, y-1)):
                            continue
                    #Case en bas
                    if (y < hauteur - 1 and self.zones[x][y+1].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x, y+1)):
                            continue
                    #Case a gauche
                    if (x > 0 and self.zones[x-1][y].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x-1, y)):
                            continue
                    #Case a droite
                    if (x < largeur - 1 and self.zones[x+1][y].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x+1, y)):
                            continue
                    
                    #Si on se rend ici, on ajoute un kiosque
                    nbKiosques += 1

        return nbKiosques
    
    def aDejaEteComptee(self, zonesDejaComptees, x, y):
        for i in (zonesDejaComptees):
            if (i == [x, y]):
                return True
        return False

def getZones():
    import xmlparser
    xml = xmlparser.XmlParser('expo.xml')
        
    zones = []
    for i in range(xml.getLargeur()):
        tmp = []
        for j in range(xml.getHauteur()):
            z = zone.Zone(i, j, xml.isZone(j, i))
            tmp.append(z)
        zones.append(tmp)
    return zones

if __name__ == '__main__':
    #~ app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    m = ModeleExposant(100, getZones())
    #~ v = VueExposant(120, getZones())
    #~ v.show()
    #~ app.exec_()
    qdb.closeSqlConnection()