/*
 * palabras.c
 *
 *  Created on: 22 jul. 2019
 *      Author: migueltoro
 */

#include "palabras.h"

int numero_de_palabras(char * file) {
	iterator git1 = file_iterable_pchar(file);
	iterator git2 = iterable_filter(&git1, pchar_not_all_space);
	iterator gmap = iterable_flatmap(&git2,&array_char_type,text_to_iterable_pchar_function);
	int n = iterable_size(&gmap);
	iterables_free(3,&git1,&git2,&gmap);
	return n;
}

int numero_de_palabras_distintas(char * file) {
	iterator git1 = file_iterable_pchar(file);
	iterator git2 = iterable_filter(&git1, pchar_not_all_space);
	iterator gmap = iterable_flatmap(&git2,&array_char_type,text_to_iterable_pchar_function);
	int n = iterable_size(&gmap);
	printf("%d\n", n);
	iterables_free(3,&git1,&git2,&gmap);
	return n;
}

char * identity(char * out, char * in){
	strcpy(out,in);
	return in;
}

multiset frecuencias_de_palabras(char * file) {
	iterator git1 = file_iterable_pchar(file);
	iterator git2 = iterable_filter(&git1, pchar_not_all_space);
	iterator gmap = iterable_flatmap(&git2,&array_char_type,text_to_iterable_pchar_function);
	multiset r = iterable_counting(&gmap,identity,&array_char_type);
	iterables_free(3,&git1,&git2,&gmap);
	return r;
}


void test_palabras() {
	int n = numero_de_palabras("ficheros/quijote.txt");
	printf("%d\n", n);
	multiset fq = frecuencias_de_palabras("ficheros/quijote.txt");
	iterator ifq = multiset_items_iterable(&fq);
	iterable_toconsole_sep(&ifq,"\n","","");
}


