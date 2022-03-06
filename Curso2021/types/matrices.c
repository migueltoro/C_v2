/*
 * matrices.c
 *
 *  Created on: 13 dic. 2019
 *      Author: migueltoro
 */

#include "matrices.h"

matrix matrix_of_array(void * a, int nf, int nc, type * type_element){
	type * t = type_element;
	matrix r = {t,a,nf,nc};
	return r;
}

matrix matrix_of(int nf, int nc, type * type){
	void * array = malloc(nf*nc*type->size);
	return matrix_of_array(array, nf, nc, type);
}

matrix matrix_of_file(char *file, type *type, int nf, int nc) {
	matrix m = matrix_of(nf,nc,type);
	iterator it1 = iterable_file_string_fix_tam(file,string_fix_tam);
	char e[type->size];
	int f = 0;
	while (iterable_has_next(&it1)) {
		char *linea = (char*) iterable_next(&it1);
		iterator it2 = iterable_split_text_tam(linea," ,",50);
		int c = 0;
		while (iterable_has_next(&it2)) {
			char *tx = iterable_next(&it2);
			parse(e, tx, type);
			matrix_set(&m,f,c,e);
			c++;
		}
		f++;
	}
	return m;
}


int matrix_nf(matrix * s){
	return s->nf;
}

int matrix_nc(matrix * s){
	return s->nc;
}

void * matrix_get(matrix * s, int f, int c){
	int size = s->type->size;
	return s->datos + size*(f*s->nc+c);
}

int matrix_get_int(matrix * s, int f, int c) {
	return to_int(matrix_get(s,f,c));
}

void matrix_set(matrix * s, int f, int c, void * value){
	int size = s->type->size;
	copy_size(s->datos + size*(f*s->nc+c), value, s->type->size);
}

void matrix_set_value(matrix * s, void * value) {
	int i, j;
	for (i = 0; i < s->nf; i++) {
		for (j = 0; j < s->nc; j++)
			matrix_set(s,i,j,value);
	}
}

void * matrix_center(matrix * s){
	int nf = matrix_nf(s)/2;
	int nc = matrix_nc(s)/2;
	return matrix_get(s,nf,nc);
}

list matrix_corners(matrix * s){
	list r = list_empty(s->type);
	int nf = matrix_nf(s);
	int nc = matrix_nc(s);
	void * e = matrix_get(s,0,0);
	list_add(&r,e);
	e = matrix_get(s,0,nc-1);
	list_add(&r,e);
	e = matrix_get(s,nf-1,0);
	list_add(&r,e);
	e = matrix_get(s,nf-1,nc-1);
	list_add(&r,e);
	return r;
}

matrix matrix_submatrix(matrix * s, int i0, int j0, int nf, int nc){
	check_argument(i0+nf <= s->nf,__FILE__, __LINE__, "Submatrix no válida");
	check_argument(j0+nc <= s->nc,__FILE__, __LINE__, "Submatrix no válida");
	matrix r = matrix_of(nf, nc, s->type);
	for(int i = 0;i<nf;i++){
		for(int j = 0; j< nc; j++){
			matrix_set(&r,i,j,matrix_get(s,i0+i,j0+j));
		}
	}
	return r;
}

matrix matrix_view(matrix * s, int nv){
	int nf = s->nf/2;
	int nc = s->nc/2;
	matrix r;
	r.datos= s->datos;
	r.type = s->type;
	switch(nv){
	case 0:  r = matrix_submatrix(s,0,0,nf,nc); break;
	case 1:  r = matrix_submatrix(s,0,nc,nf,s->nc-nc); break;
	case 2:  r = matrix_submatrix(s,nf,0,s->nf-nf,nc);  break;
	case 3:  r = matrix_submatrix(s,nf,nc,s->nf-nf,s->nc-nc);  break;
	default: check_argument(false,__FILE__, __LINE__, "Opción no válida");
	}
	return r;
}

matrix_views views_of_matrix(matrix * s){
	matrix_views r;
	r.m0 = matrix_view(s,0);
	r.m1 = matrix_view(s,1);
	r.m2 = matrix_view(s,2);
	r.m3 = matrix_view(s,3);
	return r;
}

void matrix_free(matrix * m){
	free(m->type);
	free(m->datos);
}

void matrix_print(matrix * s, char * name) {
	printf("\n%s\nnf = %d, nc = %d\n",name,s->nf,s->nc);
	char mem[Tam_String];
	int i, j;
	for (i = 0; i < s->nf; i++) {
		for (j = 0; j < s->nc; j++)
			printf("%4s ", tostring(matrix_get(s,i,j),mem,s->type));
		printf("\n");
	}
}

matrix matrix_copy(matrix * in) {
	matrix r = matrix_of(in->nf,in->nc,in->type);
	int i, j;
	for (i = 0; i < in->nf; i++) {
		for (j = 0; j < in->nc; j++){
			void * val = matrix_get(in,i,j);
			matrix_set(&r,i,j,val);
		}
	}
	return r;
}

void  matrix_copy_2(matrix * out, matrix * in) {
	int i, j;
	for (i = 0; i < in->nf; i++) {
		for (j = 0; j < in->nc; j++){
			void * val = matrix_get(in,i,j);
			matrix_set(out,i,j,val);
		}
	}
}

matrix matrix_compose(matrix *m0, matrix *m1, matrix *m2, matrix *m3) {
	matrix out = matrix_of(m0->nf + m2->nf, m0->nc + m1->nc, m0->type);
	int i, j;
	for (i = 0; i < m0->nf; i++) {
		for (j = 0; j < m0->nc; j++) {
			void *val = matrix_get(m0, i, j);
			matrix_set(&out, i, j, val);
		}
	}
	for (i = 0; i < m1->nf; i++) {
		for (j = 0; j < m1->nc; j++) {
			void *val = matrix_get(m1, i, j);
			matrix_set(&out, i, j+m0->nc, val);
		}
	}
	for (i = 0; i < m2->nf; i++) {
		for (j = 0; j < m2->nc; j++) {
			void *val = matrix_get(m2, i, j);
			matrix_set(&out, i+m0->nf, j, val);
		}
	}
	for (i = 0; i < m3->nf; i++) {
		for (j = 0; j < m3->nc; j++) {
			void *val = matrix_get(m3, i, j);
			matrix_set(&out, i+m0->nf, j+m0->nc, val);
		}
	}
	return out;
}

matrix multiply_iterativa(matrix * in1, matrix * in2) {
	bool ch = in1->nc==in2->nf;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones de multiplicación");
	matrix out = matrix_of(in1->nf,in2->nc,in1->type);
    int i, j, k;
    for (i = 0; i < in1->nf; i++) {
        for (j = 0; j < in2->nc; j++) {
        	int v = 0;
            matrix_set(&out,i,j,&v);
            for (k = 0; k < in1->nc; k++){
            	int val = (to_int(matrix_get(in1,i,k))*to_int(matrix_get(in2,k,j)))+to_int(matrix_get(&out,i,j));
            	matrix_set(&out,i,j,&val);
            }
        }
    }
    return out;
}


matrix multiply_recursiva(matrix * in1, matrix * in2) {
	bool ch = in1->nc==in2->nf;
	char mem[100];
	sprintf(mem,"No se cumplen condiciones de multiplicación = (in1.nc = %d, in2.nf = %d)",in1->nc,in2->nf);
	check_argument(ch,__FILE__, __LINE__, mem);
	matrix out = matrix_of(in1->nf,in2->nc,in1->type);
	if(in1->nc < 2 || in1->nf < 2 || in2->nf < 2 || in2->nc < 2) {
		out = multiply_iterativa(in1,in2);
	} else {
		matrix_views v1 = views_of_matrix(in1);
		matrix_views v2 = views_of_matrix(in2);
		matrix t1_m0 = multiply_recursiva(&v1.m0,&v2.m0);
		matrix t2_m0 = multiply_recursiva(&v1.m1,&v2.m2);
		matrix t1_m1 = multiply_recursiva(&v1.m0,&v2.m1);
		matrix t2_m1 = multiply_recursiva(&v1.m1,&v2.m3);
		matrix t1_m2 = multiply_recursiva(&v1.m2,&v2.m0);
		matrix t2_m2 = multiply_recursiva(&v1.m3,&v2.m2);
		matrix t1_m3 = multiply_recursiva(&v1.m2,&v2.m1);
		matrix t2_m3 = multiply_recursiva(&v1.m3,&v2.m3);
		matrix t1 = matrix_compose(&t1_m0,&t1_m1,&t1_m2,&t1_m3);
		matrix t2 = matrix_compose(&t2_m0,&t2_m1,&t2_m2,&t2_m3);
		matrix_free(&t1_m0);matrix_free(&t1_m1);matrix_free(&t1_m2);matrix_free(&t1_m3);
		matrix_free(&t2_m0);matrix_free(&t2_m1);matrix_free(&t2_m2);matrix_free(&t2_m3);
		out = sum_iterativa(&t1,&t2);
		matrix_free(&t1);matrix_free(&t2);
	}
	return out;
}

matrix sum_iterativa(matrix * in1, matrix * in2) {
	bool ch = in1->nc==in2->nc && in1->nf==in2->nf;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones de suma");
	matrix out = matrix_of(in1->nf,in1->nc,in1->type);
    int i, j;
    for (i = 0; i < in1->nf; i++) {
        for (j = 0; j < in1->nc; j++) {
        	int val = to_int(matrix_get(in1,i,j))+to_int(matrix_get(in2,i,j));
            matrix_set(&out,i,j,&val);
        }
    }
    return out;
}

matrix sum_recursiva(matrix * in1, matrix * in2) {
	bool ch = in1->nc==in2->nc && in1->nf==in2->nf;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones de suma");
	if(in1->nc > 2 && in1->nf > 2) {
		matrix_views v1 = views_of_matrix(in1);
		matrix_views v2 = views_of_matrix(in2);
		matrix m0 = sum_recursiva(&v1.m0,&v2.m0);
		matrix m1 = sum_recursiva(&v1.m1,&v2.m1);
		matrix m2 = sum_recursiva(&v1.m2,&v2.m2);
		matrix m3 = sum_recursiva(&v1.m3,&v2.m3);
		matrix_free(&m0);matrix_free(&m1);matrix_free(&m2);matrix_free(&m3);
		return matrix_compose(&m0,&m1,&m2,&m3);
	} else {
		return sum_iterativa(in1,in2);
	}
}


void test_matrices_1() {
#define n 5
	int id1[8][8] = { { 1, 0, 0, 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 0,1, 1, 1 }, { 0, 0, 1, 0,0,1, 1, 1 }, { 0, 0, 0, 1,0,1, 1, 1 },
			{0, 0, 0, 0, 1, 1, 1, 1}, { 1, 0, 0, 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 0,1, 1, 1 }, { 0, 0, 1, 0, 0,1, 1, 1 }};
	int id2[8][8] = { { 1, 1, 1, 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 0,1, 1, 1 }, { 0, 0, 1, 0,0,1, 1, 1 }, { 0, 0, 0, 1,0,1, 1, 1 },
				{0, 0, 0, 0, 1, 1, 1, 1}, { 1, 0, 0, 1, 0, 1, 1, 1 }, { 0, 1, 0, 0, 0,1, 1, 1 }, { 0, 0, 1, 0, 0,1, 1, 1 }};

	int mat1[5][5] = { { 1, 1, 1, 1, 1}, { 2, 2, 2, 2, 2}, { 3, 3, 3, 3, 3}, { 4, 4, 4, 4, 4}, { 5,5,5,5,5} };
	int mat2[5][5] = { { 1, 1, 1, 1, 1}, { 2, 2, 2, 2, 2}, { 3, 3, 3, 3, 3}, { 4, 4, 4, 4, 4}, { 5,5,5,5,5} };
	int mat3[2][2] = { { 0, 1 }, { 2, 3}};
	int mat4[2][2] = { { 1, 0 }, { 0, 1}};
	int mat5[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	int mat6[3][3] = { { 1, 0, 0 }, { 0, 1, 0}, {0, 0, 1}};
	int mat7[4][4] = { { 1, 1, 2, 3 }, { 2, 3, 4, 5}, {5, 6, 7, 6}, {5, 6, 7, 6}};
	int mat8[4][4] = { { 1, 0, 0, 1 }, { 0, 1, 0, 3}, {0, 0, 1, 3}, {5, 6, 7, 6}};
	int res[n][n]; // To store result


	matrix s1 = matrix_of_array(mat1, n, n, &int_type);
	matrix s2 = matrix_of_array(mat2, n, n, &int_type);
	matrix r = sum_iterativa(&s1, &s2);
	matrix_print(&r, "Result matrix sum iterativa is");
	matrix_views v1 = views_of_matrix(&s1);
	matrix_views v2 = views_of_matrix(&s2);
	r = sum_recursiva(&s1, &s2);
	matrix_print(&r, "Result matrix sum recursiva is");
	r = multiply_iterativa(&s1, &s2);
	matrix_print(&r, "Result matrix multiply iterativa is");
	r = multiply_recursiva(&s1, &s2);
	matrix_print(&r, "Result matrix multiply recursiva is");
}

void test_matrices_3(){
	char mem[250];
	char sep[] = " ,";
	matrix r = matrix_of_file("ficheros/datos_entrada.txt",&int_type,49,5);
	matrix_print(&r, "1____");
	list cn = matrix_corners(&r);
	char * cs = list_tostring(&cn,mem);
	printf("2___ %s\n",cs);
	matrix m = matrix_view(&r,1);
	matrix_print(&m, "3___");
	matrix_views v = views_of_matrix(&r);
	matrix_print(&v.m0, "4___");
	matrix_print(&v.m1, "4___");
	matrix_print(&v.m2, "4___");
	matrix_print(&v.m3, "4___");
	matrix s = matrix_compose(&v.m0,&v.m1,&v.m2,&v.m3);
	matrix_print(&s, "5___");
}

void test_matrices_4(){
	int mat1[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	matrix r = matrix_of_array(mat1,3,3,&int_type);
	matrix_print(&r,"1___");
	int value =0;
	matrix_set(&r,1,2,&value);
	matrix_print(&r,"2___");
	int mat2[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	matrix r2 = matrix_of_array(mat2,3,3,&int_type);
	matrix_print(&r2,"3___");
	matrix v = matrix_view(&r2,3);
	printf("%d,%d",matrix_nf(&v),matrix_nc(&v));
	matrix_print(&v,"4___");
	matrix r1 = matrix_copy(&r2);
	matrix_print(&r1,"5___");
}

void test_matrices_5(){
	int mat1[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	matrix r = matrix_of_array(mat1,3,3,&int_type);
	matrix_print(&r,"1___");
	matrix s = matrix_submatrix(&r,1,1,2,2);
	matrix_print(&s,"2___");

}
