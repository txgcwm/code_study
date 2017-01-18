#include <iostream>
#include <map>



typedef std::map <int, char> MYMAP;

int main(int argc, char **argv)
{
	MYMAP charMap1;

	charMap1[1] = 'A';
	charMap1[4] = 'D';
	charMap1[2] = 'B';
	charMap1[5] = 'E';
	charMap1[3] = 'C';

	std::cout << "Contents of first map: " << std::endl;
	MYMAP::iterator iter;
	for (iter = charMap1.begin(); iter != charMap1.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	MYMAP charMap2;

	charMap2[1] = 'F';
	charMap2[4] = 'I';
	charMap2[2] = 'G';
	charMap2[5] = 'J';
	charMap2[3] = 'H';

	std::cout << "Contents of second map: " << std::endl;
	for (iter = charMap2.begin(); iter != charMap2.end(); iter++) {
		std::cout << (*iter).first << " --> ";
		std::cout << (*iter).second << std::endl;
	}
	std::cout << std::endl;

	if(charMap1 == charMap2) {
		std::cout << "map1 == map2" << std::endl;
    } else if (charMap1 < charMap2) {
		std::cout << "map1 < map2" << std::endl;
    } else if (charMap1 > charMap2) {
		std::cout << "map1 > map2" << std::endl;
    }

	return 0;
}


