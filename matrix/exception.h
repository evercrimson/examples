#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>

#define ERR_MEM 1
#define ERR_FORMAT 2
#define ERR_NUL 3
#define ERR_SIZE 4
#define ERR_OUT_OF_RANGE 5
#define ERR_FILE_OPEN 6
#define ERR_FILE_FORMAT 7
#define ERR_NOLIST 8
#define ERR_WRONG_SIZE 9
#define ERR_NO_SIGN 10

using namespace std;

class Exception
{
	int err;
	public:
	Exception(int x);
	int get_err_no();
	void print_err();
};
#endif

