#ifndef __LIST_LINK_H__
#define __LIST_LINK_H__

template <class T> class List;
template <class T> class Listiter;

template <class T>

class List_link
{
	friend class List<T>;
	friend class Listiter<T>;

private:
	List_link<T> 	*next;
	T 				data;
	List_link(const T&, List_link<T>*);	
};

#endif