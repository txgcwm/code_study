#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "md5.h"



namespace Rtsp {

static unsigned int md_32(unsigned char *string, int length)
{
	MD5_CTX context;

  	union {
	  	char 		 c[16];
	  	unsigned int x[4];
  	} digest;

  	unsigned int r = 0;
  
	MD5Init(&context);
	MD5Update(&context, string, length);
	MD5Final((unsigned char *)&digest, &context);

	for (int i = 0; i < 3; i++) {
		r ^= digest.x[i];
	}

	return r;
}

unsigned int Random32(int type)
{
  	struct {
	  	int type;
	  	struct timeval tv;
	  	clock_t cpu;
	  	pid_t pid;
	  	int hid;
	  	uid_t uid;
	  	gid_t gid;
	  	struct utsname name;
  	} s;

  	gettimeofday(&s.tv, NULL);
  	uname(&s.name);
  	s.type = type;
  	s.cpu = clock();
  	s.pid = getpid();
  	s.hid = gethostid();
  	s.uid = getuid();
  	s.gid = getgid();

  	return md_32((unsigned char *)&s, sizeof(s));
}

unsigned long GetRanddomSeq()
{
	unsigned long seed;

	srand((unsigned)time(NULL));  
	seed = 1 + (unsigned int)(rand()%(0xFFFF));	
	
	return seed;
}

void AddTimestamp(char *b, int crlf)
{
 	struct tm *t = NULL;
 	time_t now;

 /*
  * concatenates a null terminated string with a
  * time stamp in the format of "Date: 23 Jan 1997 15:35:06 GMT"
  */
 	now = time(NULL);
 	t = gmtime(&now);
 	strftime(b, 38, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", t);
 	if (crlf) {
	 	strcat(b, "\r\n");  /* add a message header terminator (CRLF) */
 	}

 	return;
}

long GetFileSize(FILE *infile)
{
    long size_of_file;
	
	/****  jump to the end of the file. ****/
    fseek(infile, 0L, SEEK_END);

	/****  get the end position. ****/
	size_of_file = ftell(infile);

    /**** Jump back to the original position. ****/
    fseek(infile, 0L, SEEK_SET);

	return (size_of_file);
}

} // end namespace


