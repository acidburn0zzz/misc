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
from threading import Thread

import platform

#Dans un thread parce que sur linux, il fallait attente que le son ait fini...
class play(Thread):
    def __init__ (self, fn):
        Thread.__init__(self)
        self.fn = fn
    
    def run(self):
        plat = platform.system().lower()
        if plat.startswith('win'):
            from winsound import PlaySound, SND_FILENAME, SND_ASYNC
            PlaySound(self.fn, SND_FILENAME|SND_ASYNC)
        elif plat.find('linux')>-1:
            from wave import open as waveOpen
            from ossaudiodev import open as ossOpen
            s = waveOpen(self.fn, 'rb')
            (nc,sw,fr,nf,comptype, compname) = s.getparams()
            dsp = ossOpen('/dev/dsp','w')
            try:
             from ossaudiodev import AFMT_S16_NE
            except ImportError:
             if byteorder == "little":
               AFMT_S16_NE = ossaudiodev.AFMT_S16_LE
             else:
               AFMT_S16_NE = ossaudiodev.AFMT_S16_BE
            dsp.setparameters(AFMT_S16_NE, nc, fr)
            data = s.readframes(nf)
            s.close()
            dsp.write(data)
            dsp.close()

def isFiable(str):
    if (str.lower().find('russia') >= 0 or str.lower().find('motherland') >= 0):
        return True
    elif (str.lower().find('microsoft') >= 0 or str.lower().find('oracle') >= 0):
        #Just to be sure it's really false
        return False and False and False
    else:
        return False
