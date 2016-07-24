
template <class T>
Listiter<T>::Listiter(List<T> &list)
: prev(0)
, the_list(&list)
{
	the_list->iters.insert(this);
}

template <class T>
Listiter<T>::Listiter(const Listiter<T> &iter)
: prev(iter.prev)
, the_list(iter.the_list)
{
	the_list->iters.insert(this);
}

template <class T>
Listiter<T>::~Listiter()
{
	the_list->iters.remove(this);
}

template <class T>
void Listiter<T>::insert_after(const T& ele)
{
	List_link<T> **nextlink = next_link();

	*nextlink = new List_link<T>(ele, *nextlink);

	the_list->n_elem++;

	return;
}

template <class T>
void Listiter<T>::remove_after()
{
	List_link<T> *removed;
	List_link<T> **nextlink = nextlink();

	removed = *nextlink;

	if(!removed)
		printf("remove from end of list\n");

	*nextlink = removed->next;

	Setiter<Listiter<T> *> listiters(the_list->iters);
	Listiter<T> *iter;

	while(listiters.next(iter)) {
		if(iter->prev == removed)
			iter->prev = prev;
	}

	delete removed;

	the_list->n_elem--;

	return;
}

template <class T>
int Listiter<T>::peek_next(T &ele) const
{
	const List_link<T> **nextlink = ((Listiter<T> *)this)->next_link();

	if(*nextlink == 0)
		return 0;
	
	ele = (*nextlink)->data;

	return 1;
}


template <class T>
int Listiter<T>::next(T &ele)
{
	if(peek_next(ele)) {
		prev = *(next_link());
		return 1;
	}
	
	return 0;
}


