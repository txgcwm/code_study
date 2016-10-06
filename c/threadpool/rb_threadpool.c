#include "rb_threadpool.h"

typedef struct threadpool_task_st {
	thread_pool_callback_func function;
    void *data;
	dlist_elem elem;
} threadpool_task;

typedef struct thread_st {
    pthread_t tid;
    dlist_elem elem;
}thread;



static void *threadpool_func(void *argv) 
{    
    threadpool *pool = (threadpool *)argv;
    
    while(1) {

		pthread_mutex_lock(&pool->lock);

		while(rb_dlist_empty(&pool->task_list)) {
			pthread_cond_wait(&pool->condition, &pool->lock);
			
			if (pool->shutdown) {
				pthread_mutex_unlock(&pool->lock);
				pthread_exit(NULL);
			}
		}

		threadpool_task *initTask = container_of(rb_dlist_pop_front(&pool->task_list), threadpool_task, elem);

		pthread_mutex_unlock(&pool->lock);

		if(initTask != NULL) {
			(*(initTask->function))(initTask->data);
			free(initTask);
			initTask = NULL;
		}
    }
    
    return NULL;
}

int threadpool_create(threadpool ** tpool, int nthreads)
{
	int i;
	int res = -1;
	threadpool *pool = NULL;

	do {
		pool = (threadpool *)malloc(sizeof(threadpool));
		if(pool == NULL)
			break;

		memset(pool, 0, sizeof(threadpool));

		pool->num = nthreads;
		pool->shutdown = 0;

		rb_dlist_init(&pool->thread_list);
		rb_dlist_init(&pool->task_list);
		pthread_mutex_init(&pool->lock, NULL);
    	pthread_cond_init(&pool->condition, NULL);

		for (i = 0; i < nthreads; i++) {
			thread *tempThread = (thread *)malloc(sizeof(thread));
			if(tempThread == NULL)
				continue;
			rb_dlist_push_back(&pool->thread_list, &tempThread->elem);
		}
		
		dlist_elem *e = NULL;
		for (e = rb_dlist_begin(&pool->thread_list); e != rb_dlist_end(&pool->thread_list); e = rb_dlist_next(e)) {
			void *vpool = pool;
			thread *initThread = container_of(e, thread, elem);
			pthread_create(&initThread->tid, NULL, threadpool_func, vpool);
		}

		*tpool = pool;
		res = 0;
	} while(0);

	if(res == -1 && pool != NULL) {
		free(pool);
		pool = NULL;
	}

	return res;
}

void thread_pool_destroy(threadpool **tpool) 
{
	threadpool *pool = *tpool;

    if (pool == NULL)
		return;

    pthread_mutex_lock(&pool->lock);
	pool->shutdown = 1;

	if ((pthread_cond_broadcast(&pool->condition) != 0) || (pthread_mutex_unlock(&pool->lock) != 0))
		return;

	dlist_elem *e = NULL;
    for (e = rb_dlist_begin(&pool->thread_list); e != rb_dlist_end(&pool->thread_list); e = rb_dlist_next(e)) {
		thread *joinThread = container_of(e, thread, elem);
		pthread_join(joinThread->tid, NULL);
    }

    pthread_mutex_unlock(&pool->lock);
    
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->condition);

    free(pool);
	*tpool = NULL;

    return;
}

void thread_pool_add(threadpool *pool, thread_pool_callback_func function, void * data) 
{
    if (pool == NULL || function == NULL)
		return;

    pthread_mutex_lock(&pool->lock);

    if (pool->shutdown) {
		pthread_mutex_unlock(&pool->lock);
		return;
	}
    
    threadpool_task *newTask = (threadpool_task *)malloc(sizeof(threadpool_task));
    newTask->function = function;
    newTask->data = data;
    rb_dlist_push_back(&pool->task_list, &newTask->elem);
    pthread_cond_signal(&pool->condition);
    pthread_mutex_unlock(&pool->lock);

    return;
}



