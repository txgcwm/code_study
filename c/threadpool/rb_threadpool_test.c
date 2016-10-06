#include "rb_threadpool.h"

#define THREADPOOL_NUM 5
#define TASK_NUM	30


void *task_function(void *argv)
{
    char buf[1024];
    struct timeval start, end;
    
	gettimeofday(&start, NULL);
	sleep(1);
    gettimeofday(&end, NULL);

    snprintf(buf, sizeof(buf),
            "Task %02d Thread %p start=%ld.%ld end=%ld.%ld",
            *(char *)argv, (void *)pthread_self(),
            start.tv_sec, start.tv_usec,
            end.tv_sec, end.tv_usec);

	printf("%s\n", buf);

    return NULL;
}

int main(int argc, char **argv)
{
	int i = 0;
	char *count = NULL;
	threadpool *pool = NULL;

	count = (char *)malloc(TASK_NUM);

	if(threadpool_create(&pool, THREADPOOL_NUM) < 0)
		return -1;

	for(i = 0; i < TASK_NUM; i++) {
		count[i] = i;
		thread_pool_add(pool, task_function, &count[i]);
	}

	sleep(20);

	if(pool != NULL)
		thread_pool_destroy(&pool);

	return 0;
}



