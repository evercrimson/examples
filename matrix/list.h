#ifndef LIST_H
#define LIST_H

#include "rational_number.h"
#include "exception.h"

struct coords
{
	size_t x;
	size_t y;
	coords(size_t x1, size_t x2);
	coords();
};

struct node
{
	coords pos;
	rational_number a;
	node *next;
};

class List
{
	node *head;
	public:
	List();
	~List();
	void push(const coords &pos, const rational_number &a);
	void del(const coords &pos);
	rational_number find(const coords &idx) const;
	rational_number& find1(const coords &idx);
	List operator =(const List &l);
	List operator -() const;
	node* get_head() const;
	void remove();
};

#endif
