#include "Block.h"



template <class T>

void Block<T>::size(int new_sz)
{
	if(new_sz != size()) {
		T *new_ele = new T[new_sz];

		if(new_ele == NULL) {
			printf("out of memory!\n")
		}

		int n_copy = (new_sz < size()) ? new_sz : size();

		for(int i = 0; i < n_copy; ++i)
			new_ele[i] = elements[i];

		delete [] elements;
		elements = new_ele;
		n_elements = new_sz;
	}

	return;
}