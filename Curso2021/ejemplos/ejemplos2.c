

/*
 * Ejercicio62.c
 *
 *  Created on: 2 nov. 2018
 *      Author: Miguel Toro
 */

#include "ejemplos2.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

heap hp;

void * to_data_time(time_t * t){
	return heap_copy(&hp,t,sizeof(time_t));
}

void test_ejercicio2() {
	hp = heap_empty();
	time_t a = time_of_date(28, 8, 2017);
	time_t b = time_of_date(2, 11, 2018);
	to_file_while("fechas.txt", a, b);
	to_file_rec("fechas.txt", a, b);
	heap_free(&hp);
}

list lee_iterativo(char * file, time_t a, time_t b) {
	iterator f = iterable_file_string_fix(file);
	list lista = list_empty(&date_type);
	while (iterable_has_next(&f)) {
		char * linea = iterable_next(&f);
		time_t fecha = time_parse_date_s(linea);
		if (time_naturalorder(&fecha, &a) > 0 && time_naturalorder(&b, &fecha) > 0) {
			list_add(&lista, &fecha);
		}
	}
	return lista;
}

void rec(iterator * f, list * lis, time_t a, time_t b) {
	if (!iterable_has_next(f)) {
	} else {
		char * linea = iterable_next(f);
		time_t fecha = time_parse_date_s(linea);
		if (time_naturalorder(&fecha, &a) > 0 && time_naturalorder(&b, &fecha) > 0) {
			list_add_pointer(lis, to_data_time(&fecha));
		}
		rec(f,lis,a,b);
	}
}

list lee_recursivo(char * file, time_t a, time_t b) {
	iterator f = iterable_file_string_fix(file);
	list lista = list_empty(&date_type);
	rec(&f,&lista,a,b);
	return lista;
}


void to_file_while(char * file, time_t a, time_t b) {
	list lista = lee_iterativo(file,a,b) ;
	list_quick_sort(&lista, time_naturalorder);
	write_list_to_file("fechas_ordenadas_while.txt",&lista,time_tostring);
}

void to_file_rec(char * file, time_t a, time_t b) {
	list lista = lee_recursivo(file,a,b) ;
	list_quick_sort(&lista, time_naturalorder);
	write_list_to_file("fechas_ordenadas_recursivo.txt",&lista,time_tostring);
}

