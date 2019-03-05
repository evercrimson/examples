#ifndef MATRIX_H
#define MATRIX_H

#include "exception.h"
#include "list.h"
#include "rational_number.h"
#include "vector.h"
#include "matrix_row_column.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Matrix_column_coord;
class Matrix_row_coord;

class Matrix
{
	List mat;
	size_t length1;
	size_t length2;
	public:
	class proxy
	{
		Matrix *m;
		size_t x, y;
		public:
		proxy(Matrix *m1, size_t idx1, size_t idx2);
		rational_number operator =(rational_number a);
	};
	Matrix();
	Matrix(const Matrix &m);
	Matrix(const char *filename);
	Matrix(const rational_number &a, const size_t size1, const size_t size2);
	Matrix(const rational_number &a, const size_t size);
	void push(const size_t idx1, const size_t idx2, const rational_number &a);
	void del(coords pos);
	rational_number operator [](const coords idx) const;
	Vector operator [](const Matrix_column_coord idx) const;
	Vector operator [](const Matrix_row_coord idx) const;
	Matrix::proxy operator ()(const size_t idx1, const size_t idx2);
	friend Matrix operator +(const Matrix &m, const rational_number &x);
	friend Matrix operator -(const Matrix &m, const rational_number &x);
	friend Matrix operator *(const Matrix &m, const rational_number &x);
	friend Matrix operator /(const Matrix &m, const rational_number &x);
	friend Matrix operator +(const rational_number &x, const Matrix &m);
	friend Matrix operator -(const rational_number &x, const Matrix &m);
	friend Matrix operator *(const rational_number &x, const Matrix &m);
	friend Matrix operator /(const rational_number &x, const Matrix &m);
	Matrix operator =(const Matrix &m);
	Matrix operator +(const Matrix &m) const;
	Matrix operator -(const Matrix &m) const;
	Matrix operator *(const Matrix &m) const;
	Matrix operator -() const;
	Matrix operator ~() const;
	void write(const char *filename);
	void print() const;
	Vector get_column(const unsigned int column) const;
	Vector get_row(const unsigned int row) const;
	Matrix insert_column(const Vector& obj, const unsigned int column);
	Matrix insert_row(const Vector& obj, const unsigned int row);
};	

#endif
