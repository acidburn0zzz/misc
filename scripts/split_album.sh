#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Usage: $0 cue_file album_file"
    exit 1
fi

cuebreakpoints $1 | shnsplit -o flac $2
