#ifndef __LIST_H__
#define __LIST_H__

template <class T>

class List
{
	friend class List_link<T>;
	friend class Listiter<T>;

public:
	List();
	~List();

	int length() const { return n_elem; };
	
private:
	List_link<T>*  head;
	Set<Listiter<T>*> iters;
	int n_elem;
};

#endif