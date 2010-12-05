#! /bin/bash

# Find all directories & subdirectories
find -type d -exec chmod 755 '{}' \;

# Find all Files
find -type f -exec chmod 644 '{}' \;
