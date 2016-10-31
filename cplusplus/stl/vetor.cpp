#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
	int x;
	std::vector < char >charVector;

	for (x = 0; x < 10; ++x) {
		charVector.push_back(65 + x);
	}

	int size = charVector.size();

	for (x = 0; x < size; ++x) {

		std::vector < char >::iterator start = charVector.begin();
		std::vector < char >::iterator iter;

		for (iter = start; iter != charVector.end(); iter++) {
			std::cout << *iter;
		}

		charVector.erase(start);

		std::cout << std::endl;
	}

	return 0;
}
