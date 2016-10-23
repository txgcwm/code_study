#! /bin/bash

for f in $(find . -name '*.c' -or -name '*.cpp' -or -name '*.h' -type f)
do
	indent -npro -kr -i4 -ts4 -sob -l80 -ss -ncs -cp1 $f
done

# after formate the code,we need to rm '*.orig' files
for f in $(find . -name '*~' -type f)
do
	rm $f
done
