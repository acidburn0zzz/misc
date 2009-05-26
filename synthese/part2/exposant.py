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

class VueExposant(QDialog):
    def __init__(self, id, zones, parent=None):
        super(VueExposant, self).__init__(parent)
        self.id = id
        self.model = ModeleExposant(id, zones)
        self.setWindowTitle("Rapport personnel")
        
        self.layCentral = QVBoxLayout()
        self.setLayout(self.layCentral)
        
        self.lblInfos = QLabel()
        self.layCentral.addWidget(self.lblInfos)
        
        self.btnFermer = QPushButton("&Fermer")
        self.btnFermer.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.connect(self.btnFermer, SIGNAL("clicked()"), self, SLOT("accept()"))
        self.layBouton = QHBoxLayout()
        self.layBouton.addWidget(self.btnFermer, 0, Qt.AlignRight)
        
        self.layCentral.addLayout(self.layBouton)
        
        self.showInfos()
        
    def showInfos(self):
        sInfos =  "Rapport de " + db.getNomExposant(self.id) + "\n\n"
        sInfos += "    Nombre de kiosques : " + self.formatterPrix(self.model.getNbKiosques()) + "\n"
        sInfos += "    Nombre de zones : " + self.formatterPrix(self.model.getNbZones()) + "\n\n"
        sInfos += "    Prix pour la location des zones : " + self.formatterPrix(self.model.getPrixZones()) + "$\n"
        sInfos += "    Prix pour les connexions internet : " + self.formatterPrix(self.model.getPrixWebz()) + "$\n"
        sInfos += "    Prix pour les routers : " + self.formatterPrix(self.model.getPrixRouter()) + "$\n"
        sInfos += "    Prix pour les prises electriques : " + self.formatterPrix(self.model.getPrixElectricite()) + "$\n"
        sInfos += "    Prix pour les murets : " + self.formatterPrix(self.model.getPrixMurets()) + "$\n\n"
        sInfos += "    Total : " + self.formatterPrix(self.model.getPrixTotal()) + "$"
        self.lblInfos.setText(sInfos)
    
    def formatterPrix(self, prix):
        str_in = str(prix)
        length = len(str_in)
        i = 0
        
        if (length % 3 == 1):
            str_out = str_in[0] + " "
        elif (length % 3 == 2):
            str_out = str_in[0] + str_in[1] + " "
        else:
            str_out = ""
        
        i += length % 3
        
        while (i < length):
            str_out += str_in[i] + str_in[i+1] + str_in[i+2] + " "
            i += 3
        
        return str_out[:-1] #Pour enlever le dernier espace

class ModeleExposant(object):
    def __init__(self, id, zones):
        self.id = id
        self.zones = zones
        
        self.prixZones = 0
        self.prixWebz = 0
        self.prixRouter = 0
        self.prixElectricite = 0
        self.prixMurets = 0
        
        self.nbKiosques = 0
        self.nbZones = 0
        
        self.idDomaine = db.getValeur("exposants", "domaine", self.id).toInt()[0]
        self.domaine = db.getValeur("domaines", "nom", self.idDomaine).toString()
        
        self.calculNbKiosques()
        self.calculPrixTotal()
    
    def calculPrixTotal(self):
        #On calcule le prix des options de chaque zone
        for row in self.zones:
            for z in row:
                if (z.getProprio() == self.id):
                    self.prixWebz += z.getPrixWebz()
                    self.prixRouter += z.getPrixRouter()
                    self.prixElectricite += z.getPrixElectricite()
                    self.prixMurets += z.getPrixMurets()
                    self.nbZones += 1
        
        #Cout initial de l'inscription (seulement si on a un kiosque)
        if (self.nbKiosques > 0):
            if (self.domaine == "Enseignement"):
                self.prixZones += 4000
            else:
                self.prixZones += 5000
            
            #On charge 1500 par kiosque (moins un inclus dans le prix de depart)
            self.prixZones += 1500 * (self.nbKiosques - 1)
            
            #~ On charge 1000 par zone (moins une zone inclue dans le prix de chaque kiosque)
            self.prixZones += 1000 * (self.nbZones - self.nbKiosques)
    
    def getPrixTotal(self):
        #~ print self.prixZones + self.prixWebz + \
        #~ self.prixRouter + self.prixElectricite + self.prixMurets
        return self.prixZones + self.prixWebz + \
        self.prixRouter + self.prixElectricite + self.prixMurets
    
    def getPrixZones(self):
        return self.prixZones
    
    def getPrixWebz(self):
        return self.prixWebz
    
    def getPrixRouter(self):
        return self.prixRouter
    
    def getPrixElectricite(self):
        return self.prixElectricite
    
    def getPrixMurets(self):
        return self.prixMurets
    
    def getNbKiosques(self):
        return self.nbKiosques
    
    def getNbZones(self):
        return self.nbZones
    
    def calculNbKiosques(self):
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
                
                #On ajoute aussi toutes les zones qui touchent celle en cours
                #a la liste des deja comptees, pour eviter une erreur possible
                #du a l'ordre de parcours
                if (self.zones[x][y].getProprio() == self.id):
                    #Case en haut
                    if (y > 0 and self.zones[x][y-1].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x, y-1)):
                            continue
                        else:
                            zonesDejaComptees.append([x, y-1])
                    #Case en bas
                    if (y < hauteur - 1 and self.zones[x][y+1].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x, y+1)):
                            continue
                        else:
                            zonesDejaComptees.append([x, y+1])
                    #Case a gauche
                    if (x > 0 and self.zones[x-1][y].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x-1, y)):
                            continue
                        else:
                            zonesDejaComptees.append([x-1, y])
                    #Case a droite
                    if (x < largeur - 1 and self.zones[x+1][y].getProprio() == self.id):
                        if (self.aDejaEteComptee(zonesDejaComptees, x+1, y)):
                            continue
                        else:
                            zonesDejaComptees.append([x+1, y])
                    
                    #Si on se rend ici, on ajoute un kiosque
                    self.nbKiosques += 1
    
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
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    #~ m = ModeleExposant(100, getZones())
    v = VueExposant(100, getZones())
    v.show()
    app.exec_()
    qdb.closeSqlConnection()