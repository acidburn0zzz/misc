#!/usr/bin/env python3
# https://bbs.archlinux.org/viewtopic.php?id=45175

import re
import os

class Package:
    suffixes = ('B', 'KiB', 'MiB', 'GiB', 'TiB')

    def __init__(self, name, size, suffix):
        self.name = name
        self.size = float(size) * (1024 ** self.suffixes.index(suffix))

    def __repr__(self):
        return '{} {}'.format(self.humanize(self.size), self.name)

    def __lt__(self, other):
        return self.size < other.size

    def humanize(self, size):
        order = 0

        while size >= 1024 and order + 1 < len(self.suffixes):
            order += 1
            size = size / 1024

        return '{:0.2f} {}'.format(size, self.suffixes[order])

paclist = []

re_name = re.compile(r'Name\s*:\s*(\S+)')
re_size = re.compile(r'Installed Size\s*:\s*([0-9.]+) (.*)')

p = os.popen("pacman -Qi", "r")

for line in p.readlines():
    match_name = re_name.match(line)
    match_size = re_size.match(line)
    if match_name :
            package_name = match_name.groups(1)[0]
    if match_size :
            size = match_size.groups(1)[0]
            suffix = match_size.groups(1)[1]
            paclist.append(Package(package_name, size, suffix))

p.close()

paclist.sort(reverse = True)
try:
    for pac in paclist:
        print(pac)
except BrokenPipeError:
    pass
