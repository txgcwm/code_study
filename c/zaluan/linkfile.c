#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


void get_real_file_path(char* filename)
{
    struct stat fstat;

    memset(&fstat, 0, sizeof(struct stat));

    if(lstat(filename, &fstat) == 0) {
        printf("get stat success!\n");
    }

    printf("file name: %s, 0x%08x\n", filename, fstat.st_mode);

    if(S_ISLNK(fstat.st_mode)) {
        char buf[50] = {0};

        if(readlink(filename, buf, sizeof(buf)) != -1) {
            printf("%s\n", buf);
        }
    }

    return;
}

int main(int argc, char** argv)
{
    get_real_file_path(argv[1]);

    return 0;
}