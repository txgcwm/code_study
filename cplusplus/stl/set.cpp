#include <iostream>
#include <set>

int main(int argc, char **argv)
{
	// Create the set object.  
	std::set < char >charSet;

	// Populate the set with values.  
	charSet.insert('E');
	charSet.insert('D');
	charSet.insert('C');
	charSet.insert('B');
	charSet.insert('A');

	// Display the contents of the set.  
	std::cout << "Contents of set: " << std::endl;
	std::set < char >::iterator iter;
	for (iter = charSet.begin(); iter != charSet.end(); iter++)
		std::cout << *iter << std::endl;
	std::cout << std::endl;

	// Find the D.  
	iter = charSet.find('D');
	if (iter == charSet.end())
		std::cout << "Element not found.";
	else
		std::cout << "Element found: " << *iter;

	return 0;
}
