#include <string.h>
#include <stdio.h>
#include <iostream>
#include "boost/bind.hpp"



void bind_func(int x, int y)
{
	printf("%d, %d\n", x, y);
	
	return;
}

int main(int argc, char** argv)
{
	boost::bind(&bind_func, 3, 4)();
	
	boost::bind(&bind_func, _1, 3)(4);
	
	return 0;
}