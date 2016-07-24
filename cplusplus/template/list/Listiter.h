#ifndef __LISTITER_H__
#define __LISTITER_H__

#include "List.h"



template <class T>

class Listiter
{
public:
	Listiter(List<T>&);
	Listiter(const Listiter<T> &);
	~Listiter();
	
	void insert_after(const T&);
	void remove_after();

	int next(T &);
	int peek_next(T &) const;

private:
	List_link<T>*   prev;
	List<T>*		the_list;
	List_link<T>*   next_link();
};

#endif