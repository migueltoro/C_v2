/*
 * multimap.c
 *
 *  Created on: 5 ene. 2021
 *      Author: migueltoro
 */

#include "multimap.h"

//list_multimap

list_multimap list_multimap_empty(type * key_type, type * value_type){
	type  t = generic_type_1(&list_type, value_type);
	return map_empty(type_copy(key_type,NULL),type_copy(&t,NULL));
}

int list_multimap_size(list_multimap * lm){
	return map_size(lm);
}

void * list_multimap_put(list_multimap * lm, void * key, void * value){
	type * value_type = lm->value_type->types[0];
	if(!list_multimap_contains_key(lm,key)){
		list vl = list_empty(value_type);
		map_put(lm,key,&vl);
	}
	list * values = map_get(lm,key);
	list_add(values,value);
	return lm;
}

list_multimap * list_multimap_put_all(list_multimap * lm, iterator * items) {
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		list_multimap_put(lm, p->key, p->value);
	}
	return lm;
}

list_multimap list_multimap_copy(list_multimap * lm){
	return map_copy(lm);
}

list_multimap list_multimap_add(list_multimap * lm1, list_multimap * lm2) {
	iterator items = map_items_iterable(lm2);
	list_multimap r = list_multimap_copy(lm1);
	while (iterable_has_next(&items)) {
		pair * p = (pair *) iterable_next(&items);
		list * ls = p->value;
		void * key = p->key;
		int n = list_size(ls);
		for(int i = 0; i< n; i++){
			void * e = list_get(ls,i);
			list_multimap_put(&r,key,e);
		}
	}
	iterable_free(&items);
	return r;
}

list * list_multimap_get(list_multimap * lm, void * key){
	return (list *) map_get(lm,key);
}

bool list_multimap_contains_key(list_multimap * lm, void * key){
	return map_contains_key(lm,key);
}

char * list_multimap_tostring(list_multimap * lm, char * mem){
	char * s = map_tostring(lm,mem);
	return s;
}

void list_multimap_toconsole(list_multimap * lm){
	iterator it = map_items_iterable(lm);
	iterable_to_console_sep(&it,"\n","","\n");
}

iterator list_multimap_iterable(list_multimap * lm){
	return map_items_iterable(lm);
}

void list_multimap_free(list_multimap * lm){
	if(lm != NULL){
		map_free(lm);
	}
}

pair * pf(pair * out, long * in){
	int inn =  (int) *in;
	int r = inn % 5;
	pair p = pair_of(&r,in);
	*out = p;
	return out;
}

list_multimap complete_list_multimap_1() {
	iterator it1 = iterable_range_long(3, 100, 7);
	list_multimap lm1 = list_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it1)) {
		long n = *(long*) iterable_next(&it1);
		int key = (int) n % 5;
		list_multimap_put(&lm1, &key, &n);
	}
	return lm1;
}

list_multimap complete_list_multimap_2() {
	iterator it2 = iterable_range_long(5, 200, 7);
	list_multimap lm2 = list_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it2)) {
		long n = *(long*) iterable_next(&it2);
		int key = (int) n % 5;
		list_multimap_put(&lm2, &key, &n);
	}
	return lm2;
}


void test_list_multimap() {
//	char mem[1000];
	list_multimap lm1 = complete_list_multimap_1();
	iterator r = list_multimap_iterable(&lm1);
	iterable_to_console(&r);
//	printf("%s\n", mem);;
	list_multimap lm2 = complete_list_multimap_2();
	r = list_multimap_iterable(&lm2);
	iterable_to_console(&r);
//	printf("%s\n", mem);
	list_multimap lm3 = list_multimap_add(&lm1, &lm2);
	r = list_multimap_iterable(&lm3);
	iterable_to_console(&r);
//	printf("%s\n", mem);
}



// set_multimap

set_multimap set_multimap_empty(type * key_type, type * value_type){
	type  t = generic_type_1(&set_type, value_type);
	map ht = map_empty(key_type,type_copy(&t,NULL));
	return ht;
}

int set_multimap_size(set_multimap * sm){
	return map_size(sm);
}

void * set_multimap_put(set_multimap * sm, void * key, void * value){
	type * value_type = sm->value_type->types[0];
	if(!set_multimap_contains_key(sm,key)){
		set vl = set_empty(value_type);
		map_put(sm,key,&vl);
	}
	set * values = (set *) map_get(sm,key);
	set_add(values,value);
	return sm;
}

set_multimap * set_multimap_put_all(set_multimap * lm, iterator * items) {
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		set_multimap_put(lm, p->key, p->value);
	}
	return lm;
}

set_multimap set_multimap_copy(set_multimap * lm){
	map c = map_copy(lm);
	return c;
}

set_multimap set_multimap_add(set_multimap * lm1, set_multimap * lm2) {
	iterator items = map_items_iterable(lm2);
	set_multimap r = set_multimap_copy(lm1);
	while (iterable_has_next(&items)) {
		pair * p = (pair *) iterable_next(&items);
		set * ls = p->value;
		void * key = p->key;
		iterator it = set_iterable(ls);
		while(iterable_has_next(&it)){
			void * e = iterable_next(&it);
			set_multimap_put(&r,key,e);
		}
		iterable_free(&it);
	}
	iterable_free(&items);
	return r;
}

set * set_multimap_get(set_multimap * lm, void * key){
	return (set *) map_get(lm,key);
}

bool set_multimap_contains_key(set_multimap * lm, void * key){
	return map_contains_key(lm,key);
}

char * set_multimap_tostring(set_multimap * lm, char * mem){
	char * s = map_tostring(lm,mem);
	return s;
}

iterator set_multimap_iterable(set_multimap * sm){
	return map_items_iterable(sm);
}

void set_multimap_free(set_multimap * lm){
	if(lm != NULL){
		map_free(lm);
	}
}

void test_set_multimap() {
	char mem[1000];
	iterator it1 = iterable_range_long(3, 20, 7);
	set_multimap lm1 = set_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it1)) {
		long n = *(long*) iterable_next(&it1);
		int key = (int) n % 5;
		set_multimap_put(&lm1, &key, &n);
	}
	char * s = set_multimap_tostring(&lm1, mem);
	printf("1 %s\n", s);
	iterator it2 = iterable_range_long(3, 200, 7);
	set_multimap lm2 = set_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it2)) {
		long n = *(long*) iterable_next(&it2);
		int key = (int) n % 5;
		set_multimap_put(&lm2, &key, &n);
	}
	s = set_multimap_tostring(&lm2, mem);
	printf("2 %s\n", s);
	set_multimap lm3 = set_multimap_add(&lm1, &lm2);
	s = set_multimap_tostring(&lm3, mem);
	printf("3 %s\n",s);
}
