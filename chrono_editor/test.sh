#! /bin/sh

make clean || exit 1
make || exit 1
./test || exit 1
snes9x-gtk ct.smc
