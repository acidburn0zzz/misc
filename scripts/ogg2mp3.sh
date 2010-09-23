#! /bin/bash

for i in $( ls *.ogg ); do
    oggdec $i -o - | lame -b 128 -B 320 -m s -h -V 2 - `echo $i | awk -F . '{print $1 ".mp3"}'`
done
