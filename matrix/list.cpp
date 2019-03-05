#include "list.h"

coords::coords(size_t x1, size_t x2)
{
	x = x1;
	y = x2;
}

coords::coords()
{
	x = 0;
	y = 0;
}

List::List()
{
	head = NULL;
}

void List::remove()
{
	while(head != NULL)
	{
		node *tmp = head->next;
		delete head;
		head = tmp;
	}
}

List::~List()
{
	while(head != NULL)
	{
		node *tmp = head->next;
		delete head;
		head = tmp;
	}
}

void List::push(const coords &pos, const rational_number &a)
{
	node *tmp = new node;
	tmp->pos.x = pos.x;
	tmp->pos.y = pos.y;
	tmp->a = a;
	tmp->next = head;
	head = tmp;
}

void List::del(const coords &pos)
{
	node *tmp = head;
	node *ficttmp = NULL;

	if (head == NULL)
	{
		Exception t(ERR_NOLIST);
		throw (t);
	}

	if ((head->pos.x == pos.x) && (head->pos.y == pos.y))
	{
		tmp = head;
		head = head->next;
		delete tmp;
		return;
	}
	
	tmp = head->next;
	ficttmp = head;
	while (tmp!=NULL)
	{
		if ((tmp->pos.x == pos.x) && (tmp->pos.y == pos.y))
		{
			ficttmp->next = tmp->next;
			delete tmp;
			return;
		}
		ficttmp = tmp;
		tmp = tmp->next;
	}
}

rational_number List::find(const coords &idx) const
{
	node *tmp = head;
	while (tmp!=NULL)
	{
		if ((tmp->pos.x == idx.x) && (tmp->pos.y == idx.y))
			return tmp->a;
		tmp = tmp->next;
	}
	return 0;
}

rational_number& List::find1(const coords &idx)
{
	node *tmp = head;
	while (tmp!=NULL)
	{
		if ((tmp->pos.x == idx.x) && (tmp->pos.y == idx.y))
			return tmp->a;
		tmp = tmp->next;
	}
	push(idx, 0);
	return head->a;
}

List List::operator -() const
{
	node *tmp = head;
	while (tmp != NULL)
	{
		tmp->a = -tmp->a;
		tmp = tmp->next;
	}
	return (*this);
}

List List::operator =(const List &l)
{
	node *tmp = l.head;
	List obj;
	obj.head = NULL;
	if (l.head == NULL)
		return obj;
	while (tmp != NULL)
	{
		obj.push(tmp->pos, tmp->a);
		tmp = tmp->next;
	}
	return obj;
}

node* List::get_head() const
{
	return head;
}
