#! /bin/bash

if [ $# -gt 1 ]; then
    nohup $* > /dev/null 2>&1 &
fi
