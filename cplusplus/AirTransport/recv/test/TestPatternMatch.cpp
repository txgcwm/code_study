#include <string.h>
#include <stdio.h>
#include <string>

#include "PatternMatch.h"



int main(int argc, char **argv)
{
	// int data[] = {73, 74, 77, 79, 91, 100, 124, 132, 149, 167, 182, 199, 215,\
					231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};
	// 97, 98, 101, 103, 115, 124, 148, 156, 173, 191, 206, 223, 239, 255,
	// 272, 287, 305, 319, 338, 351, 371, 383, 404, 415

	// int data[] = {94, 97, 98, 99, 100, 101, 103, 105, 115, 117, 124, 126, 148, 150, 156, 158, 173, \
	// 				175, 191, 193, 206, 208, 223, 225, 239, 241, 255, 257, 272, 274, 279, 287, 289, \
	// 				305, 307, 319, 321, 338, 340, 351, 353, 371, 373, 383, 385, 404, 406, 415, 417};

	// 68, 91, 96, 122, 222, 
	int data[] = {68, 73, 74, 77, 79, 91, 96, 100, 122, 124, 132, 149, 167, 182, 199, 215, 222, 
					231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};

	// int data[] = {71, 72, 73, 74, 75, 77, 79, 91, 100, 124, 132, 149, 167, 182, 199, 215,\
	// 				231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};
	std::vector<int> vecNum;
	char pattern[] = {0, 1, 4, 6};
	CPatternMatch *pmatch = new CPatternMatch(pattern);	

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		vecNum.push_back(data[i]);

		// printf("raw data count: %d\n", i);
		pmatch->QueryIdentificationPackage(vecNum);

		std::string raw = "";
		if(pmatch->DecodeTransmitRecord(vecNum, raw)) {
			printf("get transmit record(%s) success!\n", raw.c_str());
			break;
		}

		// pmatch->PrintMagicList();
	}

	return 0;
}