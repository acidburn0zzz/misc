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

class Zone(QWidget):
    def __init__(self, isZone, parent=None):
        super(Zone, self).__init__(parent)
        self.zone = isZone
        
        if isZone:
            self.setPalette(QPalette(QColor(0, 0, 128)));
        #~ else:
            #~ self.setPalette(QPalette(QColor(0, 0, 0)));
        self.setAutoFillBackground(True);
    
    def isZone(self):
        return self.zone

    #Definitions de proprietes du widget
    def minimumSizeHint(self):
        return QSize(50, 50)

    def sizeHint(self):
        return QSize(50, 50)
    
    def mousePressEvent(self, event):
        event.ignore()
        #~ #Il ne se passe rien si c'est un couloir
        if not self.zone:
            return
        if event.button() == Qt.LeftButton:
            self.setPalette(QPalette(QColor(255, 0, 0)))

if __name__ == '__main__':
    import plancher
    app = QApplication([])
    #~ z = Zone(False)
    #~ z.show()
    z = plancher.VuePlancher(100)
    z.show()
    app.exec_()