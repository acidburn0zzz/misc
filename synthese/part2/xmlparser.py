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

import xml.dom.minidom as xmldom

class XmlParser(object):
    def __init__(self, xmlFile):
        self.grille = []
        
        xml = xmldom.parse(xmlFile)
        
        self.largeur = int(xml.getElementsByTagName('largeur')[0].childNodes[0].nodeValue)
        self.hauteur = int(xml.getElementsByTagName('hauteur')[0].childNodes[0].nodeValue)
        
        #Genere un tableau representant le plancher
        count = 0
        for i in xml.getElementsByTagName('rangee'):
            tmp = []
            for j in i.childNodes[0].nodeValue:
                tmp.append(int(j))
            self.grille.append(tmp)
    
    def getLargeur(self):
        return self.largeur
        
    def getHauteur(self):
        return self.hauteur
    
    def isZone(self, x, y):
        if (self.grille[x][y] == 1):
            return True
        else:
            return False

if __name__ == '__main__':
    a = XmlParser('expo.xml')
    print a.isZone(1, 2)