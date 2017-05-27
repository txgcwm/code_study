#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/ssl.h>
#include <map>


#include "WolfsslEncryptDecrypt.h"

/*=====================================================================*/
#define PKCS5_SALT_LEN 8

int EVP_BytesToKey_ex(const EVP_CIPHER *type, const EVP_MD *md, 
         const unsigned char *salt, const unsigned char *data, int datal,
         int count, unsigned char *key, unsigned char *iv)
{
    WOLFSSL_EVP_MD_CTX c;
    unsigned char md_buf[64];
    int niv,nkey,addmd=0;
    unsigned int mds=0,i;
    int rv = 0;
    nkey=16;
    niv=16;

    printf("start to generate key/iv, type=%s, md=%s\n",type, md);

    if(type != EVP_aes_128_cbc())
    {
        printf("just support aes 128 cbc now!!!\n");
        return 0;
    }

    if (data == NULL) 
        return 0;

    wolfSSL_EVP_MD_CTX_init(&c);
    for (;;)
    {
        if (wolfSSL_EVP_DigestInit(&c,md) != SSL_SUCCESS)
            goto err;
        if (addmd++)
        {
            if (wolfSSL_EVP_DigestUpdate(&c,&(md_buf[0]),mds) != SSL_SUCCESS)
                goto err;
        }
        
        if (wolfSSL_EVP_DigestUpdate(&c,data,datal) != SSL_SUCCESS)
            goto err;
        if (salt != NULL)
            if (wolfSSL_EVP_DigestUpdate(&c,salt,PKCS5_SALT_LEN) != SSL_SUCCESS)
                goto err;
        if (wolfSSL_EVP_DigestFinal_ex(&c,&(md_buf[0]),&mds) != SSL_SUCCESS)
            goto err;

        for (i=1; i<(unsigned int)count; i++)
            {
            if (wolfSSL_EVP_DigestInit(&c,md) != SSL_SUCCESS)
                goto err;
            if (wolfSSL_EVP_DigestUpdate(&c,&(md_buf[0]),mds) != SSL_SUCCESS)
                goto err;
            if (wolfSSL_EVP_DigestFinal_ex(&c,&(md_buf[0]),&mds) != SSL_SUCCESS)
                goto err;
            }
        i=0;
        if (nkey)
            {
            for (;;)
                {
                if (nkey == 0) break;
                if (i == mds) break;
                if (key != NULL)
                    *(key++)=md_buf[i];
                nkey--;
                i++;
                }
            }
        if (niv && (i != mds))
            {
            for (;;)
                {
                if (niv == 0) break;
                if (i == mds) break;
                if (iv != NULL)
                    *(iv++)=md_buf[i];
                niv--;
                i++;
                }
            }
        if ((nkey == 0) && (niv == 0)) break;
    }
    rv = 16;
    err:
    wolfSSL_EVP_MD_CTX_cleanup(&c);
    printf("return val:%d\n",rv);
    return rv;
}

