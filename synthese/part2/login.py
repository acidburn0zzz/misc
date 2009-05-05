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

#~ pwd = hashlib.sha256()
#~ pwd.update('passwd')
#~ print pwd.digest()
#~ print pwd.hexdigest()

class Login(QDialog):
    def __init__(self, parent=None):
        super(Login, self).__init__(parent)
        
        self.setWindowTitle('Login')
        #~ self.centralWidget = QWidget()
        #~ self.setCentralWidget(self.centralWidget)
        
        self.centralLayout = QVBoxLayout()
        
        self.lblId = QLabel('Identificateur')
        self.lblPwd = QLabel('Mot de passe')
        
        self.txtId= QLineEdit()
        self.txtId.setValidator(QIntValidator(0, 9999, self))
        self.txtPwd = QLineEdit()
        self.txtPwd.setValidator(QIntValidator(0, 60, self))
        
        self.layMain = QGridLayout()
        self.layMain.addWidget(self.lblId, 0, 0)
        self.layMain.addWidget(self.txtId, 0, 1)
        self.layMain.addWidget(self.lblPwd, 1, 0)
        self.layMain.addWidget(self.txtPwd, 1, 1)
        
        #Ajout des boutons
        self.btnLogin = QPushButton('&Login')
        self.btnInscription = QPushButton('&Inscription')
        self.btnQuit = QPushButton('&Quitter')
        #~ self.connect(self.btnLogin, SIGNAL('clicked()'), self.calculate)
        self.connect(self.btnQuit, SIGNAL('clicked()'), self.quit)
        #~ self.connect(self.btnQuit, SIGNAL('clicked()'), qApp, SLOT('quit()'))
        
        self.layBoutons = QHBoxLayout()
        self.layBoutons.addWidget(self.btnLogin)
        self.layBoutons.addWidget(self.btnInscription)
        self.layBoutons.addWidget(self.btnQuit)
        
        self.centralLayout.addLayout(self.layMain)
        self.centralLayout.addLayout(self.layBoutons)
        
        self.setLayout(self.centralLayout)
    
    def quit(self):
        qApp.quit()
        #~ self.done(2)