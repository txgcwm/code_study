#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <vector>


unsigned char crc8(const unsigned char *data, int len)
{
    int i = 0;
    unsigned char crc = 0x00;

    while(len-- > 0) {
        unsigned char extract = data[i++];

        for (unsigned char tempI = 8; tempI != 0; tempI--) {
            unsigned char sum = (unsigned char) ((crc & 0xFF) ^ (extract & 0xFF));
            sum = (unsigned char) ((sum & 0xFF) & 0x01);
            crc = (unsigned char) ((crc & 0xFF) >> 1);

            if (sum != 0) {
                crc = (unsigned char)((crc & 0xFF) ^ 0x8C);
            }

            extract = (unsigned char) ((extract & 0xFF) >> 1);
        }
    }

    return (crc & 0xFF);
}

bool shengxu(unsigned char e1, unsigned char e2)
{
	return e1 > e2;
}

int main(int argc, char **argv)
{
	unsigned char testData[][8] = {{49, 255, 51, 63, 49, 63, 51, 52},
									{239, 242, 51, 63, 53, 63, 51, 52},
									{49, 50, 63, 63, 63, 63, 51, 52},
									{239, 255, 63, 52, 49, 63, 55, 52},
									{49, 242, 63, 52, 53, 63, 55, 52},
									{239, 50, 51, 52, 63, 63, 55, 52},
									{49, 63, 63, 52, 49, 50, 63, 52},
									{239, 50, 63, 52, 53, 50, 63, 52},
									{49, 242, 51, 52, 63, 50, 63, 52},
									{63, 50, 63, 48, 49, 54, 63, 52},
									{225, 63, 63, 48, 53, 54, 63, 52},
									{63, 255, 51, 48, 63, 54, 63, 52},
									{49, 63, 63, 63, 49, 54, 51, 56},
									{239, 50, 63, 63, 53, 54, 51, 56},
									{49, 242, 51, 63, 63, 54, 51, 56},
									{225, 50, 51, 48, 49, 50, 55, 56},
									{63, 63, 51, 48, 53, 50, 55, 56},
									{225, 255, 63, 48, 63, 50, 55, 56},
									{239, 63, 51, 52, 49, 54, 55, 56},
									{49, 50, 51, 52, 53, 54, 55, 56},
									{239, 242, 63, 52, 63, 54, 55, 56},
									{63, 242, 51, 48, 49, 63, 63, 56},
									{225, 255, 51, 48, 53, 63, 63, 56},
									{63, 63, 63, 48, 63, 63, 63, 56},
									{49, 63, 51, 52, 49, 63, 51, 63},
									{239, 50, 51, 52, 53, 63, 51, 63},
									{49, 242, 63, 52, 63, 63, 51, 63},
									{239, 63, 63, 63, 49, 63, 55, 63},
									{49, 50, 63, 63, 53, 63, 55, 63},
									{239, 242, 51, 63, 63, 63, 55, 63},
									{49, 255, 63, 63, 49, 50, 63, 63},
									{239, 242, 63, 63, 53, 50, 63, 63},
									{49, 50, 51, 63, 63, 50, 63, 63}};

	int i = 0;
	std::vector<unsigned short> vec;

	for(i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
		unsigned short crc16 = crc_ccitt(testData[i], 8);
		printf("%d\n", crc16);

		vec.push_back(crc16);
	}

	std::sort(vec.begin(), vec.end(), shengxu);

	printf("i: %d, size: %d\n", i, vec.size());

	// for(int i = 0; i < sizeof(testData) / sizeof(testData[0]); i++) {
	// 	printf("%d\n", crc8(testData[i], 8));
	// }

	return 0;
}


