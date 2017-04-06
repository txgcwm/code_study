#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/ssl.h>

#include "base64.h"



int AesCbcPkcs5Decrypt(const byte *key, const int keysz, const byte *iv, 
						const byte *cipher, const int ciphersz, byte *plain, int &outlen)
{
	int ret = -1;
	Aes dec;

	do {
		ret = wc_AesSetKey(&dec, key, keysz, iv, AES_DECRYPTION);
		if (ret != 0) {
			break;
		}

		ret = wc_AesCbcDecrypt(&dec, plain, cipher, ciphersz);
		if (ret != 0) {
			outlen = 0;
			break;
		}

		outlen = ciphersz - plain[ciphersz - 1];
		plain[outlen] = '\0';
	} while(0);

	return ret;
}

int DecryptBase64Aes(const char *key, const char *iv, const char *enc, char *value)
{
	char *bdecode = NULL;
	int size = 0;
    size_t boutlen = 0;
    size_t len = 0;
    size_t enc_len = strlen(enc);

    len = 3 * enc_len / 4;
    bdecode = (char *)malloc(len);
    
    bzero(bdecode, len);

    base64_decode(enc, enc_len, bdecode, &boutlen);

	AesCbcPkcs5Decrypt((const byte*)key, 16, (const byte*)iv, (const byte*)bdecode, 
						boutlen, (byte *)value, size);

	free(bdecode);
	bdecode = NULL;

	return 0;
}

int EncypptBase64Aes(const char *key, const char *iv, const char *text, char *value)
{

	return 0;
}


// http://www.bubuko.com/infodetail-1128539.html