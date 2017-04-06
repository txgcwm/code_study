#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/ssl.h>
#include <wolfssl/openssl/ssl.h>
#include <wolfssl/openssl/evp.h>

#include <map>

class AesCbcDecrypt
{
public:
    AesCbcDecrypt();
    ~AesCbcDecrypt();
    int Init(int blocksize, const unsigned char *key, const unsigned char *iv);
    int Update(const unsigned char* inbuf, int inlen, unsigned char* outbuf, int* outlen);
    int Final(unsigned char* outbuf, int* outlen);

private:
    unsigned char m_lastcipher[32];
    int m_lastlen;
    Aes m_dec;
};

typedef std::map<EVP_CIPHER_CTX* , AesCbcDecrypt* >  Decrymap_t;
Decrymap_t g_decrymap;


AesCbcDecrypt::AesCbcDecrypt()
: m_lastlen(0)
{
    memset(m_lastcipher,0,sizeof(m_lastcipher));
}

AesCbcDecrypt::~AesCbcDecrypt()
{
}

int AesCbcDecrypt::Init(int blocksize, const unsigned char *key, const unsigned char *iv)
{
    if(blocksize != AES_BLOCK_SIZE)   //just support 128bit decode
    {
        return 0;
    }
    if(0 != wc_AesSetKey(&m_dec, key, blocksize, iv, AES_DECRYPTION))
    {
        return 0;
    }
    return 1;
}

int AesCbcDecrypt::Update(const unsigned char* inbuf, int inlen, unsigned char* outbuf, int* outlen)
{
    if(!inbuf || !outbuf)
    {
        return 0;
    }
    if((inlen+m_lastlen) <= AES_BLOCK_SIZE || inlen <= 0)
    {
        *outlen = 0 ;
        return 1;
    }
    int outbufoffset = 0;
    int inbufoffset  = 0;

    if(m_lastlen > 0 && m_lastlen <= AES_BLOCK_SIZE)
    {
        memcpy(m_lastcipher+m_lastlen, inbuf, AES_BLOCK_SIZE-m_lastlen);
        inbufoffset += AES_BLOCK_SIZE-m_lastlen;
        if(0 != wc_AesCbcDecrypt(&m_dec, outbuf + outbufoffset, m_lastcipher, AES_BLOCK_SIZE))
        {
            return 0;
        }
        outbufoffset += AES_BLOCK_SIZE;
    }
    int cipherlen = inlen-inbufoffset;
    m_lastlen = 0;
    while(cipherlen > AES_BLOCK_SIZE)
    {
        if(0 != wc_AesCbcDecrypt(&m_dec, outbuf + outbufoffset, inbuf+inbufoffset, AES_BLOCK_SIZE))
        {
            return 0;
        }
        inbufoffset += AES_BLOCK_SIZE;
        outbufoffset += AES_BLOCK_SIZE;
        cipherlen -= AES_BLOCK_SIZE;
    }
    if(cipherlen > 0)
    {
        memcpy(m_lastcipher,inbuf+inbufoffset,cipherlen);
        m_lastlen = cipherlen;
    }
    *outlen = outbufoffset;
    return 1;
}

int AesCbcDecrypt::Final(unsigned char* outbuf, int* outlen)
{
    *outlen = 0;

    if(m_lastlen != AES_BLOCK_SIZE)
    {
        return 0;
    }
    if(0 != wc_AesCbcDecrypt(&m_dec, outbuf, m_lastcipher, m_lastlen))
    {
        return 0;
    }

    *outlen = AES_BLOCK_SIZE - outbuf[m_lastlen-1];
    m_lastlen = 0;
    return 1;
}

/*=====================================================================*/
int EVP_DecryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *cipher,
                        void *impl, const unsigned char *key,
                        const unsigned char *iv)
{
    int ret = 0;
    if(cipher != EVP_aes_128_cbc())
    {
        printf("just support aes 128 cbc now!!!\n");
        return 0;
    }
    if(ctx && key && iv)
    {
        AesCbcDecrypt* pAesdecry = new AesCbcDecrypt();
        if(pAesdecry->Init(AES_BLOCK_SIZE,key,iv))
        {
            g_decrymap.insert(std::make_pair(ctx, pAesdecry));
            ret = 1;
        }
        else
        {
            delete pAesdecry;
        }
    }

    return ret;
}

int EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl,
                               const unsigned char *in, int inl)
{
    int ret = 0;
    Decrymap_t::iterator it = g_decrymap.find(ctx);
    if(it != g_decrymap.end())
    {
        AesCbcDecrypt* pAesdecry = it->second;
        if(pAesdecry)
        {
            ret = pAesdecry->Update(in,inl,out,outl);
        }
    }
    return ret;
}

int EVP_DecryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl)
{
    int ret = 0;
    Decrymap_t::iterator it = g_decrymap.find(ctx);
    if(it != g_decrymap.end())
    {
        AesCbcDecrypt* pAesdecry = it->second;
        if(pAesdecry && pAesdecry->Final(outm,outl))
        {
            ret = 1;
        }
        delete pAesdecry;
        g_decrymap.erase(it);
    }

    return ret;
}

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

    return rv;
}

#define ITER_COUNT      3
const unsigned int SALT[] = { 0x28403571, 0x03725024 };

int WsslDecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext, unsigned char *passwd)
{
    EVP_CIPHER_CTX ctx;
    unsigned char key[16], iv[16];
    int len, ret;
    int plaintext_len = 0;

    ret = EVP_BytesToKey_ex(EVP_aes_128_cbc(), EVP_sha1(), (unsigned char*)&SALT, 
                         passwd, strlen((const char *)passwd), ITER_COUNT, key, iv);
    if (16 != ret) {
        printf("failed to generate key and iv!\n");
        return -1;
    }

    EVP_CIPHER_CTX_init(&ctx);

    EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);

    if (EVP_DecryptUpdate(&ctx, plaintext, &len, ciphertext, ciphertext_len) && len > 0) {
        plaintext_len += len;
    }

    if (EVP_DecryptFinal_ex(&ctx, plaintext + len, &len) && len > 0) {
        plaintext_len += len;
    }

    EVP_CIPHER_CTX_cleanup(&ctx);

    return plaintext_len;
}


// ./configure --prefix=/srv/example/cplusplus/register_comm/install/wolfssl --enable-static  --enable-ipv6 --enable-ecc --enable-aesgcm  --enable-hkdf --enable-chacha --enable-poly1305 --enable-alpn --enable-sni --disable-oldtls --enable-supportedcurves --enable-curve25519  --enable-opensslextra