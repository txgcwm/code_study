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
	char *data = "it just for test scull0 "; 	

	fd = open("/dev/scull0",O_RDWR);
	if(fd < 0) {
		printf("error %d:can't open device scull0\n",fd);
		return -1;
	}

	printf("fd:%d\n", fd);

	while(1) {
		memset(buf, '\0', sizeof(buf));
		count = write(fd, data, strlen(data));
		if(count <= 0) {
			printf("error%d,can't write data to device scull0\n", count);
			continue;
		} else {
			count = read(fd, buf, strlen(data));
			if(count < 0) {
				printf("can't read data from device scull0,count:%d\n", count);
			} else {
				printf("num:%d,read data:%s \n",count,buf);
			}
		}

		sleep(1);
	}

	return 0;
}



