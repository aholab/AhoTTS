#!/bin/bash
#check if cmake is installed
command -v cmake >/dev/null && echo "cmake Found." || { echo "cmake not Found. Install the package, please"; exit 1;}

PROGRAMS="libhtts tts"

for i in $PROGRAMS; do
	cd $i
	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=../..
	make && make install
	make clean
	cd ..
	cd ..
done;
