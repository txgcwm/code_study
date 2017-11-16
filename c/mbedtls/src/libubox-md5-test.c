#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "libubox/md5.h"



void md5_data()
{
    char* data = "test data";
    unsigned char buf[16] = {0};
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

void md5_file(const char* file)
{
    unsigned char buf[16] = {0};

    md5sum(file, buf);

    for(int i = 0; i < 16; i++) {
        printf("%02x", (unsigned char)buf[i]);
    }

    printf("\n");

    return;
}

int main(int argc, char** argv)
{
    int res = 0;
    int action = 0;
    char file[256] = {0};

    while((res = getopt(argc, argv, "?a:f:h")) != -1) {
        switch(res) {
        case 'a':
            action = atoi(optarg);
            break;

        case 'f':
            memcpy(file, optarg, strlen(optarg));
            break;

        case 'h':
        default:
            break;
        }
    }

    if(action == 0) {
        md5_data();
    } else if (action == 1) {
        md5_file(file);
    }

    return 0;
}