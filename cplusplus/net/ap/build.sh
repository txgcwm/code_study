#!/bin/sh


if [ -d cmake_build ]; then
	rm -rf cmake_build
fi

mkdir cmake_build
cd cmake_build
cmake ..
make
cd ..