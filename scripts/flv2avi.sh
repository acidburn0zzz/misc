#! /bin/bash

for i in `ls *.flv`; do
    ffmpeg -i $i -pass 1 -r 29.97 -b 256k \
        -ar 24000 -ab 96k -acodec libmp3lame -vcodec libxvid -aspect 4:3 `echo $i | awk -F . '{print $1 ".avi"}'`
    rm `echo $i | awk -F . '{print $1 ".avi"}'`
    ffmpeg -i $i -pass 2 -r 29.97 -b 256k \
        -ar 24000 -ab 96k -acodec libmp3lame -vcodec libxvid -aspect 4:3 `echo $i | awk -F . '{print $1 ".avi"}'`
done
rm ffmpeg*.log
