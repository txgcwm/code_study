

template <class T>
List_link<T>::List_link(const T& val, List_link<T> *nxt)
: data(val)
, next(nxt)
{
	
}