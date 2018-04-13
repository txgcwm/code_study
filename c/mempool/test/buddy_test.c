#include <stdio.h>

#include "buddy.h"


int main(int argc, char** argv)
{
    struct buddy* buddy = buddy_create(32);

    buddy_dump(buddy);

    int mval = buddy_alloc(buddy, 4);
    buddy_dump(buddy);
    printf("malloc value: %d\n", mval);

    mval = buddy_alloc(buddy, 9);
    buddy_dump(buddy);
    printf("malloc value: %d\n", mval);

    mval = buddy_alloc(buddy, 3);
    buddy_dump(buddy);
    printf("malloc value: %d\n", mval);

    mval = buddy_alloc(buddy, 7);
    buddy_dump(buddy);
    printf("malloc value: %d\n", mval);

    buddy_destroy(buddy);

    return 0;
}


