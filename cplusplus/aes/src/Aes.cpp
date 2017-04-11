#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/ssl.h>
#include <string>

#include "base64.h"



static int AesCbcPkcs5Decrypt(const byte *key, const int keysz, const byte *iv, 
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

static int AesCbcPkcs5Encrypt(const byte *key, const int keysz, const byte *iv, 
						const byte *plain, const int plainsz, byte *cipher, int &outlen)
{
	int ret = -1;
	Aes dec;
	byte *temp = NULL;
	int left = plainsz % AES_BLOCK_SIZE;

	outlen = plainsz + AES_BLOCK_SIZE - left;

	temp = (byte *)malloc(outlen);

	memset(temp, AES_BLOCK_SIZE - left, outlen);

	memcpy(temp, plain, plainsz);

	do {
		ret = wc_AesSetKey(&dec, key, keysz, iv, AES_ENCRYPTION);
		if (ret != 0) {
			break;
		}

		ret = wc_AesCbcEncrypt(&dec, cipher, temp, outlen);
		if (ret != 0) {
			outlen = 0;
			break;
		}
	} while(0);

	free(temp);
	temp = NULL;

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

int EncypptBase64Aes(const char *key, const char *iv, const char *text, std::string &bval)
{
	byte *aesenc = NULL;
	int alen = strlen(text) + AES_BLOCK_SIZE - strlen(text) % AES_BLOCK_SIZE;

	aesenc = (byte *)malloc(alen);

	AesCbcPkcs5Encrypt((const byte*)key, 16, (const byte*)iv, (const byte*)text, strlen(text), aesenc, alen);

	size_t blen = (alen * 4) / 3;
	char *value = (char *)malloc(blen);

	base64_encode((char *)aesenc, alen, value, &blen);
	printf("value: %s:%s, len(%d, %d)\n", aesenc, value, blen, strlen(value));

    bval.assign(value, blen);

	free(value);
	value = NULL;

	free(aesenc);
	aesenc = NULL;

	return 0;
}


