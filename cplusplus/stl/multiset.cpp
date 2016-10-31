#include <iostream>
#include <set>



int main(int argc, char **argv)
{
	// Create the first set object.  
	std::multiset < char >charMultiset1;

	// Populate the multiset with values.  
	charMultiset1.insert('E');
	charMultiset1.insert('D');
	charMultiset1.insert('C');
	charMultiset1.insert('B');
	charMultiset1.insert('A');
	charMultiset1.insert('B');
	charMultiset1.insert('D');

	// Display the contents of the first multiset.  
	std::cout << "Contents of first multiset: " << std::endl;
	std::multiset < char >::iterator iter;
	for (iter = charMultiset1.begin(); iter != charMultiset1.end(); iter++) {
		std::cout << *iter << std::endl;
    }

	std::cout << std::endl;

	// Create the second multiset object.  
	std::multiset < char >charMultiset2;

	// Populate the multiset with values.  
	charMultiset2.insert('J');
	charMultiset2.insert('I');
	charMultiset2.insert('H');
	charMultiset2.insert('G');
	charMultiset2.insert('F');
	charMultiset2.insert('G');
	charMultiset2.insert('I');

	// Display the contents of the second multiset.  
	std::cout << "Contents of second multiset: " << std::endl;
	for (iter = charMultiset2.begin(); iter != charMultiset2.end(); iter++)
		std::cout << *iter << std::endl;
	std::cout << std::endl;

	// Compare the sets.  
	if (charMultiset1 == charMultiset2) {
		std::cout << "set1 == set2";
    } else if (charMultiset1 < charMultiset2) {
		std::cout << "set1 < set2";
    } else if (charMultiset1 > charMultiset2) {
		std::cout << "set1 > set2";
    }

	return 0;
}



