#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <boost/tuple/tuple.hpp>
#include <algorithm>
#include <iterator>
#include <vector>



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

char crc8(const char *data, int len);

unsigned short crc_ccitt(unsigned char *q, int len);

#endif