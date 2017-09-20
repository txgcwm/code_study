#include <string.h>
#include <stdio.h>
#include <iostream>

#include "boost/function.hpp"

bool some_func(int i, double d)
{
	printf("%d  %f\n", i, d);
	
	return i > d;
}

int main(int argc, char** argv)
{
	boost::function<bool (int, double)> f;
	f = &some_func;
	bool val = f(10, 1.1);
	
	return 0;
}