import logging
import os
import signal
import traceback
from xl import event

logger = logging.getLogger(__name__)

class G15StatusPlugin():
    def __init__(self):
        self.pidFile = "/tmp/.g15music.pid"
        self.exaile = None

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
            logger.error(traceback.format_exc())
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
            logger.error(traceback.format_exc())

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

    def on_play(self, type, player, track):
        title = ""
        artist = ""
        album = ""

        if track.get_tag_display('title'):
            title = track.get_tag_display('title')
        if track.get_tag_display('artist'):
            artist = track.get_tag_display('artist')
        if track.get_tag_display('album'):
            album = track.get_tag_display('album')

        status_msg = "%s;%s;%s" % (title, artist, album)
        logger.info("Change G15 status to \"%s\"", status_msg)
        self.writeSongToFile(status_msg)
        self.sendUpdateSignal()

    def on_pause(self, type, player, track):
        title = ""
        artist = ""
        album = ""

        if track.get_tag_display('title'):
            title = track.get_tag_display('title') + " (Paused)"
        if track.get_tag_display('artist'):
            artist = track.get_tag_display('artist')
        if track.get_tag_display('album'):
            album = track.get_tag_display('album')

        status_msg = "%s;%s;%s" % (title, artist, album)
        logger.info("Change G15 status to \"%s\"", status_msg)
        self.writeSongToFile(status_msg)
        self.sendUpdateSignal()

    def on_stop(self, type, player, track):
        self.resetScreen()

G15STATUS = G15StatusPlugin()

def enable(exaile):
    G15STATUS.exaile = exaile
    event.add_callback(G15STATUS.on_play, 'playback_player_start')
    event.add_callback(G15STATUS.on_play, 'playback_track_start')
    event.add_callback(G15STATUS.on_pause, 'playback_player_pause')
    event.add_callback(G15STATUS.on_stop, 'playback_player_end')
    event.add_callback(G15STATUS.on_play, 'playback_player_resume')
    #~ event.add_callback(EXAILE_NOTIFYOSD.on_quit, 'quit_application')
    #~ event.add_callback(EXAILE_NOTIFYOSD.on_changed, 'track_tags_changed')
    #~ if hasattr(exaile, 'gui'):
        #~ EXAILE_NOTIFYOSD.exaile_ready()
    #~ else:
        #~ event.add_callback(EXAILE_NOTIFYOSD.exaile_ready, 'gui_loaded')
        #~ EXAILE_NOTIFYOSD.gui_callback = True

def disable(exaile):
    event.remove_callback(G15STATUS.on_play, 'playback_player_start')
    event.remove_callback(G15STATUS.on_play, 'playback_track_start')
    event.remove_callback(G15STATUS.on_pause, 'playback_player_pause')
    event.remove_callback(G15STATUS.on_stop, 'playback_player_end')
    event.remove_callback(G15STATUS.on_play, 'playback_player_resume')

def teardown(exaile):
    G15STATUS.on_stop()
