#!/bin/sh

make clean
rm -f CMakeCache.txt Makefile
CMAKE_PREFIX_PATH=~/local/llvm cmake . 
make 

if [ $? -ne 0 ]; then
  echo "make failed"
  exit 1
fi

