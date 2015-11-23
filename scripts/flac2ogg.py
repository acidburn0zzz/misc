#! /usr/bin/env python

import os
import sys
from concurrent.futures import ThreadPoolExecutor

oggenc_opts = '-q 10'


def usage(prog):
    print('{} flac_files'.format(prog))


def check_exe(name):
    exe = None
    ext = ''

    if sys.platform == 'win32':
        ext = '.exe'

    if os.path.isfile(name + ext):
        exe = name + ext
    else:
        path = os.getenv('PATH').split(';' if sys.platform == 'win32' else ':')
        for i in path:
            if os.path.isfile(i + '/' + name + ext):
                exe = i + '/' + name + ext
                break

    if exe is None:
        print('Error: {} not found'.format(name))
        exit(-1)


def get_tag(tag, file):
    tmp = os.popen('metaflac --show-tag={} "{}"'.format(tag, file)).read()
    if len(tmp) > 0:
        return tmp.split('=')[1][:-1]
    else:
        return ''


def convert_file(flac_file):
    if not os.path.isfile(flac_file):
        print('{}: file not found'.format(flac_file))
        return

    if os.path.splitext(flac_file)[1] != '.flac':
        print('{}: not a flac'.format(flac_file))
        return

    wav_file = os.path.splitext(flac_file)[0] + '.wav'
    ogg_file = os.path.splitext(flac_file)[0] + '.ogg'

    title = get_tag('TITLE', flac_file)
    artist = get_tag('ARTIST', flac_file)
    album = get_tag('ALBUM', flac_file)
    year = get_tag('DATE', flac_file)
    track = get_tag('TRACKNUMBER', flac_file) + '/' + get_tag('TRACKTOTAL', flac_file)
    genre = get_tag('GENRE', flac_file)

    tag_opts = '--title "{}" --artist "{}" --album "{}" --date "{}" --tracknum "{}" --genre "{}"'.format(title, artist, album, year, track, genre)

    os.system('flac -df -o "{}" "{}"'.format(wav_file, flac_file))

    cmd = 'oggenc {} {} "{}" -o "{}"'.format(oggenc_opts, tag_opts, wav_file, ogg_file)
    os.system(cmd)

    os.remove(wav_file)


def main():
    check_exe('flac')
    check_exe('oggenc')

    with ThreadPoolExecutor(max_workers=8) as e:
        for i in sys.argv[1:]:
            e.submit(convert_file, i)

if __name__ == '__main__':
    main()
