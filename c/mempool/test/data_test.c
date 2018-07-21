#include "list.h"
#include "mempool.h"



typedef struct data_ {
	void* ptr;
	int   size;
} data_s;


int main(int argc, char** argv)
{
	void* phandle = NULL;
	mempool_param_s param;
	list_t *plist = list_create();

	param.size = 102400;
	param.block_size = 4096;

	phandle = mempool_create(&param);

	for(int i = 0; i < 5; ++i) {
		data_s* pdata = (data_s*)malloc(sizeof(data_s));
		pdata->size = 10;
		pdata->ptr = mempool_malloc(phandle, pdata->size);

		char cmd[20] = {0};
		snprintf(cmd, sizeof(cmd), "afafiernvmxdf-%d", i);
		strcpy(pdata->ptr, cmd);

		list_rpush(plist, pdata);
	}

	for(int j = 0; j < 5; ++j) {
		data_s* value = list_lpop(plist);
	    if(value) {
	        printf("%s\n", value->ptr);
	        list_remove(plist, value);
	    }

		mempool_free(phandle, value->ptr);
	}

	list_destroy(&plist);
	mempool_destroy(phandle);

	return 0;
}