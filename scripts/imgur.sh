#! /bin/bash

COUNT=1;

for i in $(wget $1 -O - | tidy 2>/dev/null | grep -o 'http://.*s\.jpg' | sed 's/s\.jpg/.jpg/'); do
    wget $i -O $(printf %03d $COUNT).jpg;
    let COUNT=COUNT+1;
done
