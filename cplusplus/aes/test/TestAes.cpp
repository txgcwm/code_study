#include <string.h>
#include <stdio.h>

#include "Aes.h"



#define AESKEY  "pZuY$bxkE5GZXPM@"
#define AESIV   "tt$OwY*lWLb3sr9j"

int main(int argc, char **argv)
{
	char out[50] = {0};

	DecryptBase64Aes(AESKEY, AESIV, "XVFG2u3B+QMMuvlPuBfYgT6zSlXuzCmyYrWuu77DdBc=", out);

	printf("%s\n", out);

	return 0;
}


