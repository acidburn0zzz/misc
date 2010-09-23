#! /bin/bash

OUT_FILE=~/storage/out

date > $OUT_FILE

rsync -acv --delete ~/codes ~/storage/backup >> $OUT_FILE 2>&1
rsync -acv --delete ~/documents ~/storage/backup >> $OUT_FILE 2>&1
rsync -acv --delete ~/images ~/storage/backup >> $OUT_FILE 2>&1
rsync -acv --delete ~/public_html ~/storage/backup/ >> $OUT_FILE 2>&1
rsync -acv --delete ~/scripts ~/storage/backup/ >> $OUT_FILE 2>&1
