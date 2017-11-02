#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "process.h"

const char *inputFiles[4] = {"imgs/a.bmp", "imgs/b.bmp", "imgs/c.bmp", "imgs/d.bmp"};
const char *outputFiles[4] = {"imgs/a_out.bmp", "imgs/b_out.bmp", "imgs/c_out.bmp", "imgs/d_out.bmp"};

int main(void)
{
    ClImage *srcImages[4];
    int i;

    for (i = 0; i < 1; ++i) {
		srcImages[i] = clLoadImage(inputFiles[i]);
		processImage(srcImages[i], outputFiles[i], i);
    }

    return 0;
}

// gcc main.c sources/*.c -I sources/ -o test -lm
