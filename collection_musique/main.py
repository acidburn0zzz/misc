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

from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtSql import *
import sys

import database as db
import albums

def run():
    db_file = '/home/mathieu/codes/collection_musique/albums.sqlite'
    app = QApplication(sys.argv)

    qdb = db.Database()
    qdb.openSqlConnection('QSQLITE', db_file)

    a = albums.VueAlbums()
    a.show()

    ret = app.exec_()
    qdb.closeSqlConnection()
    return ret

if __name__ == '__main__':
    run()
