#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>



int open_and_read_file(char* path)
{
	int fd = -1, num = 0;
	char buf[8000] = {0};

	fd = open(path, O_RDWR);

	while(1) {
		num = read(fd, buf, sizeof(buf));
		if(num <= 0) {
			printf("get data to end, restart to begin!\n");
			lseek(fd, 0, SEEK_SET);
		}

		usleep(100);
	}

	close(fd);
	fd = -1;

	return 0;
}

int main(int argc, char** argv)
{
	open_and_read_file(argv[1]);

	return 0;
}

