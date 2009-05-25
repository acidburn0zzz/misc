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
import xmlparser
import zone

TAILLE_CASE = 50

#Constantes d'options
NB_OPTS = 19
OPT_LOC, OPT_WEBZ, OPT_ROUTER, OPT_ELEC, OPT_MURET = range(5)
LOC_ADD, LOC_RM, WEBZ_500K, WEBZ_2M, WEBZ_5M, WEBZ_RM, ROUT_ADD, ROUT_RM, ELEC_ADD, ELEC_RM, \
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
        self.radRouter = QRadioButton("Router")
        self.radElectricite = QRadioButton("Prises electriques")
        self.radMurets = QRadioButton("Murets")
        
        self.grpCat = QButtonGroup(self)
        self.grpCat.addButton(self.radLoc, OPT_LOC)
        self.grpCat.addButton(self.radWebz, OPT_WEBZ)
        self.grpCat.addButton(self.radRouter, OPT_ROUTER)
        self.grpCat.addButton(self.radElectricite, OPT_ELEC)
        self.grpCat.addButton(self.radMurets, OPT_MURET)
        
        self.layCat = QHBoxLayout()
        self.layCat.setAlignment(Qt.AlignLeft)
        self.layCentral.addLayout(self.layCat)
        self.layCat.addWidget(self.radLoc)
        self.layCat.addWidget(self.radWebz)
        self.layCat.addWidget(self.radRouter)
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
        
        #Options de router
        self.radOptions.append(QRadioButton("Ajouter"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        #Options d'electricite
        self.radOptions.append(QRadioButton("Ajouter"))
        self.radOptions.append(QRadioButton("Enlever"))
        
        #Options de murets
        #~ self.radOptions.append(QRadioButton("Haut-Gauche"))
        #~ self.radOptions.append(QRadioButton("Haut-Droite"))
        #~ self.radOptions.append(QRadioButton("Droite-Haut"))
        #~ self.radOptions.append(QRadioButton("Droite-Bas"))
        #~ self.radOptions.append(QRadioButton("Bas-Droit"))
        #~ self.radOptions.append(QRadioButton("Bas-Gauche"))
        #~ self.radOptions.append(QRadioButton("Gauche-Bas"))
        #~ self.radOptions.append(QRadioButton("Gauche-Haut"))
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
        
        #Selection par defaut -> Location
        self.radLoc.click()
    
    def getExposant(self):
        cie = db.getValeur("exposants", "nom", self.idExposant).toString()
        nom = db.getValeur("exposants", "resp_nom", self.idExposant).toString()
        prenom = db.getValeur("exposants", "resp_prenom", self.idExposant).toString()
        str = prenom + " " + nom + " (" + cie
        
        #Afficher le domaine
        dom = db.getValeur("exposants", "domaine", self.idExposant).toInt()[0]
        domaine = db.getValeur("domaines", "nom", dom).toString()
        str += ", " + domaine
        
        #Date d'inscription
        date = db.getValeur("exposants", "DATETIME(date_inscr, 'localtime')", self.idExposant).toString()
        str += ", " + date
        
        str += ")"
        
        return str
    
    def creerMenus(self):
        self.actSave = QAction("&Sauvegarder", self)
        self.actSave.setShortcut("Ctrl+S");
        self.actSave.setStatusTip("Sauvegarder les modification");
        self.connect(self.actSave, SIGNAL('triggered()'), self.wdPlancher.sauvegarder);
        
        self.actQuitter = QAction("&Quitter", self)
        self.actQuitter.setShortcut("Ctrl+Q");
        self.actQuitter.setStatusTip("Quitter le logiciel");
        self.connect(self.actQuitter, SIGNAL('triggered()'), qApp, SLOT('quit()'));
        
        self.mnuFile = QMenu("&Fichier", self)
        self.mnuFile.addAction(self.actSave)
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
        elif (id == OPT_ROUTER):
            self.radOptions[ROUT_ADD].setVisible(True)
            self.radOptions[ROUT_ADD].click()
            self.radOptions[ROUT_RM].setVisible(True)
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
                z = zone.Zone(i, j, self.xml.isZone(j, i))
                tmp.append(z)
            self.zones.append(tmp)
    
    def getLargeur(self):
        return self.xml.getLargeur()
        
    def getHauteur(self):
        return self.xml.getHauteur()
    
    def sauvegarder(self, id):
        #Si on travaille avec un historique
        for i in self.zones:
            for j in i:
                if (j.wasModified()):
                    j.sauvegarder()
        #Fin de la partie historique
        
        #Si on travaille sans historique
        #~ query = "DELETE FROM zones"
        #~ q = QSqlQuery()
        #~ q.exec_(query)
        
        #~ for i in self.zones:
            #~ for j in i:
                #~ j.sauvegarder()
        #Fin de la partie sans historique
    
    def isZone(self, x, y):
        return self.zones[x][y].isZone()
    
    def getInfos(self, x, y, id):
        webz = {
            zone.WEBZ_NONE : "Aucune",
            zone.WEBZ_500K : "500kbps",
            zone.WEBZ_2M : "2Mbps",
            zone.WEBZ_5M : "5Mbps"
        }
        infos = "Connexion internet : " + webz[self.getInterwebz(x, y)] + "\n" + \
                "Nb de prises electriques : " + str(self.getNbPrisesElectriques(x, y)) + "\n" + \
                "Router : " + ["Non", "Oui"][self.getRouter(x, y)] + "\n" + \
                "Nb de murets : " + str(self.getNbMurets(x, y)) + "\n" + \
                "Cout des options : " + str(self.getPrixOptions(x, y))
        
        if (id == 1):
            proprio = self.getProprio(x, y)
            if (proprio == -1):
                infos = "Zone inocupee"
            else:
                infos = "Proprietaire : " + str(proprio) + "\n" + infos
        return infos
    
    def ajouterMurs(self, x, y, id):
        #Case en haut
        if (y > 0 and self.getProprio(x, y-1) != -1 and self.getProprio(x, y-1) != id):
            self.addMur(x, y, zone.MUR_HAUT)
            self.addMur(x, y-1, zone.MUR_BAS)
            self.setModified(x, y-1)
        #Case en bas
        if (y < self.getHauteur() - 1 and self.getProprio(x, y+1) != -1 and self.getProprio(x, y+1) != id):
            self.addMur(x, y, zone.MUR_BAS)
            self.addMur(x, y+1, zone.MUR_HAUT)
            self.setModified(x, y+1)
        #Case a gauche
        if (x > 0 and self.getProprio(x-1, y) != -1 and self.getProprio(x-1, y) != id):
            self.addMur(x, y, zone.MUR_GAUCHE)
            self.addMur(x-1, y, zone.MUR_DROITE)
            self.setModified(x-1, y)
        #Case a droite
        if (x < self.getLargeur() - 1 and self.getProprio(x+1, y) != -1 and self.getProprio(x+1, y) != id):
            self.addMur(x, y, zone.MUR_DROITE)
            self.addMur(x+1, y, zone.MUR_GAUCHE)
            self.setModified(x+1, y)
    
    def enleverMurs(self, x, y, id):
        #Case en haut
        if (y > 0 and self.getProprio(x, y-1) != -1 and self.getProprio(x, y-1) != id):
            self.rmMur(x, y, zone.MUR_HAUT)
            self.rmMur(x, y-1, zone.MUR_BAS)
            self.setModified(x, y-1)
        #Case en bas
        if (y < self.getHauteur() - 1 and self.getProprio(x, y+1) != -1 and self.getProprio(x, y+1) != id):
            self.rmMur(x, y, zone.MUR_BAS)
            self.rmMur(x, y+1, zone.MUR_HAUT)
            self.setModified(x, y+1)
        #Case a gauche
        if (x > 0 and self.getProprio(x-1, y) != -1 and self.getProprio(x-1, y) != id):
            self.rmMur(x, y, zone.MUR_GAUCHE)
            self.rmMur(x-1, y, zone.MUR_DROITE)
            self.setModified(x-1, y)
        #Case a droite
        if (x < self.getLargeur() - 1 and self.getProprio(x+1, y) != -1 and self.getProprio(x+1, y) != id):
            self.rmMur(x, y, zone.MUR_DROITE)
            self.rmMur(x+1, y, zone.MUR_GAUCHE)
            self.setModified(x+1, y)
    
    def setModified(self, x, y, modif=True):
        self.zones[x][y].setModified(modif)
    
    def wasModified(self, x, y):
        return self.zones[x][y].wasModified()
    
    def getPrixOptions(self, x, y):
        return self.zones[x][y].getPrixOptions()
    
    #Les 2 fonctions suivantes prennent le wdPlancher en param pour forcer
    #le repaint de cases avec des gros murs
    def setProprio(self, x, y, id):
        if (self.getProprio(x, y) == -1):
            self.zones[x][y].setProprio(id)
            self.ajouterMurs(x, y, id)
            return True
        else:
            return False
    
    def rmProprio(self, x, y, id):
        if (self.getProprio(x, y) == id):
            self.zones[x][y].setProprio(-1)
            self.enleverMurs(x, y, id)
            return True
        else:
            return False
    
    def getProprio(self, x, y):
        return self.zones[x][y].getProprio()
    
    def setInterwebz(self, x, y, webz):
        self.zones[x][y].setInterwebz(webz)
    
    def getInterwebz(self, x, y):
        return self.zones[x][y].getInterwebz()
    
    def addRouter(self, x, y):
        return self.zones[x][y].addRouter()
    
    def rmRouter(self, x, y):
        return self.zones[x][y].rmRouter()
    
    def getRouter(self, x, y):
        return self.zones[x][y].getRouter()
    
    def addPriseElectrique(self, x, y):
        return self.zones[x][y].addPriseElectrique()
    
    def rmPriseElectrique(self, x, y):
        return self.zones[x][y].rmPriseElectrique()
    
    def getNbPrisesElectriques(self, x, y):
        return self.zones[x][y].getNbPrisesElectriques()
    
    def addMuret(self, x, y, muret):
        return self.zones[x][y].addMuret(muret)
    
    def viderMurets(self, x, y):
        return self.zones[x][y].viderMurets()
    
    def getNbMurets(self, x, y):
        return self.zones[x][y].getNbMurets()
    
    def getMurets(self, x, y):
        return self.zones[x][y].getMurets()
    
    def addMur(self, x, y, mur):
        return self.zones[x][y].addMur(mur)
    
    def rmMur(self, x, y, mur):
        return self.zones[x][y].rmMur(mur)
    
    def getMurs(self, x, y):
        return self.zones[x][y].getMurs()

class WidgetPlancher(QWidget):
    def __init__(self, id = 0, parent=None):
        super(WidgetPlancher, self).__init__(parent)
        self.model = ModelePlancher()
        self.id = id
        self.option = -1
        self.currentZone = [0, 0]
    
    def updateZone(self, x, y):
        if not self.model.isZone(x, y):
            return
        
        self.currentZone = [x, y]
        
        #TEMPORAIRE, FAIRE PASSER PAR LE MODELE#
        if (self.option == LOC_ADD):
            if (not self.model.setProprio(x, y, self.id)):
                QMessageBox.warning(self, 'Attention', "Cette zone n'est pas a louer")
            else:
                #On redessine tout le plancher pour afficher les nouveaux murs
                self.update()
        elif (self.model.getProprio(x, y) != self.id):
            self.afficherPasVotreZone()
            return
        elif (self.option == LOC_RM):
            self.model.rmProprio(x, y, self.id)
            #On redessine tout le plancher pour effacer les murs
            #qui ne sont plus requis
            self.update()
        elif (self.option == WEBZ_500K):
            self.model.setInterwebz(x, y, zone.WEBZ_500K)
        elif (self.option == WEBZ_2M):
            self.model.setInterwebz(x, y, zone.WEBZ_2M)
        elif (self.option == WEBZ_5M):
            self.model.setInterwebz(x, y, zone.WEBZ_5M)
        elif (self.option == WEBZ_RM):
            self.model.setInterwebz(x, y, zone.WEBZ_NONE)
        elif (self.option == ROUT_ADD):
            if not self.model.addRouter(x, y):
                QMessageBox.warning(self, 'Erreur', "Vous avez deja un router")
        elif (self.option == ROUT_RM):
            if not self.model.rmRouter(x, y):
                QMessageBox.warning(self, 'Erreur', "Vous n'avez pas de router")
        elif (self.option == ELEC_ADD):
            if not self.model.addPriseElectrique(x, y):
                QMessageBox.warning(self, 'Erreur', "Impossible d'ajouter une prise electrique")
        elif (self.option == ELEC_RM):
            if not self.model.rmPriseElectrique(x, y):
                QMessageBox.warning(self, 'Erreur', "Il n'y a pas de prise a enlever")
        elif (self.option >= MUR_HG and self.option <= MUR_GH):
            if not self.model.addMuret(x, y, VAL_MURS[self.option]):
                QMessageBox.warning(self, 'Erreur', "Impossible d'ajouter un muret")
        elif (self.option == MUR_RM):
            self.model.viderMurets(x, y)
        
        #Si on se rend ici, on considere que la case a ete modifiee
        self.model.setModified(x, y)
        
        #Redessiner seulement la case en cours
        self.update(QRect(x*TAILLE_CASE, y*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE))
    
    def setOption(self, id):
        self.option = id
    
    def afficherInfo(self, x, y):
        if (self.model.getProprio(x, y) == self.id or self.id == 1):
            infos = self.model.getInfos(x, y, self.id)
            QMessageBox.information(self, "Zone %d:%d" % (x, y), infos)
        else:
            self.afficherPasVotreZone()
    
    def sauvegarder(self):
        self.model.sauvegarder(self.id)
        QMessageBox.information(self, 'Information', "Informations sauvegardees")
    
    def afficherPasVotreZone(self):
        QMessageBox.warning(self, 'Attention', "Cette zone ne vous appartient pas")
        
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
    
    def paintZone(self, x, y):
        painter = QPainter(self)
        rectZone = QRect(x*TAILLE_CASE, y*TAILLE_CASE, TAILLE_CASE, TAILLE_CASE)
        
        if not self.model.isZone(x, y):
            painter.setBrush(QBrush(Qt.transparent))
            painter.drawRect(rectZone)
            return
        
        #~ #Couleur de fond d'une case
        if (self.model.getProprio(x, y) == self.id):
            painter.setBrush(QBrush(QColor(0x00, 0x80, 0x00)))
        elif (self.model.getProprio(x, y) > 0):
            painter.setBrush(QBrush(QColor(0x80, 0x00, 0x00)))
        else:
            painter.setBrush(QBrush(QColor(0xff, 0xff, 0xff)))
        painter.drawRect(rectZone)
        
        #Remise a la couleur noire du painter
        painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00)))
        painter.setPen(QPen(Qt.NoPen))
        
        #Dessin des prises internet
        #EMPECHER DE VOIR LES WEBZ DES AUTRES??
        if (self.model.getProprio(x, y) == self.id or self.id == 1):
            webz = self.model.getInterwebz(x, y)
            if (webz >= zone.WEBZ_500K):
                rectWebz = QRect(x*TAILLE_CASE+10, y*TAILLE_CASE+10, 5, 5)
                painter.drawRect(rectWebz)
            if (webz >= zone.WEBZ_2M):
                rectWebz = QRect(x*TAILLE_CASE+10, y*TAILLE_CASE+20, 5, 5)
                painter.drawRect(rectWebz)
            if (webz >= zone.WEBZ_5M):
                rectWebz = QRect(x*TAILLE_CASE+10, y*TAILLE_CASE+30, 5, 5)
                painter.drawRect(rectWebz)
        
        #Dessin du router
        #EMPECHER DE VOIR LE ROUTER DES AUTRES??
        if (self.model.getProprio(x, y) == self.id or self.id == 1):
            elec = self.model.getRouter(x, y)
            if (elec == 1):
                rectRout = QRect(x*TAILLE_CASE+30, y*TAILLE_CASE+30, 10, 5)
                painter.drawRect(rectRout)
        
        #Dessin des prises electriques
        #EMPECHER DE VOIR L'ELECTRICITE DES AUTRES??
        if (self.model.getProprio(x, y) == self.id or self.id == 1):
            elec = self.model.getNbPrisesElectriques(x, y)
            if (elec >= 1):
                rectElec = QRect(x*TAILLE_CASE+35, y*TAILLE_CASE+10, 5, 5)
                painter.drawRect(rectElec)
            if (elec >= 2):
                rectElec = QRect(x*TAILLE_CASE+35, y*TAILLE_CASE+20, 5, 5)
                painter.drawRect(rectElec)
        
        #Les murets seront bleu fonce
        painter.setBrush(QBrush(QColor(0x00, 0x00, 0x60)))
        painter.setPen(QPen(Qt.NoPen))
        
        #Dessin des murets
        murets = self.model.getMurets(x, y)
        
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
        
        #Les murs seront noir
        painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00)))
        painter.setPen(QPen(Qt.NoPen))
        
        #Dessin des murs
        murs = self.model.getMurs(x, y)
        
        if (murs & zone.MUR_HAUT):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE, 50, 5)
            painter.drawRect(rectMur)
        if (murs & zone.MUR_BAS):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE+45, 50, 5)
            painter.drawRect(rectMur)
        if (murs & zone.MUR_GAUCHE):
            rectMur = QRect(x*TAILLE_CASE, y*TAILLE_CASE, 5, 50)
            painter.drawRect(rectMur)
        if (murs & zone.MUR_DROITE):
            rectMur = QRect(x*TAILLE_CASE+45, y*TAILLE_CASE, 5, 50)
            painter.drawRect(rectMur)
    
    def sizeHint(self):
        return QSize(TAILLE_CASE * self.model.getLargeur()+1, TAILLE_CASE * self.model.getHauteur()+1)
    
    def minimumSizeHint(self):
        return QSize(TAILLE_CASE * self.model.getLargeur()+1, TAILLE_CASE * self.model.getHauteur()+1)

if __name__ == '__main__':
    app = QApplication([])
    qdb = db.Database()
    qdb.openSqlConnection("QSQLITE", "db.sqlite")
    z = VuePlancher(100)
    z.show()
    #~ z = VuePlancher(1)
    app.exec_()
    qdb.closeSqlConnection()
