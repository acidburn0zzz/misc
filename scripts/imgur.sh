#! /bin/bash

COUNT=1;

if [ $# -ge 2 ]; then
    COUNT=$2
fi

for i in $(curl $1 2>/dev/null| grep -o '<div class="image".*>' | grep -Eo '[a-zA-Z0-9]{7}'); do
    wget http://i.imgur.com/$i.jpg -O $(printf %03d $COUNT).jpg;
    let COUNT=COUNT+1;
done
