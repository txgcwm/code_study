#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// #define WOLFSSL

#ifdef WOLFSSL
#include <wolfssl/wolfcrypt/des3.h>
#else
#include <openssl/bio.h>
#include <openssl/evp.h>
#endif



#ifdef WOLFSSL
long DesEncrypt(u_char *inbuf, u_char **outbuf, size_t inlen, const u_char *key, const u_char *iv)
{
	int ret;
    Des enc;
    int blocks = inlen / DES_BLOCK_SIZE;
    int left = inlen % DES_BLOCK_SIZE;

    // printf("blocks: %d, left: %d\n", blocks, left);

    ret = wc_Des_SetKey(&enc, key, iv, DES_ENCRYPTION);
    if (ret != 0) {
        return -1;
    }

    for(int i = 0; i < blocks; i++) {
    	ret = wc_Des_EcbEncrypt(&enc, *outbuf + i * DES_BLOCK_SIZE, inbuf + i * DES_BLOCK_SIZE, DES_BLOCK_SIZE);
	    if (ret != 0) {
	        return -1;
	    }
    }

    if(left > 0) {
    	char other[DES_BLOCK_SIZE] = {0};

    	memcpy(other, inbuf + blocks * DES_BLOCK_SIZE, left);
    	for(int i = left; i < DES_BLOCK_SIZE; i++) {
    		other[i] = DES_BLOCK_SIZE - left;
    	}

    	ret = wc_Des_EcbEncrypt(&enc, *outbuf + blocks * DES_BLOCK_SIZE, (const byte*)other, DES_BLOCK_SIZE);
	    if (ret != 0) {
	        return -1;
	    }
    }

    return strlen((const char *)*outbuf);
}
#else
long DesEncrypt(u_char *inbuf, u_char **outbuf, size_t inlen, const u_char *key, const u_char *iv) 
{
	long outlen = 0;
	u_char *dst = NULL;

	BIO *mbio = BIO_new(BIO_s_mem()); 
	BIO *cbio = BIO_new(BIO_f_cipher()); 
	BIO_set_cipher(cbio, EVP_des_ecb(), key, iv, 1); 
	BIO *bio = BIO_push(cbio, mbio); 
	BIO_write(bio, inbuf, (int)inlen); 
	BIO_flush(bio); 
	
	outlen = BIO_get_mem_data(mbio, &dst);
	if(outlen > 0 && dst != NULL) {
		memcpy(*outbuf, dst, outlen);
	}

	BIO_free_all(bio);

	return outlen; 
}
#endif

int main(int argc, char **argv)
{
	int len = 0;
	const u_char *key = (const u_char *)"0123456789ABCDEF";
	const u_char *iv = (const u_char *)"12345678";
	u_char *out = (u_char *)malloc(100);

	len = DesEncrypt((u_char *)argv[1], (u_char **)&out, strlen((const char *)argv[1]), key, iv);
	for(int i = 0; i < len; i++) {
		printf("%d ", out[i]);
	}

	printf("\n");

	free(out);

	return 0;
}


// g++ ssldes.cpp -lssl -lcrypto
// g++ ssldes.cpp -I wolfssl-master/ -L wolfssl-master/src/.libs/ -lwolfssl -static
// wolfssl github url: https://github.com/wolfSSL/wolfssl
// how to configure wolfssl: ./configure --enable-des3 --enable-openssh --enable-static
