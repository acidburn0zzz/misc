#! /bin/bash

VB=k
SIZE=
ASPECT=
TITLE=
#~ len=
#~ size=

if [ -f audio.dts ]; then
    ffmpeg -i audio.dts -ab 448k audio.ac3
fi

ffmpeg -i video.x264 -vcodec libxvid -vb $VB -s $SIZE -r 24000/1001 -aspect $ASPECT -threads 4 -pass 1 -y video.avi
ffmpeg -i video.x264 -vcodec libxvid -vb $VB -s $SIZE -r 24000/1001 -aspect $ASPECT -threads 4 -pass 2 -y video.avi

ffmpeg -i video.avi -i audio.ac3 -acodec copy -vcodec copy -y $TITLE.avi

#~ #Creation d'un sample pour tester les sous-titres
ffmpeg -i $TITLE.avi -acodec copy -vcodec copy -t 300 -y $TITLE.sample.avi

if [ -f subs.srt ]; then
    mv subs.srt $TITLE.srt
fi

subs $TITLE.srt -s 5:00 -c srt -o $TITLE.sample.srt
mv $TITLE.sample.1.srt $TITLE.sample.srt
rm $TITLE.sample.2.srt
