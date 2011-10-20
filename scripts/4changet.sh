#! /bin/bash

if [ $# -lt 1 ];
then
    echo "You must specify thread";
    return 1;
fi

curl -s $1 | sed -r 's/.*href="([^"]*).*/\1\n/g' | grep images | xargs wget
