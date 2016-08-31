//
//  List.h
//  IntelliCopter
//
//  Created by Minchan Kim on 2016. 8. 30.
//

#ifndef _LIST_H_
#define _LIST_H_

template <typename T>
struct Node_t
{
        T element;
        Node_t *prev;
        Node_t *next;
};

template <typename T>
class list
{
public:
	list(void);
	void pushFront(T element);
	void pushBack(T element);
	void begin(void);
	void rbegin(void);
	bool getElement(T &element);
	int getSize(void);
	void clear(void);
	~list();
private:
	Node_t<T> *first;
	Node_t<T> *last;
	Node_t<T> *current;
	bool direction;
	int size;
};

template <typename T>
list<T>::list(void)
{
	first = last = current = NULL;
	size = 0;
	direction = true;
}

template <typename T>
void list<T>::pushFront(T element)
{
	Node_t<T> *ptr = new Node_t<T>;
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
	Node_t<T> *ptr = new Node_t<T>;
	ptr->element = element;
	if (last != NULL) last->next = ptr;
	ptr->prev = last;
	ptr->next = NULL;
	last = ptr;
	if (size == 0) first = ptr;
	size++;
}

template <typename T>
void list<T>::begin(void)
{
	current = first;
	direction = true;
}

template <typename T>
void list<T>::rbegin(void)
{
	current = last;
	direction = false;
}

template <typename T>
bool list<T>::getElement(T &element)
{
	if (current == NULL) return false;
	element = current->element;
	if (direction) current = current->next;
	else current = current->prev;
	return true;
}

template <typename T>
int list<T>::getSize(void)
{
	return size;
}

template <typename T>
void list<T>::clear(void)
{
	Node_t<T> *ptr = NULL;
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
	Node_t<T> *ptr = NULL;
	while (first != NULL)
	{
		ptr = first;
		first = ptr->next;
		delete ptr;
	}
}

#endif