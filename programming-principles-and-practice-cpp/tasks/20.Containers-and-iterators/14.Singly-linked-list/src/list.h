#ifndef LIST_H
#define LIST_H

template<class T>
struct Link
{
	Link* next;
	T val;

	Link(const T& val) 
		: next(NULL), val(val) {};
};

template<class T>
struct list
{
private:
	Link<T> * first;

public:
	class iterator;

	list()
		: first(NULL) {};

	iterator begin() { return iterator(first); }
	iterator end() { return iterator(NULL); }

	iterator erase(iterator p);

	void push_front(const T& val);
	void pop_front();

	T& front();
};

template<class T>
class list<T>::iterator
{
	friend struct list<T>;
	Link<T> * curr;
public:
	iterator(Link<T>* p)
		: curr(p) {};

	iterator& operator++() { curr = curr->next; return *this; };
	T& operator*() { return curr->val; }

	bool operator==(const iterator& other) const 
	{ 
		return curr == other.curr; 
	}
	
	bool operator!=(const iterator& other) const
	{
		return !(*this==other);
	}
};

#include "list.tpp"
#endif // LIST_H
