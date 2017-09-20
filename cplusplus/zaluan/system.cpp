#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


uint64_t GetTickCount()
{
    timespec tsnow = {0};
    clock_gettime(CLOCK_MONOTONIC, &tsnow);
	
    return uint64_t(tsnow.tv_sec) * 1000 + tsnow.tv_nsec / 1000000;
}

uint64_t GetCurrentUtcTimeMs()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return ((uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int GetTimeOffset()
{
    return -timezone;
}

int main(int argc, char** argv)
{
	printf("%lld\n", GetTickCount());
	printf("%lld\n", GetCurrentUtcTimeMs());
	printf("%lld\n", GetTimeOffset());
	
	return 0;
}