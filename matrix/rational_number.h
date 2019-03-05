#ifndef RATIONAL_NUMBER_H
#define RATIONAL_NUMBER_H

#include <stdint.h>
#include "exception.h"
#include <cmath>

using namespace std;

class rational_number
{
	uint32_t a;
	uint32_t b;
	int sign;
public:
	rational_number();
	rational_number(const rational_number &x);
	rational_number(const double y);
	rational_number(const bool &x);
	rational_number(const unsigned char &x);
	rational_number(const unsigned short int &x);
	rational_number(const unsigned int &x);
	rational_number(const unsigned long int &x);
	rational_number(const unsigned long long &x);
	rational_number(const char &x);
	rational_number(const short int &x);
	rational_number(const int &x);
	rational_number(const long int &x);
	rational_number(const long long &x);
	rational_number(uint32_t x1,uint32_t x2, int x3);
	rational_number(const char *s);
	rational_number(const char *s1, const char *s2);
	void operator =(const rational_number &x);
	void operator +=(const rational_number &x);
	void operator -=(const rational_number &x);
	void operator *=(const rational_number &x);
	void operator /=(const rational_number &x);
	rational_number& operator ++();
	rational_number operator ++(int g);
	friend rational_number operator +(const rational_number &x1, const rational_number &x2);
	friend rational_number operator -(const rational_number &x1, const rational_number &x2);
	rational_number operator -() const;
	friend rational_number operator *(const rational_number &x1, const rational_number &x2);
	friend rational_number operator /(const rational_number &x1, const rational_number &x2);
	friend bool operator >(const rational_number &x1, const rational_number &x2);
	friend bool operator <(const rational_number &x1, const rational_number &x2);
	friend bool operator >=(const rational_number &x1, const rational_number &x2);
	friend bool operator <=(const rational_number &x1, const rational_number &x2);
	friend bool operator ==(const rational_number &x1, const rational_number &x2);
	friend bool operator !=(const rational_number &x1, const rational_number &x2);
	rational_number make_canonical();
	rational_number get_number_part() const;
	rational_number get_fractional_part() const;
	void set_a(uint32_t g);
	uint32_t get_a() const;
	void set_b(uint32_t g);
	uint32_t get_b() const;
	uint32_t floor() const;
	uint32_t round() const;
	int to_int() const;
	long int to_long_int() const;
	short to_short() const;
	void print() const;
	void printl() const;
	char* to_string() const;
	friend size_t digits(size_t q);
};

#endif
