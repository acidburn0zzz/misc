#! /bin/bash

if [ $# -ne 1 ]; then
    echo Usage: $0 image.iso
    exit -1
fi

wodim -v -dao speed=4 dev=/dev/sr0 $1
