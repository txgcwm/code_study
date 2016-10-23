#! /bin/bash

for f in $(find . -name '*.zip' -type f)
do
	unzip $f
done
