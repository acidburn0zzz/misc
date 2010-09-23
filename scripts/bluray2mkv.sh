#! /bin/bash

VB=10240k
SIZE=1920x1080
CROP_T=140
CROP_B=140
ASPECT=1920:800
VIDEO_IN=video.264
TITLE="Ratatouille"
#~ len=
#~ size=

ffmpeg -i $VIDEO_IN -vcodec libx264 -vb $VB -s $SIZE -croptop $CROP_T -cropbottom $CROP_B -r 24000/1001 -aspect $ASPECT -threads 4 -pass 1 -y video.h264
ffmpeg -i $VIDEO_IN -vcodec libx264 -vb $VB -s $SIZE -croptop $CROP_T -cropbottom $CROP_B -r 24000/1001 -aspect $ASPECT -threads 4 -pass 2 -y video.h264

mkvmerge -o video.mkv \
    --language 0:eng --track-name "0:x264 video - 10240 kB/s" --default-duration 0:24000/1001fps -d 0 -A -S video.h264 \
    --language 0:eng --track-name "0:English ac3 audio - 640 kB/s" -a 0 -D -S audio.ac3 \
    --language 0:eng --track-name "0:English" -s 0 -D -A eng.srt \
    --language 0:fre --track-name "0:French" -s 0 -D -A fre.srt \
    --track-order 0:0,1:0,2:0,3:0 --title $TITLE --chapters chapters.xml

