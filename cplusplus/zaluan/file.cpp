#include <string.h>
#include <stdio.h>
#include <string>


int writeFile(std::string filename, const char* text, int size)
{
    int ret = -1;
    int len = -1;
    FILE* file = NULL;

    file = fopen(filename.c_str(), "wb");
    if(file == NULL) {
        printf("open file(%s) fail!\n", filename.c_str());
        return ret;
    }

    len = fwrite(text, 1, size, file);
    if(len == size) {
        ret = 0;
    } else {
        printf("len(%d), write data to file fail!\n", len);
    }

    fclose(file);
    file = NULL;

    return ret;
}

int readFile(std::string filename)
{
    int ret = -1;
    int len = -1;
    FILE* file = NULL;
    char buffer[1024] = {0};

    file = fopen(filename.c_str(), "rb");
    if(file == NULL) {
        printf("open file(%s) fail!\n", filename.c_str());
        return ret;
    }

    len = fread(buffer, 1, sizeof(buffer), file);
    if(len <= 0) {
        printf("len(%d), can not read data from file!\n", len);
    } else {
        ret = 0;
    }

    printf("Read %d Byte  (%s)\n", len, buffer);

    fclose(file);
    file = NULL;

    return ret;
}

int main(int argc, char** argv)
{
    if(writeFile("test.txt", "nihaoa", strlen("nihaoa")) < 0) {
        printf("write file error!\n");
    } else {
        printf("write file success!\n");
    }

    if(readFile("test.txt") < 0) {
        printf("read file error!\n");
    } else {
        printf("read file success!\n");
    }

    return 0;
}


