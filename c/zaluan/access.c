#include <unistd.h>
#include <string.h>
#include <stdio.h>



int main(int argc, char *argv[])
{
	if((access(argv[1], F_OK))==0) {
		printf("find file\n");
	} else {
		printf("this file is not exsit;");
	}

	return 0;
}
