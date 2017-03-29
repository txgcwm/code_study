#include <stdint.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <boost/tuple/tuple.hpp>

using namespace std;


template <class Container>
struct TupleHelper
{
	typedef typename Container::iterator Iterator;
	typedef boost::tuple<Iterator, Iterator, Iterator> Type;
};

template <class ForwardIt>
inline bool Equal(ForwardIt x, ForwardIt y)
{
	return static_cast<uint64_t>(x)>>4 == static_cast<uint64_t>(y)>>4;
}

template <class ForwardIt>
std::vector<boost::tuple<ForwardIt, ForwardIt, ForwardIt> > GetFirstCombination(ForwardIt first, ForwardIt last)
{
	typedef boost::tuple<ForwardIt, ForwardIt, ForwardIt> Tuple;
  	std::vector<Tuple> it_pair; // begin, curr, end

  	while (first != last) {
    	ForwardIt curr = first;
    	ForwardIt next = curr;
    	++next;

    	while (next!=last && Equal(*curr, *next)) {
      		++curr;
      		++next;
    	}
    
    	it_pair.push_back(Tuple(first, first, curr));
    	first = next;
  	}

  	return it_pair;
}

template <class ForwardIt>
bool NextCombination(ForwardIt first, ForwardIt last)
{
  	for(; first != last; ++first) {
	    if (boost::get<0>(*first) == boost::get<2>(*first)) {
	      	;
	    } else if (boost::get<1>(*first) == boost::get<2>(*first)) {
	      	boost::get<1>(*first) = boost::get<0>(*first);
	    } else {
	      	++boost::get<1>(*first);
	      	break;
	    }
  	}
  
  	return first != last;
}

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
  		for(std::vector<Tuple>::iterator it = result.begin();
  			it != result.end(); ++it) {
  			Tuple const& ele = *it;
      		std::cout << *boost::get<1>(ele) << " ";
  		}

    	std::cout << std::endl;
  	} while (NextCombination(result.begin(), result.end()));

  	return 0;
}


