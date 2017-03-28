#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>


typedef struct {
	int count;

} Element;

int main(int argc, char **argv)
{
	int data[] = {0x27, 0x2c, 0x30, 0x46, 0x48, 0x50, 0x61, 0x73, 0x82, 0x93, 0xa3, 0xaa, 0xb3, \
					0xc4, 0xd3, 0xe5, 0xf3, 0x106, 0x113, 0x127, 0x133, 0x148, 0x153};
	std::vector<int> vecNum;

	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		vecNum.push_back(data[i]);
	}

	for (int j = 0; j < vecNum.size(); ++j) {
		printf("%x ", vecNum[j]);
	}

	printf("\n");

	return 0;
}


