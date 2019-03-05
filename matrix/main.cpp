#include "exception.h"
#include "rational_number.h"
#include "vector.h"
#include "list.h"
#include "matrix.h"
int main()
{
	rational_number a("-1/1");
	a.printl();
	try
	{
		rational_number b(1, 0, 1);
		b.printl();
	}
	catch (Exception t)
	{
		t.print_err();
	}
	try
	{
		Vector v("vector.txt");
		v.print();
	}
	catch (Exception t)
	{
		t.print_err();
	}
	rational_number a1(3, 6, 1);
	a1.printl();
	a1.make_canonical();
	a1.printl();
	Vector v1 (rational_number(3,4,-1), 10);
	v1.print();
	v1.write("vector_out.txt");
	try
	{
		Matrix m1("matrix.txt");
		Matrix m2(1, 3);
		(m1 * m2).print();
		(rational_number(5,3,-1) + 2).printl();
		(m1[Matrix_row_coord(2)]).print();
		m1(1, 1) = 0;
		m1(0, 2) = rational_number(225.358);
		m1.print();
	/*	Matrix m3(0,10,10);

		m3(coords(0,0))=1;
		m3(coords(2,2))=0.2;

		cout << "size " << sizeof(m3) << endl;
		m3.print();
		cout << endl;

		m3(coords(2,2))=0;

		m3.print();
		cout << endl;
		
		Matrix_row r1;*/

	}
	catch (Exception t)
	{
		t.print_err();
	}
	return 0;
}
