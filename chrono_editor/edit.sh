#! /bin/sh

hexedit test.srm || exit 1
./sanity test.srm || exit 1
snes9x-gtk ct.smc
