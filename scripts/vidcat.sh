#! /bin/sh

if [ $# -lt 2 ]; then
    echo "Usage: $0 input(s) output"
    exit 1
fi

LISTFILE=$(mktemp -p .)

while [ $# != 0 ]; do
    if [ $# -gt 1 ]; then
        echo "file '$1'" >> $LISTFILE
    else
        ffmpeg -f concat -i $LISTFILE -c copy $1
    fi
    shift 1
done

rm $LISTFILE
