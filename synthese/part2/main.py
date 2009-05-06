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
import sys

import database
import login

def run():
    app = QApplication(sys.argv)
    
    db = database.Database()
    if not db.openSqlConnection("QSQLITE", "db.sqlite"):
        print "Erreur: Impossible d'ouvrir la base de donnees"
        return
    
    l = login.VueLogin()
    l_ret = l.exec_()
    
    #Le login a ete accepte
    if (l_ret == QDialog.Accepted):
        print 'Yeah!'
        
        #Afficher la fenetre main
        return
    #On veut s'inscrire
    elif (l_ret == QDialog.Rejected):
        print 'No!'
        
        #Afficher la fenetre main
        return
    #On quitte
    else:
        db.closeSqlConnection()
        return
    
    db.closeSqlConnection()
    return app.exec_()
    

if __name__ == '__main__':
    run()
