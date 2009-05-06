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
import hashlib

import database

class VueInscription(QDialog):
    def __init__(self, parent=None):
        super(VueInscription, self).__init__(parent)
        self.model = ModeleInscription()
        
        self.setWindowTitle('Inscription')
        self.setWindowFlags(Qt.Tool)
        
        self.layCentral = QVBoxLayout()
        
        self.lblId = QLabel('Identificateur')
        self.lblCie = QLabel('Compagnie')
        self.lblDomaine = QLabel('Domaine')
        self.lblRespNom = QLabel('Nom')
        self.lblRespPrenom = QLabel('Prenom')
        self.lblEmail = QLabel('Courriel')
        self.lblPasswd = QLabel('Mot de passe')
        self.lblPasswd2 = QLabel('Confirmation')
        
        self.txtId = QLineEdit()
        self.txtId.setReadOnly(True)
        self.txtCie = QLineEdit()
        self.cmbDomaine = QComboBox()
        self.txtRespNom = QLineEdit()
        self.txtRespPrenom = QLineEdit()
        self.txtEmail = QLineEdit()
        self.txtEmail.setValidator(QRegExpValidator(QRegExp("^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$"), self))
        self.txtPasswd = QLineEdit()
        self.txtPasswd.setEchoMode(QLineEdit.Password)
        self.txtPasswd2 = QLineEdit()
        self.txtPasswd2.setEchoMode(QLineEdit.Password)
        
        self.layMain = QGridLayout()
        self.layMain.addWidget(self.lblId, 0, 0)
        self.layMain.addWidget(self.txtId, 0, 1)
        self.layMain.addWidget(self.lblCie, 1, 0)
        self.layMain.addWidget(self.txtCie, 1, 1)
        self.layMain.addWidget(self.lblDomaine, 2, 0)
        self.layMain.addWidget(self.cmbDomaine, 2, 1)
        self.layMain.addWidget(self.lblRespNom, 3, 0)
        self.layMain.addWidget(self.txtRespNom, 3, 1)
        self.layMain.addWidget(self.lblRespPrenom, 4, 0)
        self.layMain.addWidget(self.txtRespPrenom, 4, 1)
        self.layMain.addWidget(self.lblEmail, 5, 0)
        self.layMain.addWidget(self.txtEmail, 5, 1)
        self.layMain.addWidget(self.lblPasswd, 6, 0)
        self.layMain.addWidget(self.txtPasswd, 6, 1)
        self.layMain.addWidget(self.lblPasswd2, 7, 0)
        self.layMain.addWidget(self.txtPasswd2, 7, 1)
        
        self.btnOk = QPushButton('&Inscription')
        self.btnCancel = QPushButton('&Annuler')
        
        self.layBoutons = QHBoxLayout()
        self.layBoutons.addWidget(self.btnOk)
        self.layBoutons.addWidget(self.btnCancel)
        
        self.connect(self.btnOk, SIGNAL('clicked()'), self.confirmInscription)
        self.connect(self.btnCancel, SIGNAL('clicked()'), self, SLOT('reject()'))
        
        self.layCentral.addLayout(self.layMain)
        self.layCentral.addLayout(self.layBoutons)
        
        self.setLayout(self.layCentral)
        
        self.txtId.setText(str(self.model.getNextId()))
        
        self.domaines = self.model.getDomaines()
        for i in self.domaines:
            self.cmbDomaine.addItem(i)
        
        #Valeurs test#
        #~ self.txtCie.setText('Sun Microsystems')
        #~ self.txtRespNom.setText('Lemay')
        #~ self.txtRespPrenom.setText('Mathieu')
        #~ self.txtEmail.setText('acidrain1@gmail.com')
        #~ self.txtPasswd.setText('a')
        #~ self.txtPasswd2.setText('a')
        #Valeurs test#
        
    def confirmInscription(self):
        values = {}
        if not self.txtEmail.hasAcceptableInput():
            QMessageBox.warning(self, 'Erreur', 'Adresse courriel invalide')
            return False
        else:
            values['email'] = self.txtEmail.text()
        
        if (self.txtPasswd.text() != self.txtPasswd2.text()):
            QMessageBox.warning(self, 'Erreur', 'Les champs mot de passe ne concordent pas')
            return False
        else:
            values['passwd'] = self.txtPasswd.text()
        
        values['id'] = self.txtId.text()
        values['cie'] = self.txtCie.text()
        values['domaine'] = self.cmbDomaine.currentText()
        values['resp_nom'] = self.txtRespNom.text()
        values['resp_prenom'] = self.txtRespPrenom.text()
        
        if self.model.addExposant(values, self):
            self.accept()

class ModeleInscription(object):
    def getNextId(self):
        curId = database.getCurrentId('exposants')
        return curId + 10
        
    def getDomaines(self):
        doms = []
        doms.append('Dom1')
        doms.append('Dom2')
        doms.append('Dom3')
        
        return doms
    
    def addExposant(self, values, parent=None):
        passwd_hash = hashlib.sha512()
        passwd_hash.update(str(values['passwd'])) #Passer d'une QString a une str standard
        values['passwd_hash'] = passwd_hash.hexdigest()
        
        query = "INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) " + \
            "VALUES (" + values['id'] + ", '" + values['cie'] + "', '" + values['domaine'] + "', '" + \
            values['resp_nom'] + "', '" + values['resp_prenom'] + "', '" + values['email'] + "', date('now'), '"  + values['passwd_hash'] + "');"
        q = QSqlQuery()
        if not q.exec_(query):
            QMessageBox.warning(parent, 'Erreur', "Il y a deja un enregistrement avec le meme responsable et la meme compagnie")
            print 'Erreur: ' + q.lastError().text()
            return False
        else:
            database.setCurrentId('exposants', int(values['id']))
            QMessageBox.information(parent, 'Information', "Inscription reussie\nVotre identificateur est : " + values['id'])
            return True

if __name__ == '__main__':
    app = QApplication([])
    db = database.Database()
    db.openSqlConnection("QSQLITE", "db.sqlite")
    v = VueInscription()
    print v.exec_()
