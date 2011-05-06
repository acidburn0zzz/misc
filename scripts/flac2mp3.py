#! /usr/bin/env python
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

def usage(prog):
    print '%s flac_files' % prog

def check_flac():
    flac = None
    path = os.getenv('PATH').split(':')
    for i in path:
        if os.path.isfile(i + '/flac'):
            flac = i + '/flac'
            break

    if flac == None:
        print 'Error: flac not found'
        exit(-1)

def check_lame():
    lame = None
    path = os.getenv('PATH').split(':')
    for i in path:
        if os.path.isfile(i + '/lame'):
            lame = i + '/lame'
            break

    if lame == None:
        print 'Error: lame not found'
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
    #Verification de l'existence de lame et flac
    check_flac()
    check_lame()

    for i in sys.argv[1:]:
        convert_file(i)
