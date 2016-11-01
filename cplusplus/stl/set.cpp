#include <iostream>
#include <set>



int main(int argc, char **argv)
{
	std::set <char>charSet;

	charSet.insert('E');
	charSet.insert('D');
	charSet.insert('C');
	charSet.insert('B');
	charSet.insert('A');

	std::cout << "Contents of set: " << std::endl;
	std::set <char>::iterator iter;
	for (iter = charSet.begin(); iter != charSet.end(); iter++) {
		std::cout << *iter << std::endl;
    }
	std::cout << std::endl;

	iter = charSet.find('D');
	if (iter == charSet.end()) {
		std::cout << "Element not found." << std::endl;
    } else {
		std::cout << "Element found: " << *iter << std::endl;
    }

	return 0;
}


