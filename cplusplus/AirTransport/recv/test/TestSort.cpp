#include <string.h>
#include <stdio.h>
#include <iostream>

#include "Common.h"

using namespace std;



int main(int argc, char **argv)
{
	// uint32_t data[] = {0x01, 0x03, 0x07, 0x23, 0x25, 0x70, 0x80};
	int data[] = {68, 73, 74, 77, 79, 91, 96, 100, 122, 124, 132, 149, 167, 182, 199, 215, 222, 
					231, 248, 263, 281, 295, 314, 327, 347, 359, 380, 391};
  	std::vector<uint32_t> input;
  	std::copy(&data[0], &data[sizeof(data)/sizeof(data[0])], std::back_inserter(input));
  	typedef TupleHelper<std::vector<uint32_t> >::Type Tuple;
  	std::vector<Tuple> result = GetFirstCombination(input.begin(), input.end());

  	std::cout << std::hex;

  	do {
  		std::vector<uint32_t> vec;

  		for(std::vector<Tuple>::iterator it = result.begin();
  			it != result.end(); ++it) {
      		vec.push_back(*boost::get<1>(*it));
  		}

  		for(int i = 0; i < vec.size(); i++) {
  			printf("%3x ", vec[i]);
  		}

    	std::cout << std::endl;
  	} while (NextCombination(result.begin(), result.end()));

  	return 0;
}


