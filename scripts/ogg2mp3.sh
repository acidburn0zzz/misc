#! /bin/bash

for i in $( ls *.ogg ); do
    WAV=`echo $i | awk -F . '{print $1 ".wav"}'`
    MP3=`echo $i | awk -F . '{print $1 ".mp3"}'`
    oggdec $i -o $WAV
    lame -b 128 -B 320 -m s -h -V 2 $WAV $MP3

    rm $WAV
done
