#! /bin/bash -e

tolower

for i in $(ls *.zip); do
    name=`echo $i | awk -F . '{print $1}'`;
    mkdir $name;
    unzip -q $i -d $name;
    rm $i;
done
