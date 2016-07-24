#include <string.h>
#include <stdio.h>



template <class T>
void swap(T &a, T &b)
{
	T tmp = a;

	a = b;
	b = tmp;

	return;
}

int main(int argc, char **argv)
{
	int a = 3;
	int b = 5;

	swap(a, b);

	printf("a: %d, b: %d\n", a, b);


	float c = 7.0;
	float d = 2.0;

	swap(c, d);

	printf("c: %f, d: %f\n", c, d);

	return 0;
}