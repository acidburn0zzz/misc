#! /usr/bin/env python

import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

BITRATE = '256'


def has_opusenc():
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    opusenc = 'opusenc.exe' if sys.platform == 'win32' else 'opusenc'

    if is_exe(opusenc):
        return True
    else:
        path = os.getenv('PATH').split(os.pathsep)
        for p in path:
            if is_exe(os.path.join(p, opusenc)):
                return True

    return False


def convert_file(flac_file):
    if not os.path.isfile(flac_file):
        print('{}: file not found'.format(flac_file))
        return

    if os.path.splitext(flac_file)[1] != '.flac':
        print('{}: not a flac'.format(flac_file))
        return

    opus_file = os.path.splitext(flac_file)[0] + '.opus'

    subprocess.call(['opusenc', '--bitrate', BITRATE, flac_file, opus_file])


def main():
    if not has_opusenc():
        print('Error: opusenc not found')
        exit(-1)

    with ThreadPoolExecutor(max_workers=8) as e:
        for f in sys.argv[1:]:
            e.submit(convert_file, f)

if __name__ == '__main__':
    main()
