#include <string.h>
#include <stdio.h>

#include "AirData.h"



int main(int argc, char **argv)
{
	CAirData airData;
	int data[] = {73, 74, 77, 79, 91, 100, 124, 132, 149, 167, 183, 199, 217,\
					231, 251, 263, 276, 292, 308, 324, 340, 356, 372, 388};
	// int data[] = {73, 75, 74, 74, 74, 77, 79, 91, 100, 124, 132, 149, 167, 183, 199, 217,\
	// 				231, 251, 263, 276, 292, 308, 324, 340, 356, 372, 388};

	for(int i = 4; i < sizeof(data)/sizeof(data[0]); i++) {
		printf("%x ", data[i] - data[0] + 21);
	}

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		airData.Analysis(data[i]);
	}

	return 0;
}

// g++ AirData.cpp TestAirData.cpp -o parse

