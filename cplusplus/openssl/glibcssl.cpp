#include <iostream>
#include <rpc/des_crypt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace std;

//DES ECB加密
void des_encrypt(const char *key, char *data, int len)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    des_setparity(pkey);
    ecb_crypt(pkey, data, len, DES_ENCRYPT);
}

//DES ECB解密
void des_decrypt(const char *key, char *data, int len)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    des_setparity(pkey);
    ecb_crypt(pkey, data, len, DES_DECRYPT);
}

//DES CBC加密
void cbc_des_encrypt(const char *key, char *data, int len, const char *ivec)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    char vec[8];
    strncpy(vec, ivec, 8);
    des_setparity(pkey);
    cbc_crypt(pkey, data, len, DES_ENCRYPT, vec);
}

//DES CBC解密
void cbc_des_decrypt(const char *key, char *data, int len, const char *ivec)
{
    char pkey[8];
    strncpy(pkey, key, 8);
    char vec[8];
    strncpy(vec, ivec, 8);
    des_setparity(pkey);
    cbc_crypt(pkey, data, len, DES_DECRYPT, vec);
}

#if 0
int main(int argc, char **argv)
{
    char data[4096] = "cea3e8e1659582206e0be32539729e9ff";
    int len = strlen(data);

    cout<<len<<endl;
    //获取数据需要多少个8字节容纳
    int slice_num = 0;
    if(len % 8 == 0) {
        slice_num = len/8;
    } else {
        slice_num = len/8 + 1;
    }

    cbc_des_encrypt("desmiyao", data, slice_num*8, "cbcinive");

    printf("%s\n", data);

    cbc_des_decrypt("desmiyao", data, slice_num*8, "cbcinive");
    cout<<data<<endl;
    return 0;
}
#else
int main(int argc, char **argv)
{
    int len = strlen(argv[1]);
    int slice_num = 0;
    int left = len % 8;

    if(left == 0) {
        slice_num = len/8 + 1;
    } else {
        slice_num = len/8 + 1;
    }

    int inlen = slice_num * 8;
    char *plain = (char *)malloc(inlen);

    memset(plain, 8 - left, inlen);
    memcpy(plain, argv[1], len);

    des_encrypt("0123456789ABCDEF", plain, inlen);

    int size = strlen(plain);
    for(int i = 0; i < size; i++) {
        printf("%d ", (unsigned char)plain[i]);
    }

    printf("\n");

    des_decrypt("0123456789ABCDEF", plain, inlen);
    cout<<plain<<endl;

    return 0;
}
#endif