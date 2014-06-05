#!/bin/sh

make clean
rm -f CMakeCache.txt Makefile
CMAKE_PREFIX_PATH=~/local/llvm cmake . 
make 

if [ $? -ne 0 ]; then
  echo "make failed"
  exit 1
fi

./vm
~/local/llvm/bin/lli main.ll

if [ $? -eq 34 ]; then 
  echo "main.ll succeded"; 
  exit 0
else 
  echo "main.ll failed"; 
  exit 1
fi
