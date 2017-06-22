#include <boost/algorithm/string.hpp>
#include <string.h>
#include <stdio.h>
#include <string>


int main(int argc, char **argv)
{
	std::string wstr = "Abc@QWery&^(/1MS";
	boost::algorithm::to_lower(wstr); // abc 

	printf("%s\n", wstr.c_str());

	return 0;
}


