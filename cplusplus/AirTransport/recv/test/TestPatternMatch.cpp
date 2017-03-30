#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <string>

#include "PatternMatch.h"



bool lessmark(const int& ele1, const int& ele2)
{
    return ele1 < ele2;
}

int StoreRawData()
{
	int data[] = {416, 73, 74, 77, 79, 94, 114, 131, 138, 156, 164, 181, 199, 214, 231, 78, 81, 83, 77, 247, 263, 
					280, 295, 313, 327, 346, 359, 379, 391, 412, 423, 317, 81, 83, 363, 61, 56, 53, 98, 150, 203,
					235, 251, 284, 299, 317, 331, 383, 395, 416, 395, 82, 96, 602, 78, 456, 558, 540, 569, 619,
					111, 125, 166, 168, 185, 218, 350, 427, 474, 505, 523, 587, 98, 118, 135, 142, 160, 168, 185,
					203, 218, 235, 251, 267, 284, 299, 331, 350, 383, 267, 363, 450, 491, 427};

	std::vector<int> vec;

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		int len = data[i];
	  	std::vector<int>::iterator lowIter;
	  	lowIter = std::lower_bound(vec.begin(), vec.end(), len);

	  	if(vec.size() == 0 || *lowIter != len) {

	    	vec.insert(lowIter, len);
	    	printf("insert len(%d)\n", len);
	  	} else {
	  		printf("+++++++++++++++++++++++  had insert(%d)\n", len);
	  	}
  	}

  // TRACEF("no need to insert (%d)!\n", len); // do nothing

  return -1;
}

int main(int argc, char **argv)
{
	// int data[] = {77, 78, 153, 171, 186, 203, 219, 235, 252, 267, 285, 299, 318, 331, 351, 363, 384, 395, 81,
	// 				295, 314, 327, 347, 167, 182, 199, 215, 231, 248, 263, 281, 359, 380, 391, 83,
	// 				95, 104, 128, 136};

	// int data[] = {363, 383, 395, 416, 427, 138, 156, 181, 199, 214, 231, 247, 77, 78, 81,
	// 				83, 98, 118, 135, 142, 160, 168, 185, 203, 218, 235, 251, 267, 284, 299, 317, 331, 350};

	// int data[] = {160, 168, 185, 203, 218, 235, 251, 267, 73, 74, 77, 79, 94, 114, 131, 138, 156,
	// 				164, 181, 199, 214, 231, 247, 263, 284, 299, 317, 331, 363, 383, 280, 295, 327,
	// 				346, 359, 379, 416, 83, 81, 391, 412, 423, 78, 98, 135, 142, 313};

	// int data[] = {218, 74, 77, 79, 73, 94, 114, 131, 138, 164, 181, 199, 235, 214, 280, 295, 313,
	// 				327, 346, 359, 379, 391, 412, 350, 363, 383, 395, 416, 284, 423, 156, 299, 317,
	// 				231, 247, 263, 427, 83, 78, 135, 160, 168, 185, 203, 251, 331, 81, 267, 98, 142};

	int data[] = {416, 73, 74, 77, 79, 94, 114, 131, 138, 156, 164, 181, 199, 214, 231, 78, 81, 83, 247,
					263, 280, 295, 313, 327, 346, 359, 379, 391, 412, 423, 317, 61, 56, 53, 98,
					150, 395, 82, 96, 602, 456, 558, 540, 569, 619, 111, 125, 166, 185, 218, 350, 427, 474,
					505, 523, 587, 118, 135, 142, 160, 168, 203, 235, 251, 284, 299, 331, 383, 267, 363, 450, 491};

	std::vector<int> vecNum;
	char pattern[] = {0, 1, 4, 6};
	CPatternMatch *pmatch = new CPatternMatch(pattern);	

	// StoreRawData();

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		vecNum.push_back(data[i]);

		std::sort(vecNum.begin(), vecNum.end(), lessmark);

		// printf("raw data count: %d\n", i);
		pmatch->QueryIdentificationPackage(vecNum);

		std::string raw = "";
		if(pmatch->DecodeTransmitRecord(vecNum, raw)) {
			printf("get transmit record(%s) success!\n", raw.c_str());
			break;
		}

		pmatch->PrintMagicList();
	}

	return 0;
}

// cat temp.txt | awk -F ' ' '{print $7}' | awk -F '(' '{print $2}' | sed 's/)//g'
// 27  30  48  50  61  73  82  93  a3  b3  c4  d3  e5  f3 106 113 127 133 148 153