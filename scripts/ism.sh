#! /bin/bash

rename -v 's/_s\.zip/\.zip/' *.zip

for i in $(ls *.zip); do
    name=`echo $i | awk -F . '{print $1}'`;
    mkdir $name;
    unzip $i -d $name;
    mv $name/*.mpg .;
    mv $name/*.html .;
    rm $i;
done
