#include <openssl/hmac.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>



void HMAC_Sha256_Encrypt(std::string key, std::string text, std::string &ciphertext)
{
	unsigned char result[128] = {0};
    unsigned int len = 0;
    HMAC_CTX ctx;

    memset(&ctx, 0, sizeof(HMAC_CTX));

    HMAC_CTX_init(&ctx);

    // e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
    HMAC_Init_ex(&ctx, key.c_str(), key.size(), EVP_sha256(), NULL);
    HMAC_Update(&ctx, (unsigned char*)&text[0], text.size());
    HMAC_Final(&ctx, result, &len);
    HMAC_CTX_cleanup(&ctx);
 
    ciphertext = (char *)result;

    printf("HMAC digest: ");
 
    for (int i = 0; i != len; i++) {
        printf("%02x", (unsigned int)result[i]);
    }
 
    printf("\n");

    return;
}

void GetStringTime(std::string day)
{
	char tmp[32] = {0};
	time_t t = time(0); 
     
    // strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
    strftime(tmp, sizeof(tmp), "%Y%m%d", gmtime(&t)); 
    day = tmp;

    printf("(%s)\n", day.c_str());

    return;
}

int main(int argc, char **argv)
{
    const char key[] = "0123456789abcdef";
    char data[] = "hello world Be careful of the length of string";
    
    std::string day;
    std::string encrypt;
 	std::string DateKey;
 	std::string DateRegionKey;
 	std::string DateRegionServiceKey;
 	std::string SigningKey;

 	GetStringTime(day);

 	HMAC_Sha256_Encrypt(key, day, DateKey);
 	HMAC_Sha256_Encrypt(DateKey, "aws-region", DateRegionKey);
 	HMAC_Sha256_Encrypt(DateRegionKey, "aws-service", DateRegionServiceKey);
 	HMAC_Sha256_Encrypt(DateRegionServiceKey, "aws4_request", SigningKey);
 	
 	HMAC_Sha256_Encrypt(SigningKey, data, encrypt);

    return 0;
}


// g++ -o test hmacsha256.cpp -lssl -lcrypto
// http://docs.aws.amazon.com/AmazonS3/latest/API/sigv4-authentication-HTTPPOST.html