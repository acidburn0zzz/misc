#! /bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 xgd3game.iso"
    exit 1
fi

growisofs -use-the-force-luke=break:2133520 -speed=4 -Z /dev/sr0=$1
