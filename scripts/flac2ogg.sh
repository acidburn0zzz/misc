#! /bin/bash

for i in $( ls *.flac ); do
    flac -cd $i | oggenc -q 7 -o `echo $i | awk -F . '{print $1 ".ogg"}'` -
done
