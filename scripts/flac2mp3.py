#! /usr/bin/env python2
#-*- coding: utf-8 -*-
import sys
import os

lame_opts = '-b 128 -B 320 -V2 -q0 -m s --add-id3v2'

title = None
artist = None
album = None
year = None
track = None
genre = None

win32 = False

def usage(prog):
    print '%s flac_files' % prog

def check_exe(name):
    exe = None
    ext = ''

    if win32:
        ext = '.exe'

    if os.path.isfile(name + ext):
        exe = name + ext
    else:
        path = os.getenv('PATH').split(';' if win32 else ':')
        for i in path:
            if os.path.isfile(i + '/' + name + ext):
                exe = i + '/' + name + ext
                break

    if exe == None:
        print 'Error: %s not found' % name
        exit(-1)

def get_tag(tag, file):
    tmp = os.popen('metaflac --show-tag=%s %s' % (tag, file)).read()
    if len(tmp) > 0:
        return tmp.split('=')[1][:-1]
    else:
        return ''

def convert_file(flac_file):
    #Est-ce un .flac
    if flac_file[len(flac_file)-5:] != '.flac':
        print flac_file + ': Not a .flac'

    wav_file = flac_file[:-5] + '.wav'
    mp3_file = flac_file[:-5] + '.mp3'

    #Creation des options
    _title = title
    _artist = artist
    _album = album
    _year = year
    _track = track
    _genre = genre
    if _title == None:
        _title = get_tag('TITLE', flac_file)
    if _artist == None:
        _artist = get_tag('ARTIST', flac_file)
    if _album == None:
        _album = get_tag('ALBUM', flac_file)
    if _year == None:
        _year = get_tag('DATE', flac_file)
    if _track == None:
        _track = get_tag('TRACKNUMBER', flac_file) + '/' + get_tag('TRACKTOTAL', flac_file)
    if _genre == None:
        _genre = get_tag('GENRE', flac_file)

    tag_opts = '--tt "%s" --ta "%s" --tl "%s" --ty "%s" --tn "%s" --tg "%s"' % (_title, _artist, _album, _year, _track, _genre)

    #Flac 2 wav
    os.system('flac -df -o %s %s' % (wav_file, flac_file))

    #Wav 2 mp3
    cmd = 'lame %s %s %s %s' % (lame_opts, tag_opts, wav_file, mp3_file)
    os.system(cmd)

    #rm wav
    os.system('rm %s' % wav_file)

if __name__ == '__main__':
    if sys.platform == 'win32':
        win32 = True

    #Verification de l'existence de lame et flac
    check_exe('flac')
    check_exe('lame')

    for i in sys.argv[1:]:
        convert_file(i)
