#include <iostream>
#include <map>



typedef std::multimap <int, char>MYMAP;

int main(int argc, char **argv)
{
	// Create the first multimap object.  
	MYMAP charMultimap;

	// Populate the multimap with values.  
	charMultimap.insert(MYMAP::value_type(1, 'A'));
	charMultimap.insert(MYMAP::value_type(4, 'C'));
	charMultimap.insert(MYMAP::value_type(2, 'B'));
	charMultimap.insert(MYMAP::value_type(7, 'E'));
	charMultimap.insert(MYMAP::value_type(5, 'D'));
	charMultimap.insert(MYMAP::value_type(3, 'B'));
	charMultimap.insert(MYMAP::value_type(6, 'D'));

	// Display the contents of the first multimap.  
	std::cout << "Contents of first multimap: " << std::endl;
	MYMAP::iterator iter;
	for (iter = charMultimap.begin(); iter != charMultimap.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	// Create the second multimap object.  
	MYMAP charMultimap2;

	// Populate the second multimap with values.  
	charMultimap2.insert(MYMAP::value_type(1, 'C'));
	charMultimap2.insert(MYMAP::value_type(4, 'F'));
	charMultimap2.insert(MYMAP::value_type(2, 'D'));
	charMultimap2.insert(MYMAP::value_type(7, 'E'));
	charMultimap2.insert(MYMAP::value_type(5, 'F'));
	charMultimap2.insert(MYMAP::value_type(3, 'E'));
	charMultimap2.insert(MYMAP::value_type(6, 'G'));

	// Display the contents of the second multimap.  
	std::cout << "Contents of second multimap: " << std::endl;
	for (iter = charMultimap2.begin(); iter != charMultimap2.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	// Compare the multimaps.  
	if (charMultimap == charMultimap2) {
		std::cout << "multimap1 == multimap2";
    } else if (charMultimap < charMultimap2) {
		std::cout << "multimap1 < multimap2";
    } else if (charMultimap > charMultimap2) {
		std::cout << "multimap1 > multimap2";
    }

	return 0;
}


