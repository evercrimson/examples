#ifndef VECTOR_H
#define VECTOR_H

#include "exception.h"
#include "list.h"
#include "rational_number.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Vector
{
	List vec;
	size_t length;
	public:
	Vector();
	Vector(const Vector &v);
	Vector(const char *filename);
	Vector(const rational_number &a,const size_t size);
	void push(const size_t idx1, const rational_number &a);
	rational_number operator [](const size_t idx) const;
	rational_number& operator ()(const size_t idx);
	friend Vector operator +(const Vector &v, const rational_number &x);
	friend Vector operator -(const Vector &v, const rational_number &x);
	friend Vector operator *(const Vector &v, const rational_number &x);
	friend Vector operator /(const Vector &v, const rational_number &x);
	friend Vector operator +(const rational_number &x, const Vector &v);
	friend Vector operator -(const rational_number &x, const Vector &v);
	friend Vector operator *(const rational_number &x, const Vector &v);
	friend Vector operator /(const rational_number &x, const Vector &v);
	Vector operator =(const Vector &v);
	Vector operator +(const Vector &v) const;
	Vector operator -(const Vector &v) const;
	Vector operator *(const Vector &v) const;
	Vector operator -() const;
	void write(const char *filename);
	void print() const;
	size_t get_length() const;
};

#endif
