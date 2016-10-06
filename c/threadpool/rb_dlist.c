#include "rb_dlist.h"


static inline char elem_is_head(dlist_elem *elem)
{
	return elem != NULL && elem->prev == NULL && elem->next != NULL;
}

static inline char elem_is_interior(dlist_elem *elem)
{
	return elem != NULL && elem->prev != NULL && elem->next != NULL;
}

static inline char elem_is_tail(dlist_elem *elem)
{
	return elem != NULL && elem->prev != NULL && elem->next == NULL;
}

static dlist_elem *dlist_front(dlist *list)
{
	if(rb_dlist_empty(list))
		return NULL;

	return list->head.next;
}

static dlist_elem *dlist_back(dlist *list)
{
	if(rb_dlist_empty(list))
		return NULL;

	return list->tail.prev;
}

static void dlist_insert(dlist_elem *before, dlist_elem *elem)
{
	if(before == NULL || elem == NULL)
		return;
  
	if(elem_is_interior(before) || elem_is_tail(before)) {
		elem->prev = before->prev;
		elem->next = before;
		before->prev->next = elem;
		before->prev = elem;
	}

	return;
}

void rb_dlist_init(dlist *list)
{
  	if(list == NULL)
		return;

  	list->head.prev = NULL;
  	list->head.next = &list->tail;
  	list->tail.prev = &list->head;
  	list->tail.next = NULL;

	return;
}

dlist_elem * rb_dlist_begin(dlist *list)
{
	if(list == NULL)
		return NULL;

  	return list->head.next;
}

dlist_elem *rb_dlist_next(dlist_elem *elem)
{
	if(elem_is_head(elem) || elem_is_interior(elem))
  		return elem->next;
	else
		return NULL;
}

dlist_elem * rb_dlist_end(dlist *list)
{
	if(list == NULL)
		return NULL;

  	return &list->tail;
}

char rb_dlist_empty(dlist *list)
{
	return rb_dlist_begin(list) == rb_dlist_end(list);
}

void rb_dlist_push_front(dlist *list, dlist_elem *elem)
{
	dlist_insert(rb_dlist_begin(list), elem);
}

void rb_dlist_push_back(dlist *list, dlist_elem *elem)
{
	dlist_insert(rb_dlist_end(list), elem);
}

static dlist_elem *rb_dlist_remove(dlist_elem *elem)
{
	if(!elem_is_interior(elem))
		return NULL;

	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;

	return elem->next;
}

dlist_elem *rb_dlist_pop_front (dlist *list)
{
	dlist_elem *front = dlist_front(list);

	rb_dlist_remove(front);

	return front;
}

dlist_elem * rb_dlist_pop_back(dlist *list)
{
	dlist_elem *back = dlist_back(list);

	rb_dlist_remove(back);

	return back;
}

size_t rb_dlist_size(dlist *list)
{
	dlist_elem *e = NULL;
	size_t cnt = 0;

	for (e = rb_dlist_begin(list); e != rb_dlist_end(list); e = rb_dlist_next(e))
		cnt++;

	return cnt;
}
