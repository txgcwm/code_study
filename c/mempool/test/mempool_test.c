

#include "mempool.h"



int main(int argc, char** argv)
{
	void* phandle = NULL;
	mempool_param_s param;

	param.size = 102400;
	param.block_size = 4096;

	phandle = mempool_create(&param);

	void* ptr1 = mempool_malloc(phandle, 4567);

	void* ptr2 = mempool_malloc(phandle, 14567);

	mempool_free(phandle, ptr1);

	mempool_free(phandle, ptr2);

	mempool_destroy(phandle);

	return 0;
}

