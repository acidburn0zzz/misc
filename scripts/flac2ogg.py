#! /usr/bin/env python
#-*- coding: utf-8 -*-
import sys
import os

oggenc_opts = '-q 10'

win32 = False

def usage(prog):
    print('%s flac_files' % (prog))

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
        print('Error: %s not found' % (name))
        exit(-1)

def get_tag(tag, file):
    tmp = os.popen('metaflac --show-tag=%s "%s"' % (tag, file)).read()
    if len(tmp) > 0:
        return tmp.split('=')[1][:-1]
    else:
        return ''

def convert_file(flac_file):
    if not os.path.isfile(flac_file):
        print('%s: file not found' % (flac_file))
        return

    #Est-ce un .flac
    if flac_file[len(flac_file)-5:] != '.flac':
        print('%s: not a flac' % (flac_file))
        return

    wav_file = flac_file[:-5] + '.wav'
    ogg_file = flac_file[:-5] + '.ogg'

    #Creation des options
    title = get_tag('TITLE', flac_file)
    artist = get_tag('ARTIST', flac_file)
    album = get_tag('ALBUM', flac_file)
    year = get_tag('DATE', flac_file)
    track = get_tag('TRACKNUMBER', flac_file) + '/' + get_tag('TRACKTOTAL', flac_file)
    genre = get_tag('GENRE', flac_file)

    tag_opts = '--title "%s" --artist "%s" --album "%s" --date "%s" --tracknum "%s" --genre "%s"' % (title, artist, album, year, track, genre)

    #Flac 2 wav
    os.system('flac -df -o "%s" "%s"' % (wav_file, flac_file))

    #Wav 2 ogg
    cmd = 'oggenc %s %s "%s" -o "%s"' % (oggenc_opts, tag_opts, wav_file, ogg_file)
    os.system(cmd)

    #rm wav
    os.system('rm "%s"' % wav_file)

if __name__ == '__main__':
    if sys.platform == 'win32':
        win32 = True

    #Verification de l'existence de lame et flac
    check_exe('flac')
    check_exe('oggenc')

    for i in sys.argv[1:]:
        convert_file(i)
