#ifndef __RB_DLIST_H__
#define __RB_DLIST_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



#define container_of(ptr, type, member) ({	\
    const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
    (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct dlist_elem_st dlist_elem;

/* Double list element. */
typedef struct dlist_elem_st {
	dlist_elem *prev; /* Previous list element. */
    dlist_elem *next; /* Next list element. */
}dlist_elem;

/* Double list. */
typedef struct dlist_st {
	dlist_elem head; /* List head. */
	dlist_elem tail; /* List tail. */
}dlist;

void rb_dlist_init(dlist *list);

dlist_elem *rb_dlist_begin(dlist *list);

dlist_elem *rb_dlist_next(dlist_elem *elem);

dlist_elem *rb_dlist_end(dlist *list);

void rb_dlist_push_front(dlist *list, dlist_elem *elem);

void rb_dlist_push_back(dlist *list, dlist_elem *elem);

dlist_elem *rb_dlist_pop_front (dlist *list);

dlist_elem *rb_dlist_pop_back (dlist *list);

char rb_dlist_empty(dlist *list);

size_t rb_dlist_size(dlist *list);

#ifdef  __cplusplus
}
#endif
#endif
