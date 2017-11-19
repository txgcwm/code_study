#include <pthread.h>
#include <stdio.h>


static pthread_key_t thread_key;

void* thread_function(void* args)
{
	pthread_t spid = pthread_self();

	pthread_setspecific(thread_key, (void *)spid);

	pthread_t gpid = (pthread_t)pthread_getspecific(thread_key);

	printf("set: %lu, get: %lu, %s\n", spid, gpid, (spid == gpid ? "equal":"not equal"));

	return NULL;
}

int main(int argc, char** argv)
{
	int i;
	pthread_t threads[5];

	pthread_key_create(&thread_key, NULL);

	for (i = 0; i < 5; ++i) {
		pthread_create(&(threads[i]), NULL, thread_function, NULL);
	}

	for (i = 0; i < 5; ++i) {
		pthread_join(threads[i], NULL);
	}

	pthread_key_delete(thread_key);

	return 0;
}