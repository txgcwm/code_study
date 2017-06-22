#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>



struct Ying {
	int value;
	char qiao;
};


void setint64(char *section, char *key, int64_t value)
{
	printf("(%s), (%s), (%lld)\n", section, key, value);

	return;
}

void setitem(void *src)
{
	char value[64] = {0};

	snprintf(value, sizeof(value), "%lld", *(int64_t *)src);
	//sprintf(value, "%ld", *(int64_t *)src);

	printf("(%s), (%lld)\n", value, *(int64_t *)src);

	setint64((char *)"", (char *)"ceshi", *(int64_t *)src);

	return;
}

int main(int argc, char **argv)
{
	struct Ying test = {.value = 1, .qiao = 3};
	int32_t val = 0;
	//int64_t val = 0;

	printf("time_t(%d), int64_t(%d), val(%p), test(%p)\n",
			sizeof(time_t), sizeof(int64_t), &val, &test);
	printf("%d, %d\n", test.value, test.qiao);

	setitem((void *)&val);

	return 0;
}


