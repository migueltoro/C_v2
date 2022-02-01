/*
 * palabras.c
 *
 *  Created on: 22 jul. 2019
 *      Author: migueltoro
 */

#include "palabras.h"

int numero_de_palabras(char * file) {
	iterator git1 = file_iterable_string_fix(file);
	iterator git2 = iterable_filter(&git1, string_fix_not_all_space);
	iterator gmap = iterable_flatmap(&git2,&string_fix_type,text_to_iterable_string_fix_function);
	int n = iterable_size(&gmap);
	iterables_free(3,&git1,&git2,&gmap);
	return n;
}

int numero_de_palabras_distintas(char * file) {
	iterator git1 = file_iterable_string_fix(file);
	iterator git2 = iterable_filter(&git1, string_fix_not_all_space);
	iterator gmap = iterable_flatmap(&git2,&string_fix_type,text_to_iterable_string_fix_function);
	set st = iterable_to_set(&gmap);
	int n = set_size(&st);
	printf("%d\n", n);
	iterables_free(3,&git1,&git2,&gmap);
	set_free(&st);
	return n;
}

char * identity(char * out, char * in){
	strcpy(out,in);
	return in;
}

multiset frecuencias_de_palabras(char * file) {
	type t = string_fix_type_of_tam(20);
	iterator git1 = file_iterable_string_fix_tam(file,100);
	iterator git2 = iterable_filter(&git1, string_fix_not_all_space);
	iterator gmap = iterable_flatmap(&git2,type_copy(&t),text_to_iterable_string_fix_function);
	multiset r = iterable_to_multiset(&gmap);
	iterables_free(3,&git1,&git2,&gmap);
	return r;
}

set palabras_distintas(char * file) {
	type t = string_fix_type_of_tam(20);
	iterator git1 = file_iterable_string_fix_tam(file,100);
	iterator git2 = iterable_filter(&git1, string_fix_not_all_space);
	iterator gmap = iterable_flatmap(&git2,type_copy(&t),text_to_iterable_string_fix_function);
	set r = iterable_to_set(&gmap);
	iterables_free(3,&git1,&git2,&gmap);
	return r;
}


void test_palabras() {
	int n = numero_de_palabras("ficheros/quijote.txt");
	printf("%d\n", n);
	multiset fq = frecuencias_de_palabras("ficheros/quijote.txt");
	iterator ifq = multiset_items_iterable(&fq);
	iterable_to_console_sep(&ifq,"\n","","");
}

void test_palabras_2() {
	int n = numero_de_palabras("ficheros/quijote.txt");
	printf("%d\n", n);
	set fq = palabras_distintas("ficheros/quijote.txt");
	iterator ifq = set_iterable(&fq);
	iterable_to_console_sep(&ifq,"\n","","");
}


