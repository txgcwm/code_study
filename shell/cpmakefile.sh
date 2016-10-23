#! /bin/bash

for f in $(find . -maxdepth 1 -type d)
do
	cp Makefile $f
done
