#ifndef _WOLFSSLENCRYPTDECRYPT_H__
#define _WOLFSSLENCRYPTDECRYPT_H__

#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/openssl/ssl.h>
#include <wolfssl/openssl/evp.h>
#include <string>

#ifndef ENGINE
#define ENGINE void
#endif

int EVP_BytesToKey_ex(const EVP_CIPHER *type, const EVP_MD *md, 
	     const unsigned char *salt, const unsigned char *data, int datal,
	     int count, unsigned char *key, unsigned char *iv);


#endif


