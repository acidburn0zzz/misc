#! /bin/bash

tolower
rename 's/_s\.zip/\.zip/' *.zip

for i in $(ls *.zip); do
    name=`echo $i | awk -F . '{print $1}'`;
    mkdir $name;
    unzip -q $i -d $name;
    if [ -e $name/$name.mpg ];
    then
        mv $name/$name.mpg .;
        rm -rf $name;
    fi
    if [ -e $name/*.html ];
    then
        mv $name/*.html .;
    fi
    rm $i;
done
