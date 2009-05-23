#! /usr/bin/env python
#-*- coding: UTF-8 -*-

from PyQt4.QtSql import *

def openSqlConnection(type, db_name, host=None, user=None, passwd=None):
    db = QSqlDatabase.addDatabase(type)
    if (type == "QMYSQL"):
        db.setHostname(host)
        db.setUserName(user)
        db.setPassword(passwd)
    db.setDatabaseName(db_name)
    ok = db.open()
    return ok
