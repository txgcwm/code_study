#include <string.h>
#include <stdio.h>

#include "WsslEnDecrypt.h"



int main(int argc, char **argv)
{
	int decryptedtext_len, ciphertext_len = 48;
	unsigned char passwd[] = "01234567890123456";
  	unsigned char decryptedtext[64] = {0};
	unsigned char ciphertext[] = {	0x67, 0xee, 0x2, 0xf5, 0xe4, 0x7a, 0x68, 0x85, 
									0xbc, 0x10, 0xd4, 0xed, 0xb3, 0x7a, 0xce, 0x69, 
									0x1d, 0xda, 0xe1, 0x6b, 0xfa, 0xb4, 0x9a, 0x5e, 
									0xcb, 0xb4, 0x7, 0x2d, 0x99, 0xb1, 0x12, 0x72, 
									0x1a, 0x6b, 0x36, 0xca, 0x5b, 0xd3, 0xfb, 0x93, 
									0x94, 0xb5, 0x1c, 0x7c, 0x9c, 0xb, 0x72, 0xe5 };

	// unsigned char ciphertext[] = {0x67, 0xee, 0x2, 0xf5, 0xe4, 0x7a, 0x68, 0x85, 
	// 								0xbc, 0x10, 0xd4, 0xed, 0xb3, 0x7a, 0xce, 0x69, 
	// 								0x1d, 0xda, 0xe1, 0x6b, 0xfa, 0xb4, 0x9a, 0x5e, 
	// 								0xcb, 0xb4, 0x7, 0x2d, 0x99, 0xb1, 0x12, 0x72, 
	// 								0x21, 0x3a, 0x79, 0x57, 0x6d, 0xa7, 0x2e, 0xc1, 
	// 								0x18, 0x72, 0x7d, 0xa4, 0x27, 0xb3, 0xda, 0x5a, 
	// 								0x38, 0xf1, 0x57, 0x35, 0x91, 0x28, 0xbc, 0xea, 
	// 								0x11, 0xac, 0xe3, 0xc2, 0x64, 0xbf, 0x30, 0x83 };

	decryptedtext_len = WsslDecrypt(ciphertext, ciphertext_len, decryptedtext, passwd);  

  	decryptedtext[decryptedtext_len] = '\0';

  	printf("decryptedtext len: %d\n", decryptedtext_len);
  	printf("Decrypted text is:\n%s\n", decryptedtext);

	return 0;
}


// g++ -o wssltest TestWsslEnDecrypt.cpp WsslEnDecrypt.cpp -I install/wolfssl/include/ -L install/wolfssl/lib/ -lwolfssl -lpthread -static