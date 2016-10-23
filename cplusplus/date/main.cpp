#include <iostream>
#include <cstdlib>

#include "date.h"



using namespace std;
using namespace Date;

int main(int argc, char **argv)
{
	int a, b, c, d = 160000;
	int e, f, g;

	date da(1900, 2, 29);		//平年二月29天？　构造函数内有process()函数来处理
	if (da.isError()) {
		cout << "\n不正确的日期\n";
	}

	cout << da.datetostr() << endl;

	srand((unsigned)time(0));

	while (d--) {
		a = rand() % 2014;
		b = rand() % 33;
		c = rand() % 32;

		e = rand() % 2014;
		f = rand() % 33;
		g = rand() % 32;

		date dd(a, b, c);
		date ddd(e, f, g);

		if (dd.isError()) {
			cout << dd.datetostr() << endl;	//为什么输出的全是1900-2-29?构造函数内的process()函数你到哪里去了？
		}

		if (ddd.isError()) {
			cout << ddd.datetostr() << endl;
		}
	}

	return 0;
}



