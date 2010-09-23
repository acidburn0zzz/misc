#! /bin/bash

for i in $( ls *.mp3 ); do
    lame --decode $i - | oggenc -q 7 -o `echo $i | awk -F . '{print $1 ".ogg"}'` -
done
