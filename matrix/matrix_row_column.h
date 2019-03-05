#ifndef MATRIX_ROW_COLUMN_H
#define MATRIX_ROW_COLUMN_H

#include "matrix.h"

class Matrix;

class Matrix_column_coord
{
	unsigned int coord_index;
	Matrix *mat;
	public:
	Matrix_column_coord(unsigned int c);
        Matrix_column_coord(Matrix *m, unsigned int c);
	Vector column;
	void sync_to();
	void sync_from();
	unsigned int get_idx() const;
};

class Matrix_row_coord
{
	unsigned int coord_index;
	Matrix *mat;
	public:
	Matrix_row_coord(unsigned int c);
	Matrix_row_coord(Matrix *m, unsigned int c);
	Vector row;
	void sync_to();
	void sync_from();
	unsigned int get_idx() const;
};

#endif
