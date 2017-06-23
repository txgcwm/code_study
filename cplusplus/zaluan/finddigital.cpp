#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string>


void test0()
{
	char str[]="1776ad";
	int year;

	if (isdigit(str[0])) {
		year = atoi(str);
		printf ("The year that followed %d was %d.\n", year, year + 1);
	}
}

int FindDigit(const char *data, int len, std::string& dig)
{
	int ret = -1;

	for(int i = 0; i < len; i++) {
		char val = data[i];

		if(isdigit(val)) {
			dig += val;
			ret = 0;
		} else {
			break;
		}
	}

	return ret;
}

void test1()
{
	std::string test1 = "weraf";
	std::string test2 = "1dfaf";
	std::string test3 = "23afm";
	std::string test4 = "w2afe";
	std::string test5 = "56a09f";
	std::string test6 = "94567";
	std::string rev = "";

	rev = "";
	FindDigit(test1.c_str(), test1.size(), rev);
	if(rev == "") {
		printf("test1 pass, rev(%s)\n", rev.c_str());
	}

	rev = "";
	FindDigit(test2.c_str(), test2.size(), rev);
	if(rev == "1") {
		printf("test2 pass, rev(%s)\n", rev.c_str());
	}

	rev = "";
	FindDigit(test3.c_str(), test3.size(), rev);
	if(rev == "23") {
		printf("test3 pass, rev(%s)\n", rev.c_str());
	}

	rev = "";
	FindDigit(test4.c_str(), test4.size(), rev);
	if(rev == "") {
		printf("test4 pass, rev(%s)\n", rev.c_str());
	}

	rev = "";
	FindDigit(test5.c_str(), test5.size(), rev);
	if(rev == "56") {
		printf("test5 pass, rev(%s)\n", rev.c_str());
	}

	rev = "";
	FindDigit(test6.c_str(), test6.size(), rev);
	if(rev == "94567") {
		printf("test6 pass, rev(%s)\n", rev.c_str());
	}

	return;
}

int GetCloudEnv(const char *data, int len, std::string& env)
{
	std::string rev = "";

	if(data == NULL || len <= 0) {
		env = "ali";
		return 0;
	}

	if(FindDigit(data, len, rev) == 0) {
		int val = atoi(rev.c_str());
		if(val == 0) {
			env = "ali";
		} else if(val == 1) {
			env = "aws";
		}
	} else {
		if(memcmp(data, "zh_CN", len) == 0) {
			env = "ali";
		} else {
			env = "aws";
		}
	}

	return 0;
}

void test2()
{
	std::string test1 = "zh_CN";
	std::string test2 = "en_US";
	std::string test3 = "0zh_CN";
	std::string test4 = "0en_US";
	std::string test5 = "1en_US";
	std::string test6 = "1zh_CN";
	std::string val;

	val = "";
	GetCloudEnv(test1.c_str(), test1.size(), val);
	if(val == "ali") {
		printf("test1 pass, val(%s)\n", val.c_str());
	}

	val = "";
	GetCloudEnv(test2.c_str(), test2.size(), val);
	if(val == "aws") {
		printf("test2 pass, val(%s)\n", val.c_str());
	}

	val = "";
	GetCloudEnv(test3.c_str(), test3.size(), val);
	if(val == "ali") {
		printf("test3 pass, val(%s)\n", val.c_str());
	}

	val = "";
	GetCloudEnv(test4.c_str(), test4.size(), val);
	if(val == "ali") {
		printf("test4 pass, val(%s)\n", val.c_str());
	}

	val = "";
	GetCloudEnv(test5.c_str(), test5.size(), val);
	if(val == "aws") {
		printf("test5 pass, val(%s)\n", val.c_str());
	}

	val = "";
	GetCloudEnv(test6.c_str(), test6.size(), val);
	if(val == "aws") {
		printf("test6 pass, val(%s)\n", val.c_str());
	}

	GetCloudEnv(NULL, 6, val);
	if(val == "ali") {
		printf("test7 pass, val(%s)\n", val.c_str());
	}

	GetCloudEnv(test6.c_str(), 0, val);
	if(val == "ali") {
		printf("test8 pass, val(%s)\n", val.c_str());
	}

	return;
}

uint64_t getTimeMs()
{
	struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    // printf("tv_sec:%d\n", tv.tv_sec);
    // printf("tv_usec:%d\n", tv.tv_usec);
    // printf("tz_minuteswest:%d\n", tz.tz_minuteswest);
    // printf("tz_dsttime:%d\n", tz.tz_dsttime);

    return tv.tv_sec + tv.tv_usec/1000;
}

int main(int argc, char **argv)
{
	// test2();

	// char *ptr = NULL;

	// printf("len(%d)\n", strlen(ptr));

	printf("%lu\n", getTimeMs());

	return 0;
}

