#! /bin/bash

BASENAME=`echo $1 | awk -F . '{print $1}'`
VIDEO=2
AUDIO=1

cat << EOF > meta 
MUXOPT --no-pcr-on-video-pid --new-audio-pes --vbr  --vbv-len=500
V_MPEG4/ISO/AVC, "/home/mathieu/tv/$BASENAME.mkv", fps=23.976, level=4.1, insertSEI, contSPS, ar=As source, track=$VIDEO, lang=und
A_AC3, "/home/mathieu/tv/$BASENAME.mkv", track=$AUDIO, lang=eng"
EOF

tsMuxeR meta $BASENAME.m2ts

rm meta
