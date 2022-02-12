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
	return t;
}

multiset multiset_of(list * ls){
	multiset st = multiset_empty(ls->type);
	for(int i =0;i <ls->size;i++){
		multiset_add(&st,list_get(ls,i));
	}
	return st;
}

void multiset_add_n(multiset * st, void * element, int n) {
	void * e = map_get(st, element);
	int count;
	if(e != NULL) {
		 count = *(int *)  e;
		 count = count + n;
	} else {
		count = n;
	}
	map_put(st, element,&count);
}

void multiset_add(multiset * st, void * element){
	multiset_add_n(st,element,1);
}

int multiset_size(multiset * st){
	return map_size(st);
}

bool multiset_contains(multiset * st, void * element){
	return map_contains_key(st,element);
}

void multiset_remove_n(multiset * st, void * element, int nu) {
	if (map_contains_key(st, element)) {
		void * n = map_get(st, element);
		int m = MAX(*(int*) n -nu, 0);
		if(m>0) map_put(st, element, &m);
		else map_remove(st, element);
	}
}

int multiset_count(multiset * st, void * element){
	int r =0;
	if (map_contains_key(st, element)) {
		void * n = map_get(st, element);
	    r = *(int*) n;
	}
	return r;
}

iterator multiset_iterable(multiset * st){
	return map_items_iterable(st);
}

char * multiset_tostring(multiset * st, char * mem){
	return map_tostring(st,mem);
}

void multiset_free(multiset * st){
	map_free(st);
}

multiset complete_multiset() {
	int tam = 20;
	multiset st = multiset_empty(&int_type);
	for (int i = 0; i < tam; i++) {
		int a = entero_aleatorio(0, 10);
		multiset_add(&st,&a);
	}
	return st;
}

void test_multiset() {
	char mem[1000];
	new_rand();
	multiset st = complete_multiset();
	printf("%d\n", multiset_size(&st));
	char * s = multiset_tostring(&st, mem);
	printf("%s\n",s);
	iterator it = multiset_iterable(&st);
	iterable_to_console_sep(&it,"\n","","");
	multiset_free(&st);
}
