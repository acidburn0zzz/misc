#! /bin/bash

if [ `uname -r` != `cat /home/mathieu/src/linux/version` ]; then
    /home/mathieu/applications/NVIDIA-Linux-x86_64-173.14.05-pkg2.run --silent
    uname -r >/home/mathieu/src/linux/version
fi
