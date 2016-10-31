#include <iostream>
#include <list>

int main(int argc, char **argv)
{
	// Create and populate the list.  
	int x;
	std::list < char >charList;
	for (x = 0; x < 10; ++x) {
		charList.push_front(65 + x);
	}

	// Display contents of list.  
	std::cout << "Original list: ";
	std::list < char >::iterator iter;
	for (iter = charList.begin(); iter != charList.end(); iter++) {
		std::cout << *iter;
		//char ch = *iter;  
		//std::cout << ch;  
	}
	std::cout << std::endl;

	// Insert five Xs into the list.  
	std::list < char >::iterator start = charList.begin();
	charList.insert(++start, 5, 'X');

	// Display the result.  
	std::cout << "Result list: ";
	for (iter = charList.begin(); iter != charList.end(); iter++) {
		std::cout << *iter;
		//char ch = *iter;  
		//std::cout << ch;  
	}

	std::cout << std::endl;

	return 0;
}
