#include <stdio.h>


int main(int argc, char** argv)
{
	int i, total;
	int max[2] = {10, 9};
	
	for(int j = 0; j < 2; j++) {
		total = 0;
	
		for(i = 0; i < max[j]; i++) {
			total += i;
		}
		
		if(total != 45) {
			printf("fail!\n");
		} else {
			printf("success!\n");
		}
	}
	
	return 0;
}

/*
g++ -fprofile-arcs -ftest-coverage -o gcov_test gcov_test.cpp
./gcov_test
gcov gcov_test.cpp
ls
cat gcov_test.cpp.gcov 
*/