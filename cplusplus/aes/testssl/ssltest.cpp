#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// #define _USE_WOLFSSL
#ifdef _USE_WOLFSSL
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/openssl/evp.h>
#include <wolfssl/wolfcrypt/aes.h>
#include "WolfsslEncryptDecrypt.h"
#else
#include <openssl/evp.h>
#include <openssl/aes.h>
#endif

#define ITER_COUNT      3
#define BUFF_LEN        (1024 * 16)

const unsigned int SALT[] = { 0x28403571, 0x03725024 };


int check_parameters(int argc, char *argv[])
{
    if (4 > argc) {
        return -1;
    }

    return 0;
}

void print_usage(const char* app)
{
    printf("%s [encrypt|decrypt] [password] [inputfile] [outputfile]\n\n", app);
}

int do_job(const char* job, const char* password, 
           const char* src, const char* dst)
{
    int ret = -1;
    int enc_job = 1;
    int in = -1;
    int out = -1;
    int rd, wt;
    struct stat st = { 0 };
    EVP_CIPHER_CTX ctx;
    unsigned char key[16], iv[16];
    unsigned char in_buff[BUFF_LEN] = { 0 };
    unsigned char out_buff[BUFF_LEN + AES_BLOCK_SIZE] = { 0 };
    int out_len;

    if (0 == strcmp("encrypt", job)) {
        enc_job = 1;
    } else if (0 == strcmp("decrypt", job)) {
        enc_job = 0;
    } else {
        printf("Wrong method: %s.\n", job);
        return -2;
    }

    if (0 != stat(src, &st)) {
        fprintf(stderr, "src %s is invalid!\n", src);
        return -3;
    }

    do {
        if (-1 == (in = open(src, O_RDONLY))) {
            fprintf(stderr, "failed to open %s!\n", src);
            ret = -4;
            break;
        }

        if (-1 == (out = open(dst, O_RDWR | O_CREAT | O_TRUNC, 0644))) {
            fprintf(stderr, "failed to open %s!\n", dst);
            ret = -5;
            break;
        }

#ifdef _USE_WOLFSSL
        ret = EVP_BytesToKey_ex(EVP_aes_128_cbc(), EVP_sha1(), (unsigned char*)&SALT,
                                (unsigned char*)password, strlen(password), ITER_COUNT,
                                key, iv);
#else
        ret = EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha1(), (unsigned char*)&SALT,
                             (unsigned char*)password, strlen(password), ITER_COUNT,
                             key, iv);
#endif
        if (16 != ret) {
            printf("failed to generate key and iv!\n");
            ret = -6;
            break;
        }

        EVP_CIPHER_CTX_init(&ctx);

        if (enc_job) {
            EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);

            out_len = -1;
            size_t want = 0;
            size_t ttrd = 0;
            size_t tten = 0;
            int ret = 0;
            while (1) {
                want = BUFF_LEN - (int)(rand() * 1.0  / RAND_MAX * (BUFF_LEN / 2));
                printf("want: %ld\n", want);
                rd = read(in, in_buff, want);
                if (0 >= rd) {
                    break;
                }
                ttrd += rd;
                printf("read %d, total %ld ... ", rd, ttrd);
                ret = EVP_EncryptUpdate(&ctx, out_buff, &out_len, in_buff, rd);
                printf("encrypt ret %d ... ", ret);
                if (ret && 0 < out_len) {
                    tten += out_len;
                    printf("encrypt %d, total %ld ... ", out_len, tten);
                    wt = write(out, out_buff, out_len);
                    printf("write %d", wt);
                }
                printf("\n");
            };
            ret = EVP_EncryptFinal_ex(&ctx, out_buff, &out_len);
            printf("final encrypt ret %d ... ", ret);
            if (ret && 0 < out_len) {
                tten += out_len;
                printf("final encrypt %d, total %ld ... ", out_len, tten);
                wt = write(out, out_buff, out_len);
                printf("final write %d", wt);
            }
            printf("\n");
        } else {
            EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);

            out_len = -1;
            size_t want = 0;
            size_t ttrd = 0;
            size_t ttde = 0;
            int ret = 0;
            while (1) {
                want = BUFF_LEN - (int)(rand() * 1.0  / RAND_MAX * (BUFF_LEN / 2));
                rd = read(in, in_buff, want);
                if (0 >= rd) {
                    break;
                }
                ttrd += rd;
                printf("read %d, total %ld ... ", rd, ttrd);
                ret = EVP_DecryptUpdate(&ctx, out_buff, &out_len, in_buff, rd);
                printf("decrypt ret %d ... ", ret);
                if (ret && 0 < out_len) {
                    ttde += out_len;
                    printf("decrypt %d, total %ld ... ", out_len, ttde);
                    wt = write(out, out_buff, out_len);
                    printf("write %d", wt);
                }
                printf("\n");
            };
            ret = EVP_DecryptFinal_ex(&ctx, out_buff, &out_len);
            printf("final decrypt ret %d ... ", ret);
            if (ret && 0 < out_len) {
                ttde += out_len;
                printf("final decrypt %d, total %ld ... ", out_len, ttde);
                wt = write(out, out_buff, out_len);
                printf("final write %d", wt);
            }
            printf("\n");
        }

        ret = 0;

    } while (0);

    if (-1 != in) {
        close(in);
        in = -1;
    }

    if (-1 != out) {
        close(out);
        out = -1;
    }

    EVP_CIPHER_CTX_cleanup(&ctx);

    return ret;
}

int main(int argc, char *argv[])
{
    int ret = -1;

    srand((unsigned)time(NULL));

    ret = check_parameters(argc, argv);
    if (0 != ret) {
        print_usage(basename(argv[0]));
        exit(ret);
    }

#ifdef _USE_WOLFSSL
    // wolfSSL_Debugging_ON();
    wolfSSL_Init();
#endif

    ret = do_job(argv[1], argv[2], argv[3], argv[4]);

#ifdef _USE_WOLFSSL
    wolfSSL_Cleanup();
#endif

    return ret;
}


