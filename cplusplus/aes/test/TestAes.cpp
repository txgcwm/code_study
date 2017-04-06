#include <string.h>
#include <stdio.h>

#include "Aes.h"



#define AESKEY  	"pZuY$bxkE5GZXPM@"
#define AESIV   	"tt$OwY*lWLb3sr9j"
#define BASE64VAL	"XVFG2u3B+QMMuvlPuBfYgT6zSlXuzCmyYrWuu77DdBc="

int main(int argc, char **argv)
{
	char out[50] = {0};

	DecryptBase64Aes(AESKEY, AESIV, BASE64VAL, out);

	printf("%s\n", out);

	std::string bval;

	EncypptBase64Aes(AESKEY, AESIV, out, bval);

	printf("base64 value: %s\n", bval.c_str());

	if(memcmp(bval.c_str(), BASE64VAL, bval.size()) == 0) {
		printf("success!\n");
	} else {
		printf("fail\n");
	}

	return 0;
}


