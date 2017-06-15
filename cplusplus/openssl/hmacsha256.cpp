#include <openssl/hmac.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>



void GetStringTime(char* day, int size)
{
	time_t t = time(0); 
     
    // strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
    strftime(day, size, "%Y%m%d", gmtime(&t)); 

    return;
}

void HMAC_Sha256_Encrypt(void* key, int key_len, unsigned char* text,
							unsigned char* cipher, unsigned int *plen)
{
    HMAC_CTX ctx;

    HMAC_CTX_init(&ctx);

    // e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
    HMAC_Init_ex(&ctx, key, key_len, EVP_sha256(), NULL);
    HMAC_Update(&ctx, text, strlen((const char*)text));
    HMAC_Final(&ctx, cipher, plen);
    HMAC_CTX_cleanup(&ctx);

    printf("HMAC digest: ");

    for (int i = 0; i < *plen; i++) {
        printf("%02x", cipher[i]);
    }
 
    printf("\n");

    return;
}

void SignatureWithHmacSha256(unsigned char* key, unsigned char* data, unsigned char* sign)
{
	unsigned int len = 0;
	char day[20] = {0};
 	unsigned char DateKey[32] = {0};
 	unsigned char DateRegionKey[32] = {0};
 	unsigned char DateRegionServiceKey[32] = {0};
 	unsigned char SigningKey[32] = {0};

 	GetStringTime(day, sizeof(day));

 	HMAC_Sha256_Encrypt((void *)key, strlen((const char*)key), (unsigned char*)day, DateKey, &len);
 	HMAC_Sha256_Encrypt((void *)DateKey, len, (unsigned char*)"china", DateRegionKey, &len);
 	HMAC_Sha256_Encrypt((void *)DateRegionKey, len, (unsigned char*)"xmpp", DateRegionServiceKey, &len);
 	HMAC_Sha256_Encrypt((void *)DateRegionServiceKey, len, (unsigned char*)"aws4_request", SigningKey, &len);

 	HMAC_Sha256_Encrypt((void *)SigningKey, len, data, sign, &len);

	return;
}

int main(int argc, char **argv)
{
    unsigned char key[] = "123456";
    unsigned char data[] = "hello";
    unsigned char sign[32];
    
    SignatureWithHmacSha256(key, data, sign);

    for(int i = 0; i < 32; i++) {
    	printf("%02x", (unsigned char)sign[i]);
    }
 
    printf("\n");

    return 0;
}


// g++ -o test hmacsha256.cpp -lssl -lcrypto
// http://docs.aws.amazon.com/AmazonS3/latest/API/sigv4-authentication-HTTPPOST.html