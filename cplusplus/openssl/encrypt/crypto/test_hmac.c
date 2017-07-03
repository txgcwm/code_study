#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "sha256.h"
#include "hmac.h"


void GetStringTime(char* day, int size)
{
	time_t t = time(0); 
     
    // strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
    strftime(day, size, "%Y%m%d", gmtime(&t)); 

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

 	hmac_sha256((unsigned char*)day, strlen(day), key, strlen((const char*)key), DateKey);
 	hmac_sha256((unsigned char*)"china", strlen("china"), DateKey, SHA256_HASH_LEN, DateRegionKey);
 	hmac_sha256((unsigned char*)"xmpp", strlen("xmpp"), DateRegionKey, SHA256_HASH_LEN, DateRegionServiceKey);
 	hmac_sha256((unsigned char*)"aws4_request", strlen("aws4_request"), 
 					DateRegionServiceKey, SHA256_HASH_LEN, SigningKey);

 	hmac_sha256(data, strlen(data), SigningKey, SHA256_HASH_LEN, sign);

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