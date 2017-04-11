#include <boost/lexical_cast.hpp>
#include <openssl/aes.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "base64.h"



int main(int argc, char **argv)
{
	char encrypt_string[4096] = {0};
	AES_KEY aes;
	char key[17] = "quck7295abvdefgh";
	char iv[17] = "abcdefgh3762quck";
	std::string input_string = "45b00417-f7ca-4f53-bced-c1743d85604";
	int nLen = input_string.length();

	int nBei = nLen / AES_BLOCK_SIZE + 1;
	int nTotal = nBei * AES_BLOCK_SIZE;
	char *enc_s = (char*)malloc(nTotal);
	int nNumber;

	if (nLen % 16 > 0) {
		nNumber = nTotal - nLen;
	} else {
		nNumber = 16;
	}

	memset(enc_s, nNumber, nTotal);
	memcpy(enc_s, input_string.data(), nLen);

	if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0) {
		fprintf(stderr, "Unable to set encryption key in AES\n");
		exit(-1);
	}
  
	AES_cbc_encrypt((unsigned char *)enc_s, (unsigned char*)encrypt_string, nBei * 16, &aes, (unsigned char*)iv, AES_ENCRYPT);

	std::string enstr = boost::lexical_cast<std::string>(encrypt_string);

	size_t len;
	char value[128] = {0};
	base64_encode((char *)enstr.c_str(), enstr.size(), value, &len);

	printf("%s len(%d)\n", value, len);

	return 0;
}


