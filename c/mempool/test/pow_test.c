#include <string.h>
#include <stdio.h>
#include <stdint.h>



static inline int is_pow_of_2(uint32_t x)
{
	return !(x & (x-1));
}

static inline uint32_t next_pow_of_2(uint32_t x)
{
	if ( is_pow_of_2(x) )
		return x;

	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	x |= x>>16;

	return x+1;
}

int main(int argc, char** argv)
{
	printf("%d\n", next_pow_of_2(64));

	printf("%d\n", next_pow_of_2(65));

	return 0;
}


