#! /bin/bash

for i in $( ls *.mp3 ); do
    lame --decode $i - |  lame -b 128 -m s -h - $i.128
done

rm *.mp3
rename 's/\.128//' *.128
