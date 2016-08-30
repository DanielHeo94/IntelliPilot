#ifndef __LIST_H__
#define __LIST_H__

template <typename T>
struct node_t
{
	T element;
	node_t *prev;
	node_t *next;
};

template <typename T>
class list
{
public:
	list(void);
	void pushFront(T element);
	void pushBack(T element);
	node_t<T>* begin(void);
	int getSize(void);
	void clear(void);
	~list();
private:
	node_t<T> *first;
	node_t<T> *last;
	int size;
};

template <typename T>
list<T>::list(void)
{
	first = last = NULL;
	size = 0;
}

template <typename T>
void list<T>::pushFront(T element)
{
	node_t<T> *ptr = new node_t<T>;
	ptr->element = element;
	if (first != NULL) first->prev = ptr;
	ptr->prev = NULL;
	ptr->next = first;
	first = ptr;
	if (size == 0) last = ptr;
	size++;
}

template <typename T>
void list<T>::pushBack(T element)
{
	node_t<T> *ptr = new node_t<T>;
	ptr->element = element;
	if (last != NULL) last->next = ptr;
	ptr->prev = last;
	ptr->next = NULL;
	last = ptr;
	if (size == 0) first = ptr;
	size++;
}

template <typename T>
node_t<T>* list<T>::begin(void)
{
	return first;
}

template <typename T>
int list<T>::getSize(void)
{
	return size;
}

template <typename T>
void list<T>::clear(void)
{
	node_t<T> *ptr = NULL;
	last = NULL;
	while (first != NULL)
	{
		ptr = first;
		first = ptr->next;
		delete ptr;
	}
	size = 0;
}

template <typename T>
list<T>::~list()
{
	node_t<T> *ptr = NULL;
	while (first != NULL)
	{
		ptr = first;
		first = ptr->next;
		delete ptr;
	}
}

#endif