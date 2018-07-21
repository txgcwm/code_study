#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

// Memory management macros

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

//! list_t iterator direction.
//!
typedef enum {
    LIST_HEAD,
	LIST_TAIL
} list_direction_t;


//! LIST_NODE ADT
typedef struct list_node {
    struct list_node *prev;
    struct list_node *next;
    void *val;
} list_node_t;

//! LIST ADT
typedef struct list {
    list_node_t *head;
    list_node_t *tail;
    unsigned int len;
    void (*free)(void *val);
    int (*match)(void *a, void *b);
} list_t;


//! LIST_ITERATOR ADT
typedef struct list_iterator {
    list_node_t *next;
    list_direction_t direction;
} list_iterator_t;


// list_t prototypes.

list_t *list_create(void);

void list_destroy(list_t **self);

int list_rpush(list_t *self, void *value);

int list_lpush(list_t *self, void *value);

void* list_value_exist(list_t *self, void *value);

void* list_at(list_t *self, int index);

void* list_rpop(list_t *self);

void* list_lpop(list_t *self);

int list_remove(list_t *self, void *value);

// list_t iterator prototypes.

list_iterator_t* list_iterator_create (list_t *list, list_direction_t direction);

void* list_iterator_next (list_iterator_t *self);

void list_iterator_destroy (list_iterator_t **self);

#ifdef __cplusplus
}
#endif

#endif // LIST_H

