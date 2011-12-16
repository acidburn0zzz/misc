#! /bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 image.iso"
    exit 1
fi

growisofs -use-the-force-luke=dao -speed=4 -Z /dev/sr0=$1
