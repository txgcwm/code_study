#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>




int main(int argc,char **argv)
{
	int fd = -1;
	int count = 0;
	char buf[30];

	fd = open("/proc/testproc", O_RDONLY);
	if(fd < 0) {
		printf("error, can't open device\n");
		return -1;
	}

	printf("fd:%d\n", fd);

	while(1) {
		memset(buf, '\0', sizeof(buf));

		count = read(fd, buf, sizeof(buf));
		if(count < 0) {
			printf("can't read data from device, count: %d\n", count);
		} else {
			printf("num: %d, read data: %s\n", count, buf);
		}

		sleep(1);
	}

	close(fd);

	return 0;
}



