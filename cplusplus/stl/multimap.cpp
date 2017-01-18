#include <iostream>
#include <map>



typedef std::multimap <int, char> MYMAP;

int main(int argc, char **argv)
{
	MYMAP charMultimap;

	charMultimap.insert(MYMAP::value_type(1, 'A'));
	charMultimap.insert(MYMAP::value_type(4, 'C'));
	charMultimap.insert(MYMAP::value_type(2, 'B'));
	charMultimap.insert(MYMAP::value_type(7, 'E'));
	charMultimap.insert(MYMAP::value_type(5, 'D'));
	charMultimap.insert(MYMAP::value_type(3, 'B'));
	charMultimap.insert(MYMAP::value_type(6, 'D'));

	std::cout << "Contents of first multimap: " << std::endl;
	MYMAP::iterator iter;
	for (iter = charMultimap.begin(); iter != charMultimap.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	MYMAP charMultimap2;

	charMultimap2.insert(MYMAP::value_type(1, 'C'));
	charMultimap2.insert(MYMAP::value_type(4, 'F'));
	charMultimap2.insert(MYMAP::value_type(2, 'D'));
	charMultimap2.insert(MYMAP::value_type(7, 'E'));
	charMultimap2.insert(MYMAP::value_type(5, 'F'));
	charMultimap2.insert(MYMAP::value_type(3, 'E'));
	charMultimap2.insert(MYMAP::value_type(6, 'G'));

	std::cout << "Contents of second multimap: " << std::endl;
	for (iter = charMultimap2.begin(); iter != charMultimap2.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	if (charMultimap == charMultimap2) {
		std::cout << "multimap1 == multimap2" << std::endl;
    } else if (charMultimap < charMultimap2) {
		std::cout << "multimap1 < multimap2" << std::endl;
    } else if (charMultimap > charMultimap2) {
		std::cout << "multimap1 > multimap2" << std::endl;
    }

	return 0;
}


