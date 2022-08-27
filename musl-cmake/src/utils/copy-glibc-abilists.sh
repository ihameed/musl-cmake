#!/bin/sh

srcdir="$1/sysdeps/unix/sysv/linux"

if [ ! -d "$srcdir" ]; then
    echo "source directory \"$srcdir\" not found"
    exit 1
fi

if [ ! -d "$2" ]; then
    echo "destination directory \"$2\" not found"
    exit 1
fi

exec rsync -am --include='*.abilist' --include='*/' --exclude='*' "$srcdir/" "$2"
