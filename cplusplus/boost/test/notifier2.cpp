#include <iostream>
#include <vector>
#include <algorithm>

#include "boost/function.hpp"

void print_new_value(int i)
{
	std::cout << "The value has been updated and is now " << i << '\n';
}

void interested_in_the_change(int i) 
{
	std::cout << "Ah, the value has changed.\n";
}

class notifier {
	typedef boost::function<void(int)> function_type;
	std::vector<function_type> vec_;
	int value_;
	
public:
	template <typename T>
	void add_observer(T t) {
		vec_.push_back(function_type(t));
	}

	void change_value(int i) {
		value_= i;
		
		for (std::size_t i=0;i<vec_.size();++i) {
			vec_[i](value_);
		}
	}
};

int main(int argc, char** argv)
{
	notifier n;

	n.add_observer(&print_new_value);
	n.add_observer(&interested_in_the_change);

	n.change_value(42);
	
	return 0;
}

