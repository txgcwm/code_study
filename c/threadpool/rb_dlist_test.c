#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "rb_dlist.h"

#define DLIST_MAX_LEN	5

typedef struct dlist_test_st {
	int count;
	dlist_elem elem;
}dlist_test;

typedef enum push_method_en {
	PUSH_FRONT,
	PUSH_BACK,
}push_method;

typedef enum pop_method_en {
	POP_FRONT,
	POP_BACK,
}pop_method;

void dlist_push_pop_test(push_method push, pop_method pop)
{
	int i = 0;
	dlist list;
	
	printf("push: %d, pop: %d\n", push, pop);

	memset(&list, 0, sizeof(dlist));

	rb_dlist_init(&list);

	for(i = 0; i < DLIST_MAX_LEN; i++) {
		dlist_test * ltest = NULL;
		ltest = (dlist_test *)malloc(sizeof(dlist_test));
		if(ltest == NULL)
			continue;

		memset(ltest, 0, sizeof(dlist_test));
		ltest->count = i;

		if(push == PUSH_FRONT)
			rb_dlist_push_front(&list, &ltest->elem);
		else
			rb_dlist_push_back(&list, &ltest->elem);
	}

	printf("double list size: %d\n", rb_dlist_size(&list));

	for(i = 0; i < DLIST_MAX_LEN; i++) {
		dlist_elem * elem = NULL;
		
		if(pop == POP_FRONT)
			elem = rb_dlist_pop_front(&list);
		else
			elem = rb_dlist_pop_back(&list);

		if(elem == NULL) 
			continue;

		dlist_test * ltest = container_of(elem, dlist_test, elem);
		printf("pop count: %d\n", ltest->count);
		free(ltest);
		ltest = NULL;
	}

	printf("list if empty: %s\n", rb_dlist_empty(&list) ? "empty":"no empty");

	return;
}

int main(int argc, char **argv)
{
	dlist_push_pop_test(PUSH_FRONT, POP_FRONT);
	dlist_push_pop_test(PUSH_FRONT, POP_BACK);

	dlist_push_pop_test(PUSH_BACK, POP_FRONT);
	dlist_push_pop_test(PUSH_BACK, POP_BACK);

	return 0;
}
