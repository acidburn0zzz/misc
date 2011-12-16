#! /bin/bash

BASENAME=`echo $1 | awk -F . '{print $1}'`
VIDEO=1
AUDIO=2

cat << EOF > meta 
MUXOPT --no-pcr-on-video-pid --new-audio-pes --vbr  --vbv-len=500
V_MPEG4/ISO/AVC, "$1", fps=23.976, level=4.1, insertSEI, contSPS, ar=As source, track=$VIDEO, lang=und
A_AC3, "$1", track=$AUDIO, lang=eng"
EOF

tsMuxeR meta $BASENAME.m2ts

rm meta
