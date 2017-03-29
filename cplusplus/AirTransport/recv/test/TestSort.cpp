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


#if 0
2017-03-29 18:06:08.992636 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(77)
2017-03-29 18:06:08.995511 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(78)
2017-03-29 18:06:08.996947 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(153)
2017-03-29 18:06:08.998685 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(171)
2017-03-29 18:06:09.000096 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(186)
2017-03-29 18:06:09.002146 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(203)
2017-03-29 18:06:09.004185 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(219)
2017-03-29 18:06:09.005725 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(235)
2017-03-29 18:06:09.007159 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(252)
2017-03-29 18:06:09.009042 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(267)
2017-03-29 18:06:09.010521 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(285)
2017-03-29 18:06:09.012733 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(299)
2017-03-29 18:06:09.014187 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(318)
2017-03-29 18:06:09.015590 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(331)
2017-03-29 18:06:09.017004 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(351)
2017-03-29 18:06:09.018417 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(363)
2017-03-29 18:06:09.019832 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(384)
2017-03-29 18:06:09.021331 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(395)
2017-03-29 18:06:09.024977 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(81)
2017-03-29 18:06:09.026455 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(295)
2017-03-29 18:06:09.028115 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(314)
2017-03-29 18:06:09.029823 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(327)
2017-03-29 18:06:09.033759 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(347)
2017-03-29 18:06:09.036093 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(167)
2017-03-29 18:06:09.038578 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(182)
2017-03-29 18:06:09.040447 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(199)
2017-03-29 18:06:09.042268 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(215)
2017-03-29 18:06:09.043782 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(231)
2017-03-29 18:06:09.045235 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(248)
2017-03-29 18:06:09.046689 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(263)
2017-03-29 18:06:09.048581 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(281)
2017-03-29 18:06:09.050522 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(359)
2017-03-29 18:06:09.052126 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(380)
2017-03-29 18:06:09.054150 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(391)
2017-03-29 18:06:09.055653 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(83)
2017-03-29 18:06:09.058558 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(95)
2017-03-29 18:06:09.360428 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(104)
2017-03-29 18:06:09.657711 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(128)
2017-03-29 18:06:09.975168 [28683] [AirData.cpp(140) StoreRawData]Trace: insert len(136)


#endif