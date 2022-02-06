/*
 * multiset.c
 *
 *  Created on: 25 jul. 2019
 *      Author: migueltoro
 */

#include "../types/multiset.h"
#include "../types/accumulators.h"

multiset multiset_empty(type * type_element){
	map t = map_empty(type_element,&int_type);
	multiset st = {t};
	return st;
}

multiset multiset_of(list * ls){
	multiset st = multiset_empty(ls->type);
	for(int i =0;i <ls->size;i++){
		multiset_add(&st,list_get(ls,i));
	}
	return st;
}

void multiset_add_n(multiset * st, void * element, int n) {
	void * e = map_get(&st->hash_table, element);
	int count;
	if(e != NULL) {
		 count = *(int *)  e;
		 count = count + n;
	} else {
		count = n;
	}
	map_put(&st->hash_table, element,&count);
}

void multiset_add(multiset * st, void * element){
	multiset_add_n(st,element,1);
}

int multiset_size(multiset * st){
	return map_size(&(st->hash_table));
}

bool multiset_contains(multiset * st, void * element){
	return map_contains_key(&(st->hash_table),element);
}

void multiset_remove_n(multiset * st, void * element, int nu) {
	if (map_contains_key(&st->hash_table, element)) {
		void * n = map_get(&st->hash_table, element);
		int m = MAX(*(int*) n -nu, 0);
		if(m>0) map_put(&st->hash_table, element, &m);
		else map_remove(&st->hash_table, element);
	}
}

int multiset_count(multiset * st, void * element){
	int r =0;
	if (map_contains_key(&st->hash_table, element)) {
		void * n = map_get(&st->hash_table, element);
	    r = *(int*) n;
	}
	return r;
}

iterator multiset_items_iterable(multiset * st){
	return map_items_iterable(&st->hash_table);
}

char * multiset_tostring(multiset * st, char * mem){
	return map_tostring(&st->hash_table,mem);
}

void multiset_free(multiset * st){
	map_free(&(st->hash_table));
}

multiset complete_multiset() {
	char mem[30];
	int tam = 50;
	type t = string_fix_type_of_tam(10);
	multiset st = multiset_empty(type_copy(&t,NULL));
	for (int i = 0; i < tam; i++) {
		int a2 = entero_aleatorio(0, 10);
		char * b2 = tostring(&a2,mem,&int_type);
		multiset_add(&st,b2);
	}
	return st;
}

multiset complete_multiset_2() {
	char mem[30];
	int tam = 50;
	type t = string_fix_type_of_tam(10);
	list ls = list_empty(type_copy(&t,NULL));
	for (int i = 0; i < tam; i++) {
		int a2 = entero_aleatorio(0, 10);
		char * b2 = tostring(&a2,mem,&int_type);
		list_add(&ls,b2);
	}
	iterator it = list_iterable(&ls);
	multiset st = iterable_to_multiset(&it);
	return st;
}

double * _random(double * out, long * in){
	*out = double_aleatorio(0, 100);
	return out;
}

void test_multiset() {
	char mem[1000];
	new_rand();
	multiset st = complete_multiset();
	printf("%d\n", multiset_size(&st));
	char * s = multiset_tostring(&st, mem);
	printf("%s\n",s);
	iterator it = multiset_items_iterable(&st);
	iterable_to_console_sep(&it,"\n","","");
	multiset_free(&st);
}
