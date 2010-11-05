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

from PySide.QtSql import *

def formatterDuree(duree): #Pourrait prendre un nbre de secondes
    duree_tmp = duree.replace('-', ':').split(':')

    if (len(duree_tmp) == 2):
        h = '00'
        m = duree_tmp[0]
        s = duree_tmp[1]
    else:
        h = duree_tmp[0]
        m = duree_tmp[1]
        s = duree_tmp[2]

    if (len(h) == 1):
        h = '0' + h
    if (len(m) == 1):
        m = '0' + m
    if (len(s) == 1):
        s = '0' + s

    return h + ':' + m + ':' + s

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
