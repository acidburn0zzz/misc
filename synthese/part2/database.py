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

class Database(object):
    def openSqlConnection(self, type, db_name, host=None, user=None, passwd=None):
        self.db = QSqlDatabase.addDatabase(type)
        if (type == "QMYSQL"):
            self.db.setHostname(host)
            self.db.setUserName(user)
            self.db.setPassword(passwd)
        self.db.setDatabaseName(db_name)
        return self.db.open()

    def closeSqlConnection(self):
        self.db.close()
    
    def getCurrentId(self, table):
        q = QSqlQuery()
        if q.exec_("SELECT seq FROM db_sequence WHERE name='" + table + "'"):
            q.first()
            return q.value(0).toInt()[0]
        else:
            return -1
    
    def setCurrentId(self, table, newid):
        q = QSqlQuery()
        if q.exec_("UPDATE db_sequence SET seq=" + str(newid) + " WHERE name='" + table + "'"):
            return True
        else:
            return False
