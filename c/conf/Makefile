test:test.o inifile.o
	gcc -o test test.o inifile.o
test.o:test.c inifile.h
	gcc -c test.c
inifile.o:inifile.c inifile.h
	gcc -c inifile.c
clean:
	rm -f *.o test
