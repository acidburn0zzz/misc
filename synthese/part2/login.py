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

import inscription

class VueLogin(QDialog):
    def __init__(self, parent=None):
        super(VueLogin, self).__init__(parent)
        self.model = ModeleLogin()
        
        self.setWindowTitle('Login')
        self.setWindowFlags(Qt.Tool)
        
        self.layCentral = QVBoxLayout()
        
        self.lblId = QLabel('Identificateur')
        self.lblPasswd = QLabel('Mot de passe')
        
        self.txtId= QLineEdit()
        self.txtId.setValidator(QIntValidator(0, 9999, self))
        self.txtPasswd = QLineEdit()
        self.txtPasswd.setEchoMode(QLineEdit.Password)
        
        self.layMain = QGridLayout()
        self.layMain.addWidget(self.lblId, 0, 0)
        self.layMain.addWidget(self.txtId, 0, 1)
        self.layMain.addWidget(self.lblPasswd, 1, 0)
        self.layMain.addWidget(self.txtPasswd, 1, 1)
        
        #Ajout des boutons
        self.btnLogin = QPushButton('&Login')
        self.btnInscription = QPushButton('&Inscription')
        self.btnQuit = QPushButton('&Quitter')
        self.connect(self.btnLogin, SIGNAL('clicked()'), self.testLogin)
        #~ self.connect(self.btnInscription, SIGNAL('clicked()'), self, SLOT('reject()'))
        self.connect(self.btnInscription, SIGNAL('clicked()'), self.inscription)
        self.connect(self.btnQuit, SIGNAL('clicked()'), self.quit)
        
        self.layBoutons = QHBoxLayout()
        self.layBoutons.addWidget(self.btnLogin)
        self.layBoutons.addWidget(self.btnInscription)
        self.layBoutons.addWidget(self.btnQuit)
        
        self.layCentral.addLayout(self.layMain)
        self.layCentral.addLayout(self.layBoutons)
        
        self.setLayout(self.layCentral)
    
    def testLogin(self):
        id = self.txtId.text()
        passwd = self.txtPasswd.text()
        if (self.model.testLogin(id, passwd)):
            self.accept()
        else:
            QMessageBox.warning(self, "Erreur", "L'identificateur ou le mot de passe saisi est invalide")
            self.txtPasswd.setText("")
            self.txtPasswd.setFocus()
            
    def inscription(self):
        i = inscription.VueInscription()
        i.exec_()
    
    def quit(self):
        self.done(-1)

class ModeleLogin(object):
    def testLogin(self, id, passwd):
        q = QSqlQuery()
        if not q.exec_("SELECT passwd FROM exposants WHERE id=" + id):
            return False
        
        if not q.first():
            return False
        
        passwd_hash = hashlib.sha512()
        passwd_hash.update(str(passwd)) #Passer d'une QString a une str standard
        
        if (passwd_hash.hexdigest() == q.value(0).toString()):
            return True
        else:
            return False

if __name__ == '__main__':
    import database
    app = QApplication([])
    db = database.Database()
    db.openSqlConnection("QSQLITE", "db.sqlite")
    v = VueLogin()
    print v.exec_()