#include <string.h>
#include <stdio.h>

#include "PatternMatch.h"



int main(int argc, char **argv)
{
	// int data[] = {73, 74, 77, 79, 91, 100, 124, 132, 149, 167, 182, 199, 215,\
	// 				231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};

	int data[] = {73, 74, 77, 79, 91, 100, 124, 132, 149, 167, 182, 199, 215,\
					231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};
	std::vector<int> listNum;
	char pattern[] = {0, 1, 4, 6};
	CPatternMatch *pmatch = new CPatternMatch(pattern);	

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		listNum.push_back(data[i]);

		printf("raw data count: %d\n", i);
		pmatch->QueryIdentificationPackage(listNum);
		pmatch->PrintMagicList();
	}

	return 0;
}