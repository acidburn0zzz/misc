import gobject
import gtk
import os
import signal
import traceback
import errno

from player import Player
from plugins.generic import GenericPlugin

class G15StatusPlugin(GenericPlugin):
    PLUGIN_NAME = "G15Status"
    PLUGIN_DESC = "Plugin to show Current player track on G15 LCD"
    PLUGIN_VERSION = "0.1"
    PLUGIN_AUTHOR = "AcidRain"
    PLUGIN_WEBSITE = ""

    def __init__(self):
        GenericPlugin.__init__(self)

        self.pidFile = "/tmp/.g15music.pid"

        self.__lastsong = None

        self.autoconnect(Player, "new-song", self.on_new_song)

        self.on_new_song(Player, Player.song)
        gtk.quit_add(0, self.resetScreen)

    def g15MusicIsAlive(self):
        try:
            pf = open(self.pidFile)
        except:
            # probably file not found
            return False

        try:
            self.daemonPid = int(pf.read().strip())
            pf.close()
        except:
            traceback.print_exc()
            return False

        try:
            if not os.path.exists('/proc'):
                print "missing /proc"
                return True # no /proc, assume G15Music is running

            try:
                f = open('/proc/%d/cmdline'% self.daemonPid)
            except IOError, e:
                if e.errno == errno.ENOENT:
                    return False # file/pid does not exist
                raise

            n = f.read().lower()
            f.close()
            if n.find('g15music') < 0:
                return False
            return True # Running G15Music found at pid
        except:
            traceback.print_exc()

        # If we are here, pidfile exists, but some unexpected error occured.
        # Assume G15Music is running.
        return True

    def sendUpdateSignal(self):
        if (self.g15MusicIsAlive()):
            os.kill(self.daemonPid, signal.SIGUSR1)

    def resetScreen(self):
        if (self.g15MusicIsAlive()):
            os.kill(self.daemonPid, signal.SIGUSR2)

    def sendKillSignal(self):
        if (self.g15MusicIsAlive()):
            os.kill(self.daemonPid, signal.SIGINT)

    def writeSongToFile(self, songInfo):
        file = open("/tmp/.g15music_song", "w")
        file.write(songInfo)
        file.close()

    def on_new_song(self, player, song):
        self.__lastsong = song

        title = ""
        artist = ""
        album = ""

        if song.get_str("title"):
            title = song.get_str("title")
        if song.get_str("artist"):
            artist = song.get_str("artist")
        if song.get_str("album"):
            album = song.get_str("album")

        status_msg = "%s;%s;%s" % (title, artist, album)
        self.loginfo("Change G15 status to \"%s\"", status_msg)
        self.writeSongToFile(status_msg)
        self.sendUpdateSignal()
