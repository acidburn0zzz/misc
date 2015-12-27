#! /usr/bin/env python

import os
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor

BITRATE = '256'

cur_file = nb_files = 0
mutex = threading.Lock()


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


def print_progress():
    print('\rProgress: [{}{}] {:02d}/{:02d}'.format('.' * cur_file,
                                                    ' ' * (nb_files - cur_file),
                                                    cur_file,
                                                    nb_files),
          end='')


def convert_file(flac_file):
    if not os.path.isfile(flac_file):
        print('{}: file not found'.format(flac_file))
        return

    if os.path.splitext(flac_file)[1] != '.flac':
        print('{}: not a flac'.format(flac_file))
        return

    opus_file = os.path.splitext(flac_file)[0] + '.opus'

    try:
        err_msg = None
        subprocess.call(['opusenc', '--bitrate', BITRATE, flac_file, opus_file],
                        stdout=subprocess.DEVNULL,
                        stderr=subprocess.DEVNULL)
    except Exception as e:
        err_msg = str(e)

    global cur_file

    mutex.acquire()

    if err_msg is not None:
        print('\n' + err_msg)

    cur_file += 1
    print_progress()

    mutex.release()


def main():
    if not has_opusenc():
        print('Error: opusenc not found')
        exit(-1)

    global nb_files
    nb_files = len(sys.argv) - 1

    if nb_files == 0:
        print('No files specified')
        exit(-2)

    print_progress()

    with ThreadPoolExecutor(max_workers=8) as e:
        for f in sys.argv[1:]:
            e.submit(convert_file, f)
    print('')

if __name__ == '__main__':
    main()
