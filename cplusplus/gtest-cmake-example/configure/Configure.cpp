#include <algorithm>

#include "Configure.h" 



int Configure::addItem(std::string str) 
{ 
	std::vector<std::string>::const_iterator vi = std::find(vItems.begin(), vItems.end(), str); 
	if (vi != vItems.end()) {
	    return vi - vItems.begin(); 
	}

	vItems.push_back(str);

	return vItems.size() - 1; 
} 

std::string Configure::getItem(int index) 
{ 
	if (index >= vItems.size()) {
	    return ""; 
	} else {
	    return vItems.at(index); 
	}
} 

int Configure::getSize() 
{ 
	return vItems.size(); 
}