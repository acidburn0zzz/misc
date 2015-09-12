#! /bin/zsh -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 input vb ab"
    exit 1
fi

INPUT="$1"
OUTPUT="${INPUT%.*}.mp4"
VB="${2:-8192}k"
AB="${3:-128}k"

if [ -e "$OUTPUT" ]; then
    echo "$OUTPUT already exists"
    exit 2
fi

ffmpeg -i "$INPUT" -acodec libmp3lame -ab "$AB" -vcodec libx264 -vb "$VB" -threads 8 -y -pass 1 "$OUTPUT"
ffmpeg -i "$INPUT" -acodec libmp3lame -ab "$AB" -vcodec libx264 -vb "$VB" -threads 8 -y -pass 2 "$OUTPUT"

rm ffmpeg2pass*
