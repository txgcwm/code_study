#ifndef __BLOCK_H__
#define __BLOCK_H__

template <class T>

class Block
{
public:
	Block(int n_elem = 0)
	: elements(new T[n_elem])
	, n_elements(n_elem)
	{

	}

	~Block()
	{
		delete [] elements;
	}

	void size(int);
	int size() const 
	{ 
		return n_elements; 
	}

	T& operator[] (int indx) 
	{
		return elements[indx];
	}

private:
	T 	*elements;
	int n_elements;
};

#endif