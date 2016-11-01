#include <iostream>
#include <deque>



int main(int argc, char **argv)
{
	int x;
	std::deque <char>charDeque;

	for (x = 0; x < 10; ++x) {
		charDeque.push_front(65 + x);
	}

	int size = charDeque.size();

	for(x = 0; x < size; ++x) {
		std::deque <char>::iterator start = charDeque.begin();
		std::deque <char>::iterator iter;

		for(iter = start; iter != charDeque.end(); iter++) {
			std::cout << *iter;
		}

		charDeque.erase(start);

		std::cout << std::endl;
	}

	return 0;
}


