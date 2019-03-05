#include "vector.h"

using namespace std;

Vector::Vector()
{
	length = 0;
}

Vector::Vector(const Vector &v)
{
	vec = v.vec;
	length = v.length;
}

Vector::Vector(const rational_number &a, const size_t size)
{
	length = size;
	if (a != 0)
		for (size_t i = 0; i < length; i++)
		{
			vec.push(coords(i,0), a);
		}
}

void Vector::push(const size_t idx, const rational_number &a)
{
	if ((idx >= length) || (idx < 0))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
	vec.push(coords(idx, 0), a);
}

Vector::Vector(const char *filename)
{
	ifstream f;
	int sign = 1;
	char c;
	size_t x = 0;
	uint32_t x1 = 0, x2 = 0;
	bool no_length = 1, end = 0, coord = 0, numerator = 0, denominator = 0;
	bool end_of_coord = 0, end_of_numerator = 0;
	length = 0;
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

		if ((c == 'v') && (no_length))
		{
			f >> c;
			if ((c != 'e') || (f.eof()))
			{
				end = 1;
				break;
			}
			f >> c;
			if ((c != 'c') || (f.eof()))
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
			if ((c != 'o') || (f.eof()))
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

			while (c != '\n')
			{
				f >> c;
				if (( c>= '0') && (c <= '9'))
				{
					no_length = 0;
					length = length * 10 + (c - '0');
				}
				if (f.eof())
				{
					end = 1;
					break;
				}
			}
			if ((end) || (no_length))
				break;
		}
		
		if ((c >= '0') && (c <= '9'))
		{
			while (c != '\n')
			{
				if ((c >= '0') && (c <= '9'))
				{
					if ((!coord) || (!end_of_coord))
					{
						coord = 1;
						x = x * 10 + (c - '0');
					}
					else
					if ((!numerator) || ((end_of_coord) && !(end_of_numerator)))
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
				if ((c == ' ') && (coord))
					end_of_coord = 1;
				if ((c == '-') && (end_of_coord) && !(numerator))
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
			if (!((coord) && (numerator)))
			{
				f.close();
				Exception t(ERR_FILE_FORMAT);
				throw t;
			}
			if (!(denominator))
				(*this).push(x, rational_number(x1,1,sign));
			else
				if (x2 == 0)
				{
					f.close();
					Exception t(ERR_NUL);
					throw t;
				}
				else
					(*this).push(x, rational_number(x1,x2,sign));
			sign = 1;
			x = x1 = x2 = 0;
			coord = numerator = denominator = end_of_coord = end_of_numerator = 0;
		}

		if (end) break;
	}
	f.close();
	if (no_length) 
	{
		Exception t(ERR_FILE_FORMAT);
		throw (t);
	}
}

void Vector::write(const char *filename)
{
	ofstream f;
	f.open(filename);
	if (!f.is_open())
	{
		Exception t(ERR_FILE_OPEN);
		throw (t);
	}
	node *tmp = vec.get_head();
	f << "vector " << length << endl;
	while (tmp!=NULL)
	{
		f << tmp->pos.x << " " << tmp->a.get_a() << "/" << tmp->a.get_b() << endl;
		tmp = tmp->next;
	}
	f.close();
}

rational_number Vector::operator [](const size_t idx) const
{
	if ((idx >= length) || (idx < 0))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
	return vec.find(coords(idx,0));
}

rational_number& Vector::operator ()(const size_t idx)
{
	if ((idx >= length) || (idx < 0))
	{
		Exception t(ERR_OUT_OF_RANGE);
		throw (t);
	}
	return vec.find1(coords(idx,0));
}

Vector Vector::operator -() const
{
	Vector obj;
	obj.vec = -vec;
	obj.length = length;
	return obj;
}

Vector Vector::operator =(const Vector &v)
{
	vec.remove();
	vec = v.vec;
	length = v.length;
	return (*this);
}

Vector operator +(const Vector &v,const rational_number &x)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] + x) != 0)
			obj.push(i, v[i] + x);
	}
	return obj;
}

Vector operator -(const Vector &v, const rational_number &x)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] - x) != 0)
			obj.push(i, v[i] - x);
	}
	return obj;
}

Vector operator *(const Vector &v, const rational_number &x)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] * x) != 0)
			obj.push(i, v[i] * x);
	}
	return obj;
}

Vector operator /(const Vector &v, const rational_number &x)
{
	if (x == 0)
	{
		Exception t(ERR_NUL);
		throw (t);
	}

	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] + x) != 0)
			obj.push(i, v[i] + x);
	}
	return obj;
}

Vector operator +(const rational_number &x,const Vector &v)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] + x) != 0)
			obj.push(i, v[i] + x);
	}
	return obj;
}

Vector operator -(const rational_number &x, const Vector &v)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((x - v[i]) != 0)
			obj.push(i, x - v[i]);
	}
	return obj;
}

Vector operator *(const rational_number &x, const Vector &v)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if ((v[i] * x) != 0)
			obj.push(i, v[i] * x);
	}
	return obj;
}

Vector operator /(const rational_number &x, const Vector &v)
{
	Vector obj;
	obj.length = v.length;
	for (size_t i = 0; i < v.length; i++)
	{
		if (v[i] == 0)
		{
			Exception t(ERR_NUL);
			throw (t);
		}
		if ((x / v[i]) != 0)
			obj.push(i, x / v[i]);
	}
	return obj;
}

Vector Vector::operator +(const Vector &v) const
{
	if (length != v.length)
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}
	
	Vector obj;
	obj.length = length;
	for (size_t i = 0; i < length; i++)
	{
		if (((*this)[i] + v[i] ) != 0)
			obj.push(i, v[i] + (*this)[i]);
	}
	return obj;
}

Vector Vector::operator -(const Vector &v) const
{
	if (length != v.length)
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}
	
	Vector obj;
	obj.length = length;
	for (size_t i = 0; i < length; i++)
	{
		if (((*this)[i] - v[i] ) != 0)
			obj.push(i, v[i] - (*this)[i]);
	}
	return obj;
}

Vector Vector::operator *(const Vector &v) const
{
	if (length != v.length)
	{
		Exception t(ERR_WRONG_SIZE);
		throw (t);
	}

	Vector obj;
	obj.length = length;
	for (size_t i = 0; i < length; i++)
	{
		if (((*this)[i] * v[i]) != 0)
			obj.push(i, v[i] * (*this)[i]);
	}
	return obj;
}

void Vector::print() const
{
	node *tmp = vec.get_head();

	if (length == 0)
	{
		Exception t(ERR_NOLIST);
		throw (t);
	}
	while (tmp != NULL)
	{
		cout << tmp->pos.x << " ";	
		tmp->a.printl();
		tmp = tmp->next;
	}
	if (vec.get_head() == NULL)
	{
		cout << "Только нули" << endl;
	}
}

size_t Vector::get_length() const
{
	return length;
}
	
//Vector Vector::operator *(Matrix &m)
//{
//}
	
