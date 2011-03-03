#! /bin/bash

lsof -n -P | grep 'Flash[[:alnum:]]\{6\}' | awk '{ print "/proc/" $2 "/fd/" substr($4, 1, length($4)-1) }'
