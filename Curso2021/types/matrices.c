/*
 * matrices.c
 *
 *  Created on: 13 dic. 2019
 *      Author: migueltoro
 */

#include "matrices.h"

matrix matrix_of_array(void * a, int nf, int nc, type * type_element){
	type * t = type_element;
	matrix r = {t,a,nc,0,nf,0,nc};
	return r;
}

matrix matrix_of_file(char * file, void * array, type * type) {
	list ls = list_of_list_of_file_type(file, type);
	int nf = list_size(&ls);
	int nc = list_size(list_get(&ls, 0));
	list_of_list_to_2_array(&ls, array);
	return matrix_of_array(array, nf, nc, type);
}

int matrix_nf(matrix s){
	return s.nf;
}
int matrix_nc(matrix s){
	return s.nc;
}

void * matrix_get(matrix s, int i, int j){
	return s.datos + s.type_element->size*(s.__nc*(s.iv+i)+ s.jv+ j);
}

int matrix_get_int(matrix s, int i, int j) {
	return to_int(matrix_get(s,i,j));
}

void matrix_set(matrix s, int i, int j, void * value){
	copy(s.datos + s.type_element->size*(s.__nc*(s.iv+i)+ s.jv+ j), value, s.type_element->size);
}

void matrix_set_value(matrix s, void * value) {
	int i, j;
	for (i = 0; i < s.nf; i++) {
		for (j = 0; j < s.nc; j++)
			matrix_set(s,i,j,value);
	}
}

void * matrix_center(matrix s){
	int nf = matrix_nf(s)/2;
	int nc = matrix_nc(s)/2;
	return matrix_get(s,nf,nc);
}

list matrix_corners(matrix s){
	list r = list_empty(s.type_element);
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

matrix matrix_view(matrix s, int nv){
	int nf = s.nf/2;
	int nc = s.nc/2;
	matrix r;
	r.datos= s.datos;
	r.__nc = s.__nc;
	r.type_element = s.type_element;
	switch(nv){
	case 0:  r.iv = s.iv; r.nf = nf; r.jv = s.jv; r.nc = nc; break;
	case 1:  r.iv = s.iv; r.nf = nf; r.jv = s.jv + nc; r.nc = s.nc-nc;  break;
	case 2:  r.iv = s.iv + nf; r.nf = s.nf-nf; r.jv = s.jv; r.nc = nc; break;
	case 3:  r.iv = s.iv + nf; r.nf = s.nf-nf; r.jv = s.jv + nc; r.nc = s.nc-nc;  break;
	default: check_argument(false,__FILE__, __LINE__, "Opción no válida");
	}
	return r;
}

matrix_views views_of_matrix(matrix s){
	matrix_views r;
	r.m0 = matrix_view(s,0);
	r.m1 = matrix_view(s,1);
	r.m2 = matrix_view(s,2);
	r.m3 = matrix_view(s,3);
	return r;
}

void matrix_free(matrix m){
	free(m.type_element);
}

void matrix_print(matrix s, char * name) {
	printf("\n%s = (n = %d, iv = %d, nf = %d, jv = %d, nc = %d)\n",name,s.__nc,s.iv,s.nf,s.jv,s.nc);
	char mem[Tam_String];
	int i, j;
	for (i = 0; i < s.nf; i++) {
		for (j = 0; j < s.nc; j++)
			printf("%4s ", tostring(matrix_get(s,i,j),mem,s.type_element));
		printf("\n");
	}
}

void matrix_copy(matrix out, matrix in) {
	int i, j;
	for (i = 0; i < in.nf; i++) {
		for (j = 0; j < in.nc; j++){
			void * val = matrix_get(in,i,j);
			matrix_set(out,i,j,val);
		}
	}
}

void multiply_iterativa(matrix out, matrix in1, matrix in2) {
	bool ch = in1.nc==in2.nf && out.nf == in1.nf && out.nc == in2.nc;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones de multiplicación");
    int i, j, k;
    for (i = 0; i < in1.nf; i++) {
        for (j = 0; j < in2.nc; j++) {
        	int v = 0;
            matrix_set(out,i,j,&v);
            for (k = 0; k < in1.nc; k++){
            	int val = (to_int(matrix_get(in1,i,k))*to_int(matrix_get(in2,k,j)))+to_int(matrix_get(out,i,j));
            	matrix_set(out,i,j,&val);
            }
        }
    }
}


void multiply_recursiva(matrix out, matrix in1, matrix in2) {
	bool ch = in1.nc==in2.nf;
	char mem[100];
	sprintf(mem,"No se cumplen condiciones de multiplicación = (in1.nc = %d, in2.nf = %d)",in1.nc,in2.nf);
	check_argument(ch,__FILE__, __LINE__, mem);
	if(in1.nc < 2 || in1.nf < 2 || in2.nf < 2 || in2.nc < 2) {
		multiply_iterativa(out,in1,in2);
	} else {
		int tmp1[out.nf*out.nc];
		matrix tt1 = matrix_of_array(tmp1,out.nf,out.nc,&int_type);
		int tmp2[out.nf*out.nc];
		matrix tt2 = matrix_of_array(tmp2,out.nf,out.nc,&int_type);
		matrix_views v1 = views_of_matrix(in1);
		matrix_views v2 = views_of_matrix(in2);
		matrix_views t1 = views_of_matrix(tt1);
		matrix_views t2 = views_of_matrix(tt2);
		multiply_recursiva(t1.m0,v1.m0,v2.m0);
		multiply_recursiva(t2.m0,v1.m1,v2.m2);
		multiply_recursiva(t1.m1,v1.m0,v2.m1);
		multiply_recursiva(t2.m1,v1.m1,v2.m3);
		multiply_recursiva(t1.m2,v1.m2,v2.m0);
		multiply_recursiva(t2.m2,v1.m3,v2.m2);
		multiply_recursiva(t1.m3,v1.m2,v2.m1);
		multiply_recursiva(t2.m3,v1.m3,v2.m3);
		sum_iterativa(out,tt1,tt2);
	}
}

void sum_iterativa(matrix out, matrix in1, matrix in2) {
	bool ch = in1.nc==in2.nc && in1.nf==in2.nf && out.nc == in1.nc && out.nf == in1.nf;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones de suma");
    int i, j;
    for (i = 0; i < in1.nf; i++) {
        for (j = 0; j < in1.nc; j++) {
        	int val = to_int(matrix_get(in1,i,j))+to_int(matrix_get(in2,i,j));
            matrix_set(out,i,j,&val);
        }
    }
}


void sum_recursiva(matrix out, matrix in1, matrix in2) {
	bool ch = in1.nc==in2.nc && in1.nf==in2.nf && out.nc == in1.nc && out.nf == in1.nf;
	check_argument(ch,__FILE__, __LINE__, "No se cumplen condiciones desuma");
	if(in1.nc > 1 && in1.nf > 1) {
		matrix_views v1 = views_of_matrix(in1);
		matrix_views v2 = views_of_matrix(in2);
		matrix_views r = views_of_matrix(out);
		sum_recursiva(r.m0,v1.m0,v2.m0);
		sum_recursiva(r.m1,v1.m1,v2.m1);
		sum_recursiva(r.m2,v1.m2,v2.m2);
		sum_recursiva(r.m3,v1.m3,v2.m3);
	} else {
		sum_iterativa(out,in1,in2);
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


	matrix s1 = matrix_of_array(mat1,n,n, &int_type);
	matrix s2 = matrix_of_array(mat2,n,n, &int_type);
	matrix r = matrix_of_array(res,n,n, &int_type);
	multiply_iterativa(r,s1,s2);
	matrix_print(r, "Result matrix multiply iterativa is");
	multiply_recursiva(r,s1,s2);
	matrix_print(r, "Result matrix multiply recursiva is");
	printf("%d,%d",matrix_nf(r),matrix_nc(r));
}

void test_matrices_2() {
	// el array tiene que estar dimensionado para que quepa la matrix
	int array[49][5];
	matrix r = matrix_of_file("ficheros/datos_entrada.txt",array,&int_type);
	matrix_print(r, "Result matrix multiply iterativa is");
	matrix_print(matrix_view(r,1), "View");
}

void test_matrices_3(){
	char mem[250];
	list ls = list_of_list_of_file_type("ficheros/datos_entrada.txt",&int_type);
    int nf = list_size(&ls);
	int nc = list_size(list_get(&ls, 0));
	int array[nf][nc];
	list_of_list_to_2_array(&ls, array);
	matrix r = matrix_of_array(array, nf, nc, &int_type);
	matrix_print(r, "Result matrix multiply iterativa is");
	printf("%d,%d,%d,%d",to_int(matrix_get(r,0,0)),to_int(matrix_get(r,0,nc-1)),to_int(matrix_get(r,nf-1,0)),
			to_int(matrix_get(r,nf-1,nc-1)));
	list cn = matrix_corners(r);
	list_tostring(&cn,mem);
	printf("\n%s\n",mem);
	matrix_print(matrix_view(r,1), "View");

}

void test_matrices_4(){
	int mat1[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	matrix r = matrix_of_array(mat1,3,3,&int_type);
	matrix_print(r,"Prueba");
	int value =0;
	matrix_set(r,0,0,&value);
	matrix_print(r,"Prueba_2");
	int mat2[3][3] = { { 1, 1, 2 }, { 2, 3, 4}, {5, 6, 7}};
	matrix r2 = matrix_of_array(mat2,3,3,&int_type);
	matrix_copy(r2,r);
	matrix_print(r2,"View");
	matrix v = matrix_view(r2,3);
	printf("%d,%d",matrix_nf(v),matrix_nc(v));
	matrix_print(v,"View_2");
}
