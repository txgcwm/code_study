#include <stdio.h>

#include "bounce.h"



int main(int argc, char **argv)
{
#ifdef BOUNCE1
	do_bounce();
#endif

#ifdef BOUNCE2
	ball();
#endif

	return 0;
}
