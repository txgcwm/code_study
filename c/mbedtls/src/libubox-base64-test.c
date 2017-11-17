#include <string.h>
#include <stdio.h>

#include "libubox/utils.h"


int main(int argc, char** argv)
{
    char* src = "test data";
    int src_len = strlen(src);
    char dest[64] = {0};
    int dest_len = 64;

    b64_encode(src, src_len, dest, dest_len);

    printf("encode: %s, len: %d\n", dest, B64_ENCODE_LEN(src_len));

    char decode[64] = {0};
    int declen = 64;

    b64_decode(dest, decode, declen);
    printf("decode: %s, len: %d\n", decode, B64_DECODE_LEN(strlen(dest)));

    return 0;
}