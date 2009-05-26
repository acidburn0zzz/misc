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
import math

import database as db
import zone

#Definition des couleurs pour le graphique
COULEURS = [
    QColor(0xc0, 0x00, 0x00), # webz
    QColor(0x00, 0x00, 0xc0), # router
    QColor(0x00, 0xc0, 0xc0), # electricite
    QColor(0xc0, 0x00, 0xc0), # murets
    QColor(0x00, 0xc0, 0x00)  # zone
]

def formatterPrix(prix):
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
        
    #Remplacer le dernier espace par un '$'
    str_out = str_out[:-1] + "$"
    
    return str_out

class VueRapportExposant(QDialog):
    def __init__(self, id, zones, parent=None):
        super(VueRapportExposant, self).__init__(parent)
        self.id = id
        self.zones = zones
        self.model = ModeleExposant(id, zones)
        self.setWindowTitle("Rapport personnel")
        
        self.layCentral = QVBoxLayout()
        self.setLayout(self.layCentral)
        
        self.txtInfos = QTextEdit()
        self.txtInfos.setMinimumSize(400, 480)
        self.txtInfos.setSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.MinimumExpanding)
        self.txtInfos.setReadOnly(True)
        self.layCentral.addWidget(self.txtInfos)
        
        self.btnFermer = QPushButton("&Fermer")
        self.btnFermer.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.connect(self.btnFermer, SIGNAL("clicked()"), self, SLOT("accept()"))
        self.layBouton = QHBoxLayout()
        self.layBouton.addWidget(self.btnFermer, 0, Qt.AlignRight)
        
        self.layCentral.addLayout(self.layBouton)
        
        self.showInfos()
        
    def showInfos(self):
        sInfos =  "Rapport de " + db.getNomExposant(self.id, True, True) + "\n\n"
        sInfos += "  Nombre de kiosques : " + str(self.model.getNbKiosques()) + "\n"
        sInfos += "  Nombre de zones : " + str(self.model.getNbZones()) + "\n\n"
        sInfos += "  Detail des zones :\n"
        for row in self.zones:
            for z in row:
                if (z.getProprio() == self.id):
                    sInfos += "    Zone [" + str(z.getPosX()) + "," + str(z.getPosY()) + "]\n"
                    sInfos += "    Prix pour la connexion internet : " + formatterPrix(z.getPrixWebz()) + "\n"
                    sInfos += "    Prix pour le router : " + formatterPrix(z.getPrixRouter()) + "\n"
                    sInfos += "    Prix pour les prises electriques : " + formatterPrix(z.getPrixElectricite()) + "\n"
                    sInfos += "    Prix pour les murets : " + formatterPrix(z.getPrixMurets()) + "\n\n"
        sInfos += "  Prix total des zones : " + formatterPrix(self.model.getPrixZones()) + "\n"
        sInfos += "  Prix total des connexions internet : " + formatterPrix(self.model.getPrixWebz()) + "\n"
        sInfos += "  Prix total des routers : " + formatterPrix(self.model.getPrixRouter()) + "\n"
        sInfos += "  Prix total des prises electriques : " + formatterPrix(self.model.getPrixElectricite()) + "\n"
        sInfos += "  Prix total des murets : " + formatterPrix(self.model.getPrixMurets()) + "\n\n"
        sInfos += "  Grand Total : " + formatterPrix(self.model.getPrixTotal())
        self.txtInfos.setText(sInfos)

class VueRapportGeneral(QDialog):
    def __init__(self, zones, parent=None):
        super(VueRapportGeneral, self).__init__(parent)
        self.setWindowTitle("Rapport general")
        
        self.zones = zones
        self.exposants = []
        
        self.fillExposants()
    
        self.wdGraphique = WidgetGraphique(self.exposants, self.getPrixPlusEleve())
        self.wdGraphique.setMinimumSize(800, 600)
        self.wdGraphique.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        
        self.lblInfos = QLabel()
        
        self.cmbExposants = QComboBox()
        self.cmbExposants.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.connect(self.cmbExposants, SIGNAL('currentIndexChanged(int)'), self.afficheInfos)
        for i in self.exposants:
            self.cmbExposants.addItem(i['nom'])
        
        self.layInfos = QHBoxLayout()
        self.layInfos.addWidget(self.cmbExposants, 0, Qt.AlignLeft)
        self.layInfos.addWidget(self.lblInfos, 0, Qt.AlignRight)
        
        #Footer
        self.lblTotal = QLabel("Total des couts : " + formatterPrix(self.getCoutTotal()))
        self.lblTotal.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        
        self.btnFermer = QPushButton("&Fermer")
        self.btnFermer.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.connect(self.btnFermer, SIGNAL("clicked()"), self, SLOT("accept()"))
        self.layFooter = QHBoxLayout()
        self.layFooter.addWidget(self.lblTotal, 0, Qt.AlignLeft)
        self.layFooter.addWidget(self.btnFermer, 0, Qt.AlignRight)
        
        self.layCentral = QVBoxLayout()
        self.layCentral.addWidget(self.wdGraphique)
        self.layCentral.addLayout(self.layInfos)
        #~ self.layCentral.addWidget(self.lblTotal, 0, Qt.AlignRight)
        self.layCentral.addLayout(self.layFooter)
        
        self.setLayout(self.layCentral)
    
    def afficheInfos(self, id):
        sInfos = "ID: " + str(self.exposants[id]['id']) + ", " + \
            "Webz: " + formatterPrix(self.exposants[id]['pWebz']) + ", " + \
            "Router: " + formatterPrix(self.exposants[id]['pRouter']) + ", " + \
            "Electricite: " + formatterPrix(self.exposants[id]['pElectricite']) + ", " + \
            "Murets: " + formatterPrix(self.exposants[id]['pMurets']) + ", " + \
            "Zones: " + formatterPrix(self.exposants[id]['pZones'])
        self.lblInfos.setText(sInfos)
    
    def fillExposants(self):
        q = QSqlQuery()
        q.exec_("SELECT id FROM exposants WHERE id >= 100")
        q.first()
        
        while (q.isValid()):
            id = q.value(0).toInt()[0]
            m = ModeleExposant(id, self.zones)
            
            exposant = {}
            exposant['id'] = id
            exposant['nom'] = db.getNomExposant(id)
            exposant['pZones'] = m.getPrixZones()
            exposant['pWebz'] = m.getPrixWebz()
            exposant['pRouter'] = m.getPrixRouter()
            exposant['pElectricite'] = m.getPrixElectricite()
            exposant['pMurets'] = m.getPrixMurets()
            exposant['pTotal'] = m.getPrixTotal()
            
            self.exposants.append(exposant)
            q.next()
    
    #Devrait etre dans un modele
    def getPrixPlusEleve(self):
        prix = 0
        
        for i in self.exposants:
            if (i['pTotal'] > prix):
                prix = i['pTotal']
        
        return prix
    
    def getCoutTotal(self):
        prix = 0
        
        for i in self.exposants:
            prix += i['pTotal']
        
        return prix

class WidgetGraphique(QWidget):
    def __init__(self, exposants, prixPlusEleve, parent=None):
        super(WidgetGraphique, self).__init__(parent)
        self.exposants = exposants
        self.prixPlusEleve = prixPlusEleve
        
        if (len(self.exposants) < 7):
            self.largeurBarre = 50
        else:
            self.largeurBarre = math.floor(780 / (len(self.exposants) * 2))
        
        self.update()
    
    def paintEvent(self, event):
        num = 1
        
        for i in self.exposants:
            self.paintBarre(num, i)
            num += 1
        
    
    def paintBarre(self, numero, exposant):
        hauteurTot = math.floor(float(exposant['pTotal']) / self.prixPlusEleve * 550)
        
        prix = exposant['pWebz'], exposant['pRouter'], exposant['pElectricite'], \
            exposant['pMurets'], exposant['pZones']
        
        hauteurs = []
    
        painter = QPainter(self)
        #~ painter.setPen(QPen(Qt.NoPen))
        
        #Dessin du rectangle autour
        rect = QRect(0, 0, 799, 599)
        painter.drawRect(rect)
        
        for i in range(len(prix)):
            try:
                hauteurs.append(prix[i] / float(exposant['pTotal']) * hauteurTot)
            except ZeroDivisionError:
                hauteurs.append(0)
        
        if (hauteurTot > 0):
            posY = 600 - hauteurTot
            
            for i in range(len(prix)):
                painter.setBrush(QBrush(COULEURS[i]))
                rect = QRect((2*numero-1)*self.largeurBarre, posY, self.largeurBarre, hauteurs[i])
                painter.drawRect(rect)
                posY += hauteurs[i]
    
    def sizeHint(self):
        return QSize(800, 600)

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

if __name__ == '__main__':
    import xmlparser
    xml = xmlparser.XmlParser('expo.xml')
    
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
        
    zones = []
    for i in range(xml.getLargeur()):
        tmp = []
        for j in range(xml.getHauteur()):
            z = zone.Zone(i, j, xml.isZone(j, i))
            tmp.append(z)
        zones.append(tmp)
    
    v = VueRapportExposant(100, zones)
    #~ v = VueRapportGeneral(zones)
    v.show()
    app.exec_()
    qdb.closeSqlConnection()