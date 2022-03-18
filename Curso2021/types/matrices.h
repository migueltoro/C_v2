/*
 * matrices.h
 *
 *  Created on: 13 dic. 2019
 *      Author: migueltoro
 */

#ifndef MATRICES_H_
#define MATRICES_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "preconditions.h"
#include "types.h"
#include "list.h"

typedef struct {
	type * type;
	char * datos;
	int nf;
	int nc;
}matrix;

typedef struct {
	matrix m0;
	matrix m1;
	matrix m2;
	matrix m3;
}matrix_views;

matrix matrix_of_array(void * a, int nf, int nc, type * type_element);
matrix matrix_of(int nf, int nc, type * type_element);
matrix matrix_of_file(char * file, type * type, int nf, int nc);

void * matrix_get(matrix * s, int f, int c);
void matrix_set(matrix * s, int f, int c, void * value);
int matrix_nf(matrix * s);
int matrix_nc(matrix * s);
void * matrix_center(matrix * s);
list matrix_corners(matrix * s);

int matrix_get_int(matrix * s, int f, int c);

void matrix_free(matrix * m);

matrix matrix_submatrix(matrix * s, int i0, int j0, int nf, int nc);
matrix matrix_view(matrix * s, int ns);
void matrix_print(matrix * s, char * text);
matrix matrix_copy(matrix * in);
void  matrix_copy_2(matrix * out, matrix * in);
matrix_views views_of_matrix(matrix * s);
matrix matrix_compose(matrix * m0, matrix * m1, matrix * m2, matrix * m3);

matrix multiply_iterativa(matrix * in1, matrix * in2);
matrix multiply_recursiva(matrix * in1, matrix * in2);
matrix sum_iterativa(matrix * in1, matrix * in2);
matrix sum_recursiva(matrix * in1, matrix * in2);

void test_matrices_1();
void test_matrices_2();
void test_matrices_3();
void test_matrices_4();

#endif /* MATRICES_H_ */
