#include <iostream>
#include <list>



int main(int argc, char **argv)
{
	int x;
	std::list <char>charList;

	for(x = 0; x < 10; ++x) {
		charList.push_front(65 + x);
	}

	std::cout << "Original list: ";

	std::list <char>::iterator iter;

	for(iter = charList.begin(); iter != charList.end(); iter++) {
		std::cout << *iter;
	}

	std::cout << std::endl;

	// Insert five Xs into the list.  
	std::list <char>::iterator start = charList.begin();
	charList.insert(start, 5, 'X');
 
	std::cout << "Result list: ";
	for(iter = charList.begin(); iter != charList.end(); iter++) {
		std::cout << *iter;
	}

	std::cout << std::endl;

	return 0;
}


