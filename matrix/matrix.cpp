#include "matrix.h"

using namespace std;

Matrix::Matrix()
{
	length1 = 0;
	length2 = 0;
}

Matrix::Matrix(const Matrix &m)
{
	mat = m.mat;
	length1 = m.length1;
	length2 = m.length2;
}

Matrix::Matrix(const rational_number &a, const size_t size1, const size_t size2)
{
	length1 = size1;
	length2 = size2;
	if (a != 0)
		for (size_t i = 0; i < length1; i++)
			for (size_t j = 0; j < length2; j++)
				mat.push(coords(i,j), a);
}

Matrix::Matrix(const rational_number &a, const size_t size)
{
	length1 = size;
	length2 = size;
	if (a != 0)
		for (size_t i = 0; i < length1; i++)
			mat.push(coords(i,i), a);
}

void Matrix::push(const size_t idx1, const size_t idx2, const rational_number &a)
{
	if ((idx1 >= length1) || (idx1 < 0) || (idx2 >= length2) || (idx2 < 0))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
	mat.push(coords(idx1, idx2), a);
}

Matrix::Matrix(const char *filename)
{
	ifstream f;
	int sign = 1;
	char c;
	size_t x = 0, y = 0;
	uint32_t x1 = 0, x2 = 0;
	bool no_length1 = 1, no_length2 = 1, end = 0, coordx = 0, coordy = 0, numerator = 0, denominator = 0;
	bool end_of_coordx = 0, end_of_coordy = 0, end_of_numerator = 0, end_of_length1 = 0;
	length1 = length2 = 0;

	f.open(filename);
	if (!f.is_open())
	{
		Exception t(ERR_FILE_OPEN);
		throw (t);
	}
	f.unsetf(ios_base::skipws);

	while (!f.eof())
	{
		f >> c;

		if (c == '\n')
			continue;

		if (c == '#')
		{
			while (c != '\n')
			{
				f >> c;
				if (f.eof())
				{
					end = 1;
					break;
				}
			}
			if (end) break;
			continue;
		}

		if ((c == 'm') && (no_length1))
		{
			f >> c;
			if ((c != 'a') || (f.eof()))
			{
				end = 1;
				break;
			}
			f >> c;
			if ((c != 't') || (f.eof()))
			{
				end = 1;
				break;
			}
			f >> c;
			if ((c != 'r') || (f.eof()))
			{
				end = 1;
				break;
			}
			f >> c;
			if ((c != 'i') || (f.eof()))
			{
				end = 1;
				break;
			}
			f >> c;
			if ((c != 'x') || (f.eof()))
			{
				end = 1;
				break;
			}

			while (c != '\n')
			{
				f >> c;
				if (( c>= '0') && (c <= '9'))
				{
					if (!end_of_length1)
					{
						no_length1 = 0;
						length1 = length1 * 10 + (c - '0');
					}
					else
					{
						no_length2 = 0;
						length2 = length2 * 10 + (c - '0');
					}
				}
				if ((c == ' ') && !(no_length1))
					end_of_length1 = 1;
				if (f.eof())
				{
					end = 1;
					break;
				}
			}
			if ((end) || (no_length1) || (no_length2))
				break;
		}
		
		if ((c >= '0') && (c <= '9'))
		{
			while (c != '\n')
			{
				if ((c >= '0') && (c <= '9'))
				{
					if ((!coordx) || (!end_of_coordx))
					{
						coordx = 1;
						x = x * 10 + (c - '0');
					}
					else
					if ((!coordy) || ((end_of_coordx) && !(end_of_coordy)))
					{
						coordy = 1;
						y = y * 10 + (c - '0');
					}
					else
					if ((!numerator) || ((end_of_coordy) && !(end_of_numerator)))
					{
						numerator = 1;
						x1 = x1 * 10 + (c - '0');
					}
					else
					if ((!denominator) || (end_of_numerator))
					{
						denominator = 1;
						x2 = x2 * 10 + (c - '0');
					}
				}
				if ((c == ' ') && (coordx))
					end_of_coordx = 1;
				if ((c == ' ') && (coordy))
					end_of_coordy = 1;
				if ((c == '-') && (end_of_coordy) && !(numerator))
					sign = -1;
				if ((c == '/') && (numerator))
					end_of_numerator = 1;
				if (f.eof())
				{
					end = 1;
					break;
				}
				f >> c;
			}
			if (!((coordx) && (coordy) && (numerator)))
			{
				f.close();
				Exception t(ERR_FILE_FORMAT);
				throw t;
			}
			if (!(denominator))
				(*this).push(x, y, rational_number(x1,1,sign));
			else
				if (x2 == 0)
				{
					f.close();
					Exception t(ERR_NUL);
					throw t;
				}
				else
					(*this).push(x, y, rational_number(x1,x2,sign));
			sign = 1;
			x = y = x1 = x2 = 0;
			coordx = coordy = numerator = denominator = end_of_coordx = end_of_coordy = end_of_numerator = 0;
		}

		if (end) break;
	}
	f.close();
	if ((no_length1) || (no_length2))
	{
		Exception t(ERR_FILE_FORMAT);
		throw (t);
	}
}

void Matrix::write(const char *filename)
{
	ofstream f;
	f.open(filename);
	if (!f.is_open())
	{
		Exception t(ERR_FILE_OPEN);
		throw (t);
	}
	node *tmp = mat.get_head();
	f << "matrix " << length1 << " " << length2 << endl;
	while (tmp!=NULL)
	{
		f << tmp->pos.x << " " << tmp->pos.y << " " << tmp->a.get_a() << "/" << tmp->a.get_b() << endl;
		tmp = tmp->next;
	}
	f.close();
}

rational_number Matrix::operator [](const coords idx) const
{
	if ((idx.x >= length1) || (idx.y >= length2))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
	return mat.find(idx);
}

Vector Matrix::operator [](const Matrix_column_coord idx) const
{
	return (*this).get_column(idx.get_idx());
}

Vector Matrix::operator [](const Matrix_row_coord idx) const
{
	return (*this).get_row(idx.get_idx());
}

Matrix::proxy Matrix::operator ()(const size_t idx1, const size_t idx2)
{
	if ((idx1 >= length1) || (idx2 >= length2))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
//	return mat.find1(coords(idx1, idx2));
	Matrix::proxy p(this, idx1, idx2);
	return p;
}

Matrix::proxy::proxy(Matrix *m1, size_t idx1, size_t idx2)
{
	m = m1;
	x = idx1;
	y = idx2;
}

rational_number Matrix::proxy::operator =(rational_number a)
{
	if (a != 0)
	{
		if ((*m)[coords(x, y)] != 0)
		{
			m->del(coords(x, y));
		}
		m->push(x, y, a);
	}
	else
		if (((*m)[coords(x, y)]) != 0)
		m->del(coords(x, y));
	return a;
}

Matrix Matrix::operator -() const
{
	Matrix obj;
	obj.mat = -mat;
	obj.length1 = length1;
	obj.length2 = length2;
	return obj;
}

Matrix Matrix::operator ~() const
{
	Matrix obj;
	obj.length1 = length2;
	obj.length2 = length1;
	for (size_t i = 0; i < length1; i++)
		for (size_t j = 0; j < length2; j++)
			if ((*this)[coords(i,j)]!=0)
				obj.push(j, i, (*this)[coords(i, j)]);
	return obj;
}

Matrix Matrix::operator =(const Matrix &m)
{
	mat.remove();
	mat = m.mat;
	length1 = m.length1;
	length2 = m.length2;
	return (*this);
}

Matrix operator +(const Matrix &m,const rational_number &x)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((m[coords(i,j)] + x) != 0)
				obj.push(i, j, m[coords(i,j)] + x);
	return obj;
}

Matrix operator -(const Matrix &m, const rational_number &x)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((m[coords(i,j)] - x) != 0)
				obj.push(i, j, m[coords(i,j)] - x);
	return obj;
}

Matrix operator *(const Matrix &m, const rational_number &x)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((m[coords(i,j)] * x) != 0)
				obj.push(i, j, m[coords(i,j)] * x);
	return obj;
}

Matrix operator /(const Matrix &m, const rational_number &x)
{
	if (x == 0)
	{
		Exception t(ERR_NUL);
		throw (t);
	}

	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((m[coords(i,j)] / x) != 0)
				obj.push(i, j, m[coords(i,j)] / x);
	return obj;
}

Matrix operator +(const rational_number &x,const Matrix &m)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((m[coords(i,j)] + x) != 0)
				obj.push(i, j, x + m[coords(i,j)]);
	return obj;
}

Matrix operator -(const rational_number &x, const Matrix &m)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((x - m[coords(i,j)]) != 0)
				obj.push(i, j, x - m[coords(i,j)]);
	return obj;
}

Matrix operator *(const rational_number &x, const Matrix &m)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
			if ((x * m[coords(i,j)]) != 0)
				obj.push(i, j, x * m[coords(i,j)]);
	return obj;
}

Matrix operator /(const rational_number &x, const Matrix &m)
{
	Matrix obj;
	obj.length1 = m.length1;
	obj.length2 = m.length2;
	for (size_t i = 0; i < m.length1; i++)
		for (size_t j = 0; j < m.length2; j++)
		{
			if (m[coords(i,j)] == 0)
			{
				Exception t(ERR_NUL);
				throw (t);
			}
			if ((x * m[coords(i,j)]) != 0)
				obj.push(i, j, x / m[coords(i,j)]);
		}
	return obj;
}

Matrix Matrix::operator +(const Matrix &m) const
{
	if ((length1 != m.length1) || (length2 != m.length2))
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}
	
	Matrix obj;
	obj.length1 = length1;
	obj.length2 = length2;
	for (size_t i = 0; i < length1; i++)
		for(size_t j = 0; j < length2; j++)
			if (((*this)[coords(i,j)] + m[coords(i,j)] ) != 0)
				obj.push(i, j, m[coords(i,j)] + (*this)[coords(i,j)]);
	return obj;
}

Matrix Matrix::operator -(const Matrix &m) const
{
	if ((length1 != m.length1) || (length2 != m.length2))
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}
	
	Matrix obj;
	obj.length1 = length1;
	obj.length2 = length2;
	for (size_t i = 0; i < length1; i++)
		for(size_t j = 0; j < length2; j++)
			if (((*this)[coords(i,j)] - m[coords(i,j)] ) != 0)
				obj.push(i, j, m[coords(i,j)] - (*this)[coords(i,j)]);
	return obj;
}

Matrix Matrix::operator *(const Matrix &m) const
{
	if (length2 != m.length1)
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}
	
	Matrix obj(0, length1, m.length2);
	rational_number r;
	for (size_t i = 0; i < length1; i++)
		for(size_t j = 0; j < m.length2; j++)
		{
			r = 0;
			for (size_t k = 0; k < length2; k++)
				r = r + (*this)[coords(i,k)] * m[coords(k,j)];
			if (r != 0)
				obj.push(i, j, r.make_canonical());
		}
	return obj;
}

void Matrix::print() const
{
	node *tmp = mat.get_head();

	if ((length1 == 0) || (length2 == 0))
	{
		Exception t(ERR_NOLIST);
		throw (t);
	}
	while (tmp != NULL)
	{
		cout << tmp->pos.x << " " << tmp->pos.y << " ";	
		tmp->a.printl();
		tmp = tmp->next;
	}
	if (mat.get_head() == NULL)
	{
		cout << "Только нули" << endl;
	}
}

Vector Matrix::get_column(const unsigned int column) const
{
	if(column >= length2)
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw t;
	}

	Vector obj(0, length1);
	rational_number a;
	for(unsigned int i = 0; i < length1; i++)
	{
		if (((*this)[coords(i, column)]) != 0)
			obj(i) = (*this)[coords(i, column)];
	}
	return obj;
}

Vector Matrix::get_row(const unsigned int row) const
{
	if(row >= length1)
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw t;
	}

	Vector obj(0, length2);
	rational_number a;
	for(unsigned int i = 0; i < length2; i++)
	{
		if (((*this)[coords(row, i)]) != 0)
			obj(i) = (*this)[coords(row, i)];
	}
	return obj;
}

Matrix Matrix::insert_row(const Vector& obj, const unsigned int row)
{
	if (row >= length1) 
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw t;
	}

	if(length2 != obj.get_length()) 
	{
		Exception t(ERR_WRONG_SIZE);
		throw t;
	}

	rational_number a;
	for(unsigned int i = 0; i < length2; i++)
	{
		if (( obj[i]) != 0)
			(*this)(row, i) = obj[i];
	}
	return *this;
}

Matrix Matrix::insert_column(const Vector& obj, const unsigned int column)
{
	if (column >= length2) 
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw t;
	}

	if(length1 != obj.get_length()) 
	{
		Exception t(ERR_WRONG_SIZE);
		throw t;
	}

	rational_number a;
	for(unsigned int i = 0; i < length1; i++)
	{
		if ((obj[i]) != 0)
			(*this)(i, column) = obj[i];
	}
	return *this;
}

/*Matrix Matrix::operator *(Matrix &m)
{
	if ((*this).length1 != m.length2)
	{
		Exception t(ERR_WRONG_SIZE);
		throw t;
	}
	Matrix obj(0, (*this).length2, m.length1);
}

int main()
{
	try
	{
		Matrix m1("matrix.txt");
		m1.print();
		(~m1).print();
	}
	catch (Exception t)
	{
		t.print_err();
	}
	return 0;
}*/

void Matrix::del(coords pos)
{
	mat.del(pos);
}
