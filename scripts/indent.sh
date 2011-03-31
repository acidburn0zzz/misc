#!/bin/bash

PARAM="--k-and-r-style \
--braces-on-func-def-line \
--indent-level4 \
--tab-size8 \
--no-tabs \
--swallow-optional-blank-lines \
--line-length120 \
--space-special-semicolon \
--space-after-cast \
--else-endif-column1 \
--indent-label0"

# Fix indentation
indent $PARAM "$@"

# Remove all trailing whitespaces
sed 's/[ \t]*$//' -i "$@"

# Remove backups
rm -f *~
