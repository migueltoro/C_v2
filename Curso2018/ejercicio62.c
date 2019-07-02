

/*
 * Ejercicio62.c
 *
 *  Created on: 2 nov. 2018
 *      Author: Miguel Toro
 */

#include "ejercicio62.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

memory_heap hp;

void * to_data_time(time_t * t){
	return memory_heap_to_data(&hp,t,sizeof(time_t));
}

void test_ejercicio62() {
	hp = memory_heap_create();
	time_t a = time_create(28, 8, 2017);
	time_t b = time_create(2, 11, 2018);
	tofileWHILE("fechas.txt", a, b);
	tofileRec("fechas.txt", a, b);
	memory_heap_free(&hp);
}

list leeIterativo(char * file, time_t a, time_t b) {
	file_iterator fichero = open_file(file);
	list lista = list_empty();
	while (has_next_line(&fichero)) {
		char * linea = next_line(&fichero);
		time_t fecha = time_parse(linea);
		if (time_naturalorder(&fecha, &a) > 0 && time_naturalorder(&b, &fecha) > 0) {
			list_add_pointer(&lista, to_data_time(&fecha));
		}
	}
	fclose(fichero.file);
	return lista;
}

void rec(file_iterator * f, list * lis, time_t a, time_t b) {
	if (!has_next_line(f)) {
	} else {
		char * linea = next_line(f);
		time_t fecha = time_parse(linea);
		if (time_naturalorder(&fecha, &a) > 0 && time_naturalorder(&b, &fecha) > 0) {
			list_add_pointer(lis, to_data_time(&fecha));
		}
		rec(f,lis,a,b);
	}
}

list leeRecursivo(char * file, time_t a, time_t b) {
	file_iterator fichero = open_file(file);
	list lista = list_empty();
	rec(&fichero,&lista,a,b);
	fclose(fichero.file);
	return lista;
}


void tofileWHILE(char * file, time_t a, time_t b) {
	list lista = leeIterativo(file,a,b) ;
	list_sort(&lista, time_naturalorder);
	write_list_to_file("fechasOrdenadasWhile.txt",&lista,time_tostring);
}

void tofileRec(char * file, time_t a, time_t b) {
	list lista = leeRecursivo(file,a,b) ;
	list_sort(&lista, time_naturalorder);
	write_list_to_file("fechasOrdenadasRecursivo.txt",&lista,time_tostring);
}

