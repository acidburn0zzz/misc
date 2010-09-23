#! /bin/bash

OPTIONS=
#~ vb="256k"
#~ vb="320k"
vb="384k"
#~ vb="448k"
#~ vb="512k"
#~ vb="576k"
#~ vb="640k"
#~ vb="704k"
#~ vb="768k"

if [ $# -lt 1 ]; then
    echo "Usage: $0 <infile> [audiobitrate] [fps] [aspect]"
    exit -1
fi

file=$1

if [ $# -ge 2 ]; then
    OPTIONS+="-ab $2k "
fi

if [ $# -ge 3 ]; then
    OPTIONS+="-r $3 "
else
    OPTIONS+="-r 30000/1001 "
fi

if [ $# -ge 4 ]; then
    OPTIONS+="-aspect $4 -s 480x272 "
else
    #NTSC
    #~ OPTIONS+="-aspect 16:9 -s 360x270 -padbottom 2 -padleft 60 -padright 60 "
    
    #PAL
    OPTIONS+="-aspect 16:9 -s 340x272 -padleft 70 -padright 70 "
    #~ OPTIONS+="-aspect 16:9 -s 332x272 -padleft 74 -padright 74 "
fi

ffmpeg -i $file -vb $vb -vcodec libx264 \
    -ar 44100 -ac 2 -acodec libfaac -threads 4 -y -pass 1 \
    $OPTIONS `echo $file | awk -F . '{print $1 ".m4v"}'`
ffmpeg -i $file -vb $vb -vcodec libx264 \
    -ar 44100 -ac 2 -acodec libfaac -threads 4 -y -pass 2 \
    $OPTIONS `echo $file | awk -F . '{print $1 ".m4v"}'`

rm *.log
