/*
 * ejemplos.c
 *
 *  Created on: 19 nov. 2018
 *      Author: migueltoro
 */

#include "ejemplos0.h"

heap hp;

double n_problema1(list ls){
	double sum = 0.;
	for(int i=0;i<list_size(&ls);i++){
		sum = sum + *(double*) list_get(&ls,i);
	}
	return sum;
}

list n_problema2(list ls) {
	list r = list_empty(&double_type);
	for (int i = 0; i <list_size(&ls); i++) {
		double x = (*(pair_double*)list_get(&ls,i)).x;
		list_add(&r,&x);
	}
	return r;
}

bool n_problema3(list ls, bool (*p)(int e)) {
	bool b = true;
	int i = 0;
	while (i < list_size(&ls) && b) {
		b = p(*(int*) list_get(&ls,i));
		i++;
	}
	return b;
}

bool n_problema4(list ls, bool (*p)(int e)) {
	bool b = false;
	int i = 0;
	while (i < list_size(&ls) && !b) {
		b = p(*(int*) list_get(&ls,i));
		i++;
	}
	return b;
}

int n_problema5(list ls) {
	int sum = 0;
	for (int i = 0; i < list_size(&ls); i++) {
		int e = *(int*) list_get(&ls,i);
		int t = e * e;
		sum = sum + t;
	}
	return sum;
}

double n_problema6(list ls, double umbral) {
	double b;
	int p = 0;
	int i = 0;
	while (i <list_size(&ls) && p == 0) {
		double e = *(double *) list_get(&ls,i);
		if (e > umbral) {
			p++;
			b = e;
		}
		i++;
	}
	return b;
}

pair_double * n_problema8(list ls, int (*cmp)(const void *, const void *)) {
	pair_double * r = NULL;
	for (int i = 0; i <list_size(&ls); i++) {
		pair_double * p = (pair_double *) list_get(&ls,i);
		if (r == NULL || cmp(p, r) > 0)
			r = p;
	}
	return r;
}

map n_problema11(list ls){
	map r = map_empty(&int_type,&list_type);
	for (int i = 0; i <list_size(&ls); i++) {
		pair_double * p = (pair_double *) list_get(&ls,i);
		Cuadrante c = punto_cuadrante(p);
		if(map_contains_key(&r,&c)){
			list * ls = map_get(&r,&c);
			list_add(ls,p);
		} else {
			list ls = list_empty(&pair_double_type);
			list_add(&ls,p);
			map_put(&r,&c,&ls);
		}
	}
	return r;
}

list n_problema61(char * file){
	list r = list_empty(&pair_double_type);
	iterator f = iterable_file_string_fix(file);
	while(iterable_has_next(&f)){
		char * s = (char *) iterable_next(&f);
		pair_double e = pair_double_parse_s(s);
		list_add(&r,&e);
	}
	return r;
}

void n_problema59(char * file, long n) {
	long i = 2;
	FILE * st = fopen(file, "wt");
	assert(st != NULL && "no se encuentra el fichero");
	while (i < n) {
		fprintf(st, "%ld\n", i);
		i = siguiente_primo(i);
	}
	fclose(st);
}

int nn = 200;

bool menor_que(long * in){
	return *in < nn;
}


void n_problema59_b(char * file, long n){
	long ini = 2;
	iterator st = iterable_iterate(&long_type,&ini,menor_que,siguiente_primo_f);
	write_iterable_to_file(file,&st);
}

void test_ejemplos_1(){
//	hp = memory_heap_create();

	printf("\n\n");

	double d1[] = { 45., 56, 90., 1., 23., 78., 76. };
	list ls1 = list_of(d1, 7, &double_type);
	double r1 = n_problema1(ls1);
	printf("r1 = %lf\n", r1);

	printf("\n\n");

	char mem[256];
	char * s;
	pair_double ap[] = { { 1., 2. }, { -1., 2.1 }, { 3.1, 2.1 }, { 1.3, 27.0 }, { 1.,-2. } };
	list r2 = list_of(ap, 5, &pair_double_type);
	list rp = n_problema2(r2);
	s = list_tostring(&r2,mem);
	printf("%s\n",s);
	s = list_tostring(&rp,mem);
	printf("%s\n",s);

	printf("\n\n");

	int d3[] = {45, 57, 10, 1, 23, 77, 79};
	list ls3 = list_of(d3, 7,&int_type);
	bool r3 = n_problema3(ls3, es_impar);
	printf("r3 = %s\n", r3 ? "true" : "false");

	printf("\n\n");

	bool r4 = n_problema4(ls3, es_impar);
	printf("r4 = %s\n", r4 ? "true" : "false");

	printf("\n\n");

	int r5 = n_problema5(ls3);
	printf("r5 = %d\n", r5);

	printf("\n\n");

	double r6 = n_problema6(ls1, 60.);
	printf("r6 = %lf\n", r6);

	printf("\n\n");

	pair_double * p = n_problema8(r2, pair_double_naturalorder);
	printf("%s\n",tostring(p,mem,&pair_double_type));

	printf("\n\n");

	list puntos = n_problema61("ficheros/prueba.txt");
	s = list_tostring(&puntos,mem);
	printf("%s\n",s);

	n_problema59("primos.txt", 200);
	n_problema59_b("primos_2.txt", 200);

	printf("\n\n");

	map t2 = n_problema11(r2);
	list * r;
	for(int i=0;i<4;i++){
		r = map_get(&t2,&i);
		if (r!=NULL) {
			s = list_tostring(r,mem);
			printf("%d == %s\n",i, s);
		}
	}

//	memory_heap_free(&hp);
}

void test_ejemplos1(){
		pair_double p = {-2.1,-3.2};
		Cuadrante c = punto_cuadrante(&p);
		int ci = c;
		printf("%d",ci);
}
