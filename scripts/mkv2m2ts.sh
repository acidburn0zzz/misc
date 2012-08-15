#! /bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 mkv_file"
    exit 1
fi

BASENAME=`echo $1 | awk -F . '{print $1}'`
VIDEO=`mkvinfo $1 | grep -B 2 'type: video' | head -n1 | grep -o '[0-9]' | head -n1`
AUDIO=`mkvinfo $1 | grep -B 2 'type: audio' | head -n1 | grep -o '[0-9]' | head -n1`

cat << EOF > meta 
MUXOPT --no-pcr-on-video-pid --new-audio-pes --vbr  --vbv-len=500
V_MPEG4/ISO/AVC, "$1", fps=23.976, level=4.1, insertSEI, contSPS, ar=As source, track=$VIDEO, lang=und
A_AC3, "$1", track=$AUDIO, lang=eng"
EOF

tsMuxeR meta $BASENAME.m2ts

rm meta
