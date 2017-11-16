#include <string.h>
#include <stdio.h>

#include "libubox/md5.h"


// int md5sum(const char *file, void *md5_buf);

void md5_data()
{
    char* data = "test data";
    unsigned char buf[128] = {0};
    md5_ctx_t ctx;

    md5_begin(&ctx);
    md5_hash(data, strlen(data), &ctx);
    md5_end(buf, &ctx);

    for(int i = 0; i < 16; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }

    printf("\n");

    return;
}

int main(int argc, char** argv)
{
    md5_data();

    return 0;
}