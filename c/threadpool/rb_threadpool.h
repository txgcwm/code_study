#ifndef __RB_THREADPOOL_H__
#define __RB_THREADPOOL_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include "rb_dlist.h"

typedef void * (* thread_pool_callback_func)(void * data);

typedef struct threadpool_st {
	int num;	/*number of threads*/
    int shutdown;

	dlist thread_list;
	dlist task_list;

	pthread_mutex_t lock;
    pthread_cond_t condition;
} threadpool;

int threadpool_create(threadpool ** tpool, int nthreads);

void thread_pool_destroy(threadpool **tpool);

void thread_pool_add(threadpool *pool, thread_pool_callback_func function, void * data);

#ifdef  __cplusplus
}
#endif
#endif
