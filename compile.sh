#!/bin/sh

DRB_ROOT=.
PROOT="mygame/extensions"

mkdir -p mygame/native/macos
mkdir -p mygame/native/windows
mkdir -p mygame/native/linux

FILES="mygame/extensions/dragondb.c"

zig cc -O3 -march=native \
  -isystem $DRB_ROOT/include -isystem $DRB_ROOT -I$PROOT/sqlite \
  -fPIC -shared $FILES -o mygame/native/macos/dragondb.dylib -lm -Wl,-undefined -Wl,dynamic_lookup 