#! /usr/bin/env python
#-*- coding: utf-8 -*-

import serial
import sys
import time

from PyQt4.QtCore import SIGNAL, Qt
from PyQt4.QtGui import *

def int2hex(n):
    return str(hex(n)[2:]).zfill(2)

class ArduinoControlCenter(QMainWindow):
    def __init__(self, parent=None):
        super(ArduinoControlCenter, self).__init__(parent)
        self.centralWidget = QWidget()
        self.setCentralWidget(self.centralWidget)
        self.setWindowTitle('Arduino Control Center')
        
        #Attributs
        self.valR = 0
        self.valG = 0
        self.valB = 0
        
        #Boutons d'intervalle
        self.btnLEDBlinkOff = QPushButton("Off")
        self.btnLEDBlinkSlow = QPushButton("Slow")
        self.btnLEDBlinkMedium = QPushButton("Medium")
        self.btnLEDBlinkFast = QPushButton("Fast")
        self.btnLEDBlinkPlain = QPushButton("ON")
        self.connect(self.btnLEDBlinkOff, SIGNAL('clicked()'), self.setLEDBlinkOff)
        self.connect(self.btnLEDBlinkSlow, SIGNAL('clicked()'), self.setLEDBlinkDelaySlow)
        self.connect(self.btnLEDBlinkMedium, SIGNAL('clicked()'), self.setLEDBlinkDelayMedium)
        self.connect(self.btnLEDBlinkFast, SIGNAL('clicked()'), self.setLEDBlinkDelayFast)
        self.connect(self.btnLEDBlinkPlain, SIGNAL('clicked()'), self.setLEDBlinkPlain)
        
        #Slider d'intensite
        self.sldBrightness = QSlider(Qt.Horizontal, self)
        self.sldBrightness.setMinimum(0)
        self.sldBrightness.setMaximum(255)
        self.connect(self.sldBrightness, SIGNAL('sliderMoved(int)'), self.setBrightness)
        
        #Sliders de couleur
        self.sldR = QSlider(Qt.Horizontal, self)
        self.sldR.setMinimum(0)
        self.sldR.setMaximum(255)
        self.sldG = QSlider(Qt.Horizontal, self)
        self.sldG.setMinimum(0)
        self.sldG.setMaximum(255)
        self.sldB = QSlider(Qt.Horizontal, self)
        self.sldB.setMinimum(0)
        self.sldB.setMaximum(255)
        self.lblTestCouleur = QLabel()
        self.lblCodeCouleur = QLabel('#000000')
        
        self.connect(self.sldR, SIGNAL('sliderMoved(int)'), self.setR)
        self.connect(self.sldG, SIGNAL('sliderMoved(int)'), self.setG)
        self.connect(self.sldB, SIGNAL('sliderMoved(int)'), self.setB)
        
        #Layout LED Blink
        self.layLEDBlink = QHBoxLayout()
        self.layLEDBlink.addWidget(self.btnLEDBlinkOff)
        self.layLEDBlink.addWidget(self.btnLEDBlinkSlow)
        self.layLEDBlink.addWidget(self.btnLEDBlinkMedium)
        self.layLEDBlink.addWidget(self.btnLEDBlinkFast)
        self.layLEDBlink.addWidget(self.btnLEDBlinkPlain)
        
        #Layout Brightness
        self.layBrightness = QVBoxLayout()
        self.layBrightness.addWidget(self.sldBrightness)
        
        #Layout Couleur
        self.layCouleur = QVBoxLayout()
        self.layCouleur.addWidget(self.sldR)
        self.layCouleur.addWidget(self.sldG)
        self.layCouleur.addWidget(self.sldB)
        self.layCouleur.addWidget(self.lblTestCouleur)
        self.layCouleur.addWidget(self.lblCodeCouleur)
        
        #Groupes
        self.grpLEDBlink = QGroupBox(self)
        self.grpLEDBlink.setLayout(self.layLEDBlink)
        self.grpLEDBlink.setTitle("LED Blink")
        
        self.grpLEDBright = QGroupBox(self)
        self.grpLEDBright.setLayout(self.layBrightness)
        self.grpLEDBright.setTitle("LED Brightness")
        
        self.grpCouleur = QGroupBox(self)
        self.grpCouleur.setLayout(self.layCouleur)
        self.grpCouleur.setTitle("Couleur")
        
        #Layout principal
        self.centralLayout = QVBoxLayout()
        self.centralLayout.addWidget(self.grpLEDBlink)
        self.centralLayout.addWidget(self.grpLEDBright)
        self.centralLayout.addWidget(self.grpCouleur)
        self.centralWidget.setLayout(self.centralLayout)
    
    def setArduino(self, value):
        arduino.write(value)
    
    def setLEDBlinkOff(self):
        self.setArduino('\x00')
    
    def setLEDBlinkDelaySlow(self):
        self.setArduino('\xff')
    
    def setLEDBlinkDelayMedium(self):
        self.setArduino('\x80')
    
    def setLEDBlinkDelayFast(self):
        self.setArduino('\x10')
    
    def setLEDBlinkPlain(self):
        self.setArduino('\x01')
    
    def setBrightness(self, val):
        self.setArduino(chr(val))
    
    def setR(self, val):
        self.valR = val
        self.setCouleur()
    
    def setG(self, val):
        self.valG = val
        self.setCouleur()
    
    def setB(self, val):
        self.valB = val
        self.setCouleur()
    
    def setCouleur(self):
        palette = self.lblTestCouleur.palette();
        palette.setColor(self.lblTestCouleur.backgroundRole(), QColor(self.valR, self.valG, self.valB));
        self.lblTestCouleur.setPalette(palette);
        self.lblTestCouleur.setAutoFillBackground(True);
        self.lblTestCouleur.update();
        
        self.lblCodeCouleur.setText('#' + int2hex(self.valR) + int2hex(self.valG) + int2hex(self.valB))

def run():
    app = QApplication(sys.argv)
    
    cc = ArduinoControlCenter()
    cc.show()
    
    ret = app.exec_()
    return ret

if __name__ == '__main__':
    try:
        arduino = serial.Serial('/dev/ttyUSB0', baudrate=57600)
    except serial.serialutil.SerialException:
        print sys.exc_value
        sys.exit()
    #~ arduino = None
    run()