
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"



int main(int argc, char *argv[])
{
    list_t *plist = list_create();

    list_rpush(plist, "test");

    char* value = list_rpop(plist);
    if(value) {
        printf("%s\n", value);
        list_remove(plist, value);
    }

    value = list_rpop(plist);
    if(value == NULL) {
        printf("no value!\n");
    }

    list_destroy(&plist);

    return 0;
}
