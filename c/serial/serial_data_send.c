#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


#define UART_DEVICE     "/dev/ttyUSB0"

int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
          		   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, 
		  		  115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, };

void set_speed(int fd, int speed)
{
	int   i; 
  	int   status; 
  	struct termios Opt;
  
  	tcgetattr(fd, &Opt);

  	for(i= 0; i < sizeof(speed_arr) / sizeof(int); i++) { 
    	if  (speed == name_arr[i]) {     
      		tcflush(fd, TCIOFLUSH);     
      		cfsetispeed(&Opt, speed_arr[i]);  
      		cfsetospeed(&Opt, speed_arr[i]);   
      		status = tcsetattr(fd, TCSANOW, &Opt);  
      		if  (status != 0) {        
        		perror("tcsetattr fd1");  
        		return;     
      		}

      		tcflush(fd, TCIOFLUSH);   
    	}  
  	}

  	return;
}

int set_parity(int fd, int databits, int stopbits, int parity)
{ 
	struct termios options;

	if(tcgetattr(fd, &options) != 0) { 
		perror("SetupSerial 1");     
		return -1;  
	}

	options.c_cflag &= ~CSIZE;

	switch (databits) { /*设置数据位数*/   
	case 7:		
		options.c_cflag |= CS7; 
		break;

	case 8:     
		options.c_cflag |= CS8;
		break;

	default:    
		fprintf(stderr, "Unsupported data size\n");
		return -1;  
	}

	switch (parity) {   
	case 'n':
	case 'N':    
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
		break;

	case 'o':   
	case 'O':     
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
		options.c_iflag |= INPCK;             /* Disnable parity checking */ 
		break;

	case 'e':  
	case 'E':   
		options.c_cflag |= PARENB;     /* Enable parity */    
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;

	case 'S': 
	case 's':  /*as no parity*/   
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;

	default:   
		fprintf(stderr, "Unsupported parity\n");    
		return -1;
	}

	/* 设置停止位*/  
	switch(stopbits) {   
	case 1:    
		options.c_cflag &= ~CSTOPB;  
		break;

	case 2:    
		options.c_cflag |= CSTOPB;  
	   	break;

	default:    
		fprintf(stderr, "Unsupported stop bits\n");  
		return -1; 
	}

	/* Set input parity option */ 
	if (parity != 'n') {
		options.c_iflag |= INPCK;
	} 

	tcflush(fd,TCIFLUSH);

	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/   
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */

	if(tcsetattr(fd, TCSANOW, &options) != 0) { 
		perror("SetupSerial 3");   
		return -1;  
	}

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag &= ~OPOST;   /*Output*/

	return 0;
}

int main(int argc, char *argv[])
{
    int fd = -1;
    char buf[256] = {0};

    fd = open(UART_DEVICE, O_RDWR);
    if(fd < 0) {
        perror(UART_DEVICE);
        exit(1);
    }

    set_speed(fd, 9600);

	if(set_parity(fd, 8, 1, 'N') < 0) {
		printf("Set Parity Error\n");
		exit (0);
	}

    strcpy(buf, "hello world!");

    write(fd, buf, strlen(buf));

    close(fd);

    return 0;
}

