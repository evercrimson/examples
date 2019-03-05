#include "rational_number.h"
#include <cmath>

rational_number::rational_number()
{
	a = 0;
	b = 1;
	sign = 1;
}

rational_number::rational_number(const double y)
{
	rational_number obj;
	double x = y;
	if (x >= 0)
		obj.sign = 1;
	else
		obj.sign = -1;
	x = abs(x);
	obj.b = 1;
	while (::floor(x) - x != 0)
	{
		x = x * 10;
		obj.b = obj.b * 10;
	}
	obj.a = ::floor(x);
	obj.make_canonical();
	a = obj.a;
	b = obj.b;
	sign = obj.sign;
}

rational_number::rational_number(uint32_t x1, uint32_t x2, int x3)
{
	if (x2 == 0) 
	{
		Exception t(ERR_NUL);
		throw (t);
	}
	a = x1;

	if (x3 == 0)
	{
		Exception t(ERR_NO_SIGN);
		throw (t);
	}

	if (a == 0)
		b = 1;
	else
		b = x2;
	if (x3 > 0)
		sign = 1;
	else
		sign = -1;
}

rational_number::rational_number(const rational_number &x)
{
	a = x.a;
	b = x.b;
	sign = x.sign;
}

rational_number::rational_number(const bool &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const unsigned char &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const unsigned short int &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const unsigned int &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const unsigned long int &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const unsigned long long &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const char &x)
{
	a = x;
	b = 1;
}

rational_number::rational_number(const short int &x)
{
	if (x < 0)
	{
		sign = -1;
		a = -x;
	}
	else
	{
		sign = 1;
		a = x;
	}
	b = 1;
}

rational_number::rational_number(const int &x)
{
	if (x < 0)
	{
		sign = -1;
		a = -x;
	}
	else
	{
		sign = 1;
		a = x;
	}
	b = 1;
}

rational_number::rational_number(const long int &x)
{
	if (x < 0)
	{
		sign = -1;
		a = -x;
	}
	else
	{
		sign = 1;
		a = x;
	}
	b = 1;
}

rational_number::rational_number(const long long &x)
{
	if (x < 0)
	{
		sign = -1;
		a = -x;
	}
	else
	{
		sign = 1;
		a = x;
	}
	b = 1;
}

rational_number::rational_number(const char *s)
{
	bool numerator = 0, denominator = 0, nul = 1;
	int c;
	a = 0;
	b = 0;

	if (s == NULL)
	{
		Exception t(ERR_FORMAT);
		throw t;
	}

	if (s[0] == '-')
		sign = -1;
	else
		sign = 1;

	for (int i=0;;i++)
	{
		if (s[i] == '\0')
			break;
		if ((s[i] >= '0') && (s[i] <= '9'))
		{
			switch (s[i])
			{
				case '0':  c = 0; break;
				case '1':  c = 1; break;
				case '2':  c = 2; break;
				case '3':  c = 3; break;
				case '4':  c = 4; break;
				case '5':  c = 5; break;
				case '6':  c = 6; break;
				case '7':  c = 7; break;
				case '8':  c = 8; break;
				case '9':  c = 9; break;
			}
			if (!denominator)
			{
				numerator = 1;
				a = a * 10 + c;
			}
			else
			{
				if (c != 0)
					nul = 0;
				b = b * 10 + c;
			}
		}
		if (s[i] == '/')
			denominator = 1;
	}

	if (nul)
	{
		Exception t(ERR_NUL);
		throw(t);
	}
	if (numerator)
	{
		if (b == 0)
			b = 1;
	}
	else
	{
		Exception t(ERR_FORMAT);
		throw(t);
	}
}

rational_number::rational_number(const char *s1, const char *s2)
{
	int c;
	bool numerator = 0, denominator = 0;
	sign = 1;
	a = 0;
	b = 0;

	if ((s1 == NULL) || (s2 == NULL))
	{
	}
	if (s1[0] == '-')
		sign = -1;
	if (s2[0] == '-')
		sign = sign * -1;

	for (int i=0;;i++)
	{
		if (s1[i] == '\0')
			break;
		if ((s1[i] >= '0') && (s1[i] <= '9'))
		{
			numerator = 1;
			switch (s1[i])
			{
				case '0':  c = 0; break;
				case '1':  c = 1; break;
				case '2':  c = 2; break;
				case '3':  c = 3; break;
				case '4':  c = 4; break;
				case '5':  c = 5; break;
				case '6':  c = 6; break;
				case '7':  c = 7; break;
				case '8':  c = 8; break;
				case '9':  c = 9; break;
			}
			a = a*10 + c;
		}
	}

	for (int i=0;;i++)
	{
		if (s2[i] == '\0')
			break;
		if ((s2[i] >= '0') && (s2[i] <= '9'))
		{
			denominator = 1;
			switch (s2[i])
			{
				case '0':  c = 0; break;
				case '1':  c = 1; break;
				case '2':  c = 2; break;
				case '3':  c = 3; break;
				case '4':  c = 4; break;
				case '5':  c = 5; break;
				case '6':  c = 6; break;
				case '7':  c = 7; break;
				case '8':  c = 8; break;
				case '9':  c = 9; break;
			}
			b = b*10 + c;
		}
	}
	if (b == 0)
	{
	}
	if ((!denominator) || (!numerator))
	{
		Exception t(ERR_FORMAT);
		throw (t);
	}
}	

void rational_number::operator =(const rational_number &x)
{
	a = x.a;
	b = x.b;
	sign = x.sign;
}

void rational_number::operator +=(const rational_number &x)
{
	a = sign * a * x.b + x.sign * b * x.a;
	b = x.b * x.a;
	if (a >= 0)
		sign = 1;
	else
	{
		sign = -1;
		a = a * -1;
	}
	make_canonical();
}

void rational_number::operator -=(const rational_number &x)
{
	a = sign * a * x.b - x.sign * b * x.a;
	b = b * x.b;
	if (a >= 0)
		sign = 1;
	else
	{
		sign = -1;
		a = a * -1;
	}
	make_canonical();
}

void rational_number::operator *=(const rational_number &x)
{
	a = sign * a * x.sign * x.a;
	b = b * x.b;
	if (a >= 0)
		sign = 1;
	else
	{
		sign = -1;
		a = a * -1;
	}
	make_canonical();
}

void rational_number::operator /=(const rational_number &x)
{
	if (x.a == 0)
	{
		Exception t(ERR_NUL);
		throw (t);
	}

	a = sign * a * x.sign * x.b;
	b = b * x.a;
	if (a >= 0)
		sign = 1;
	else
	{
		sign = -1;
		a = a * -1;
	}
	make_canonical();
}

rational_number& rational_number::operator ++()
{
	a = a + b;
	return (*this);
}

rational_number rational_number::operator ++(int g)
{
	rational_number obj(*this);
	a++;
	return obj;
}

rational_number operator +(const rational_number &x1, const rational_number &x2)
{
	rational_number obj;
	if ((x1.sign < 0) && (x2.sign < 0))
	{
		obj.a = x1.a*x2.b + x2.a*x1.b;
		obj.sign = -1;
	}
	if ((x1.sign > 0) && (x2.sign > 0))
	{
		obj.a = x1.a*x2.b + x2.a*x1.b;
		obj.sign = 1;
	}
	if ((x1.sign > 0) && (x2.sign < 0))
	{
		if (x1.a*x2.b >= x2.a*x1.b)
		{
			obj.a = x1.a*x2.b - x2.a*x1.b;
			obj.sign = 1;
		}
		else
		{
			obj.a = x2.a*x1.b - x1.a*x2.b;
			obj.sign = -1;
		}
	}
	if ((x1.sign < 0) && (x2.sign > 0))
	{
		if (x1.a*x2.b > x2.a*x1.b)
		{
			obj.a = x1.a*x2.b - x2.a*x1.b;
			obj.sign = -1;
		}
		else
		{
			obj.a = x2.a*x1.b - x1.a*x2.b;
			obj.sign = 1;
		}
	}
	obj.b = x1.b*x2.b;
	obj.make_canonical();
	return obj;
}

rational_number operator -(const rational_number &x1, const rational_number &x2)
{
	/*rational_number obj;
	if ((x1.sign > 0) && (x2.sign < 0))
	{
		obj.a = x1.a*x2.b + x2.a*x1.b;
		obj.sign = 1;
	}
	if ((x1.sign < 0) && (x2.sign > 0))
	{
		obj.a = x1.a*x2.b + x2.a*x1.b;
		obj.sign = -1;
	}
	if ((x1.sign > 0) && (x2.sign < 0))
		if (x1.a*x2.b >= x2.a*x1.b)
		{
			obj.a = x1.a*x2.b - x2.a*x1.b;
			obj.sign = 1;
		}
		else
		{
			obj.a = x2.a*x1.b - x1.a*x2.b;
			obj.sign = -1;
		}
	if ((x1.sign < 0) && (x2.sign > 0))
		if (x1.a*x2.b > x2.a*x1.b)
		{
			obj.a = x1.a*x2.b - x2.a*x1.b;
			obj.sign = -1;
		}
		else
		{
			obj.a = x2.a*x1.b - x1.a*x2.b;
			obj.sign = 1;
		}
	obj.b = x1.b*x2.b;
	obj.make_canonical();
	return obj;*/
	return (x1 + (-x2));
}

rational_number rational_number::operator -() const
{
	rational_number obj = (*this);
	obj.sign = obj.sign * -1;
	return obj;
}

rational_number operator *(const rational_number &x1, const rational_number &x2)
{
	rational_number obj;
	obj.a = x1.a*x2.a;
	obj.b = x1.b*x2.b;
	if (((x1.sign > 0) && (x2.sign > 0)) || ((x1.sign < 0) && (x2.sign < 0)))
		obj.sign = 1;
	else
		obj.sign = -1;
	obj.make_canonical();
	return obj;
}

rational_number operator /(const rational_number &x1, const rational_number &x2)
{
	if (x2.a == 0)
	{
		Exception t(ERR_NUL);
		throw (t);
	}

	rational_number obj;
	obj.a = x1.a*x2.b;
	obj.b = x1.b*x2.a;
	if (((x1.sign > 0) && (x2.sign > 0)) || ((x1.sign < 0) && (x2.sign < 0)))
		obj.sign = 1;
	else
		obj.sign = -1;
	obj.make_canonical();
	return obj;
}

bool operator ==(const rational_number &x1, const rational_number &x2)
{
	rational_number obj1 = x1;
	rational_number obj2 = x2;
	obj1.make_canonical();
	obj2.make_canonical();
	if ((obj1.a == obj2.a) && (obj1.b == obj2.b) && (obj1.sign == obj2.sign))
		return 1;
	else
		return 0;
}

bool operator >(const rational_number &x1, const rational_number &x2)
{
	rational_number obj1 = x1;
	rational_number obj2 = x2;
	if (x1 == x2)
		return 0;
	if (x1.sign != x2.sign)
	{
		if (x1.sign == 1)
			return 1;
		else
			return 0;
	}
	obj1.make_canonical();
	obj2.make_canonical();
	obj1.a = obj1.a * obj2.b;
	obj2.a = obj2.a * obj1.b;
	if (obj1.a > obj2.a)
		if (obj1.sign == 1)
			return 1;
		else
			return 0;
	else
		if (obj1.sign == 1)
			return 0;
		else
			return 1;
}

bool operator >=(const rational_number &x1, const rational_number &x2)
{
	rational_number obj1 = x1;
	rational_number obj2 = x2;
	if (x1 == x2)
		return 1;
	if (x1.sign != x2.sign)
	{
		if (x1.sign == 1)
			return 1;
		else
			return 0;
	}
	obj1.make_canonical();
	obj2.make_canonical();
	obj1.a = obj1.a * obj2.b;
	obj2.a = obj2.a * obj1.b;
	if (obj1.a >= obj2.a)
		if (obj1.sign == 1)
			return 1;
		else
			return 0;
	else
		if (obj1.sign == 1)
			return 0;
		else
			return 1;
}

bool operator <(const rational_number &x1, const rational_number &x2)
{
	rational_number obj1 = x1;
	rational_number obj2 = x2;
	if (x1 == x2)
		return 0;
	if (x1.sign != x2.sign)
	{
		if (x1.sign == 1)
			return 0;
		else
			return 1;
	}
	obj1.make_canonical();
	obj2.make_canonical();
	obj1.a = obj1.a * obj2.b;
	obj2.a = obj2.a * obj1.b;
	if (obj1.a >= obj2.a)
		if (obj1.sign == 1)
			return 0;
		else
			return 1;
	else
		if (obj1.sign == 1)
			return 1;
		else
			return 0;
}

bool operator <=(const rational_number &x1, const rational_number &x2)
{
	rational_number obj1 = x1;
	rational_number obj2 = x2;
	if (x1 == x2)
		return 1;
	if (x1.sign != x2.sign)
	{
		if (x1.sign == 1)
			return 0;
		else
			return 1;
	}
	obj1.make_canonical();
	obj2.make_canonical();
	obj1.a = obj1.a * obj2.b;
	obj2.a = obj2.a * obj1.b;
	if (obj1.a > obj2.a)
		if (obj1.sign == 1)
			return 0;
		else
			return 1;
	else
		if (obj1.sign == 1)
			return 1;
		else
			return 0;
}

bool operator !=(const rational_number &x1, const rational_number &x2)
{
	if (!(x1 == x2))
		return 1;
	else
		return 0;
}

rational_number rational_number::make_canonical()
{
	uint32_t nod = 1;
	if (a == 0)
	{
		b = 1;
		sign = 1;
	}
	else
	{
		for (uint32_t i = a + b; i > 0; i --)
		{
			if ((a % i == 0) && (b % i == 0))
			{
				nod = i;
				break;
			}
		}
		a = a / nod;
		b = b / nod;
	}
	return *this;
}

rational_number rational_number::get_number_part() const
{
	rational_number obj;
	obj.a = a/b;
	obj.b = 1;
	obj.sign = sign;
	return obj;
}

rational_number rational_number::get_fractional_part() const
{
	rational_number obj;
	obj.a = a - (a/b)*b;
	obj.b = b;
	obj.sign = sign;
	return obj;
}

void rational_number::set_a(uint32_t g)
{
	a = g;
}

uint32_t rational_number::get_a() const
{
	return a;
}
void rational_number::set_b(uint32_t g)
{
	b = g;
}

uint32_t rational_number::get_b() const
{
	return b;
}

uint32_t rational_number::floor() const
{
	return a/b;
}

uint32_t rational_number::round() const
{
	return (a+2)/(b+1);
}

void rational_number::print() const
{
	if (sign == -1) cout << "-";
	cout << a << "/" << b;
}

void rational_number::printl() const
{
	if (sign == -1) cout << "-";
	cout << a << "/" << b << endl;
}

size_t digits(size_t q)
{
	if (q == 0)
		return 1;
	size_t p = 0;
	while (q != 0)
	{
		q = q >> 1;
		p = p + 1;
	}
	return p;
}

char* rational_number::to_string() const
{
	char *s = NULL;
	size_t size = 0;
	if (sign == -1)
		size = 1;
	size = size + digits(a) + digits(b);
	s = new char(size + 2);
	if (s == NULL)
	{
		Exception t(ERR_MEM);
		throw (t);
	}
	if (sign == -1)
		s[0] = '-';
	
	return s;
}
