#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "buddy.h"
#include "mempool.h"



typedef struct mempool_info {
	void* 	ptr;
	size_t 	size;
	size_t 	block_size;
	size_t 	block_num;
	struct buddy* pbdy;
} mempool_info_s;


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

void* mempool_create(mempool_param_s* pparam)
{
	if(pparam->size <= 0) {
		return NULL;
	}

	mempool_info_s* phandle = (mempool_info_s*)malloc(sizeof(mempool_info_s));
	if(phandle == NULL) {
		return NULL;
	}

	memset(phandle, 0, sizeof(mempool_info_s));

	phandle->ptr = malloc(pparam->size);
	if(phandle->ptr == NULL) {
		free(phandle);
		return NULL;
	}

	memset(phandle->ptr, 0, pparam->size);

	phandle->size = pparam->size;
	phandle->block_size = pparam->block_size;
	if(pparam->block_size > 0) {
		phandle->block_num = next_pow_of_2(pparam->size / pparam->block_size);
	}

	phandle->pbdy = buddy_create(phandle->block_num);

	printf("[%s, %d] block num: %lu\n", __FUNCTION__, __LINE__, phandle->block_num);

	return phandle;
}

void mempool_destroy(void* phandle)
{
	mempool_info_s* pmhdl = (mempool_info_s*)phandle;

	if(pmhdl == NULL) {
		return;
	}

	if(pmhdl->pbdy != NULL) {
		buddy_destroy(pmhdl->pbdy);
	}

	if(pmhdl->ptr != NULL) {
		free(pmhdl->ptr);
	}

	free(pmhdl);

	return;
}

void* mempool_malloc(void* phandle, size_t size)
{
	mempool_info_s* pmhdl = (mempool_info_s*)phandle;

	if(pmhdl == NULL) {
		return NULL;
	}

	int block = size/pmhdl->block_size + (size%pmhdl->block_size > 0 ? 1 : 0);

	int mval = buddy_alloc(pmhdl->pbdy, block);

	printf("[%s, %d] block num: %d, buddy val: %d\n",
			__FUNCTION__, __LINE__, block, mval);

	return (pmhdl->ptr + mval * pmhdl->block_size);
}

void mempool_free(void* phandle, void* ptr)
{
	mempool_info_s* pmhdl = (mempool_info_s*)phandle;

	if(pmhdl == NULL) {
		return;
	}

	int block = (ptr - pmhdl->ptr)/pmhdl->block_size;

	buddy_free(pmhdl->pbdy, block);

	printf("[%s, %d] block num: %d\n", __FUNCTION__, __LINE__, block);

	return;
}

