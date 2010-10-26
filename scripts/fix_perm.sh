#! /bin/bash

# Find all directories & subdirectories
for i in $(find -type d); do
    chmod 755 $i;
done

# Find all Files
for i in $(find -type f); do
    chmod 644 $i;
done
