#! /bin/bash

for i in $( ls *.wav ); do
    lame -b 128 -B 320 -m s -h -V 2 $i `echo $i | awk -F . '{print $1 ".mp3"}'`
done
