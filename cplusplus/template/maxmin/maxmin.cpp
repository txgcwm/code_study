#include <string.h>
#include <stdio.h>



template <class T>
T max(T a, T b)
{
	return (a > b) ? a:b;
}

template <class T>
T min(T a, T b)
{
	return (a > b) ? b:a;
}

int main(int argc, char **argv)
{
	printf("max test int: %d\n", max(3, 5));
	printf("max test float: %f\n", max(7.0, 5.0));

	printf("min test int: %d\n", min(3, 5));
	printf("min test float: %f\n", min(7.0, 5.0));

	return 0;
}