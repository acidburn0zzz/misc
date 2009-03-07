#! /bin/bash

cd ~/codes/cpp/sudoku/

if test ! -f sudoku.pro; then
    qmake -project -norecursive
fi

if test ! -f Makefile; then
    qmake -makefile -Wall -Wparser -Wlogic
fi

lupdate sudoku.pro > /dev/null

lrelease sudoku.pro > /dev/null

make > /dev/null && ./sudoku $@
