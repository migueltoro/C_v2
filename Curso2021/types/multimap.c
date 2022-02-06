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
	map ht = map_empty(key_type,type_copy(&t,NULL));
	list_multimap lm = {ht,value_type};
	return lm;
}

int list_multimap_size(list_multimap * lm){
	return map_size(&(lm->hash_table));
}

void * list_multimap_put(list_multimap * lm, void * key, void * value){
	if(!list_multimap_contains_key(lm,key)){
		list vl = list_empty(lm->value_type);
		map_put(&lm->hash_table,key,&vl);
	}
	list * values = map_get(&lm->hash_table,key);
	list_add(values,value);
	return lm;
}

list_multimap * list_multimap_put_all(list_multimap * lm, iterator * items) {
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		list_multimap_put(&lm->hash_table, p->key, p->value);
	}
	return lm;
}

list_multimap list_multimap_copy(list_multimap * lm){
	map c = map_copy(&(lm->hash_table));
	list_multimap r = {c,lm->value_type};
	return r;
}

list_multimap list_multimap_add(list_multimap * lm1, list_multimap * lm2) {
	iterator items = map_items_iterable(&(lm2->hash_table));
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
	return (list *) map_get(&(lm->hash_table),key);
}

bool list_multimap_contains_key(list_multimap * lm, void * key){
	return map_contains_key(&(lm->hash_table),key);
}

char * list_multimap_tostring(list_multimap * lm, char * mem){
	map_tostring(&lm->hash_table,mem);
	return mem;
}

void list_multimap_toconsole(list_multimap * lm){
	iterator it = map_items_iterable(&(lm->hash_table));
	iterable_to_console_sep(&it,"\n","","\n");
}

iterator list_multimap_iterable(list_multimap * lm){
	return map_items_iterable(&(lm->hash_table));
}

void list_multimap_free(list_multimap * lm){
	if(lm != NULL){
		map_free(&lm->hash_table);
	}
}

pair * pf(pair * out, long * in){
	int inn =  (int) *in;
	int r = inn % 5;
	pair p = pair_of(&r,in);
	*out = p;
	return out;
}

void test_list_multimap() {
	char mem[1000];
	iterator it1 = iterable_range_long(3, 100, 7);
	list_multimap lm1 = list_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it1)) {
		long n = *(long*) iterable_next(&it1);
		int key = (int) n % 5;
		list_multimap_put(&lm1, &key, &n);
	}
	list_multimap_tostring(&lm1, mem);
	printf("%s\n", mem);
	iterator it2 = iterable_range_long(5, 200, 7);
	list_multimap lm2 = list_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it2)) {
		long n = *(long*) iterable_next(&it2);
		int key = (int) n % 5;
		list_multimap_put(&lm2, &key, &n);
	}
	list_multimap_tostring(&lm2, mem);
	printf("%s\n", mem);
	list_multimap lm3 = list_multimap_add(&lm1, &lm2);
	list_multimap_tostring(&lm3, mem);
	printf("%s\n", mem);
}



// set_multimap

set_multimap set_multimap_empty(type * key_type, type * value_type){
	type  t = generic_type_1(&set_type, value_type);
	map ht = map_empty(key_type,type_copy(&t,NULL));
    set_multimap sm = {ht,value_type};
	return sm;
}

int set_multimap_size(set_multimap * sm){
	return map_size(&(sm->hash_table));
}

void * set_multimap_put(set_multimap * sm, void * key, void * value){
	if(!set_multimap_contains_key(sm,key)){
		set vl = set_empty(sm->value_type);
		map_put(&sm->hash_table,key,&vl);
	}
	set * values = (set *) map_get(&sm->hash_table,key);
	set_add(values,value);
	return sm;
}

set_multimap * set_multimap_put_all(set_multimap * lm, iterator * items) {
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		set_multimap_put(&(lm->hash_table), p->key, p->value);
	}
	return lm;
}

set_multimap set_multimap_copy(set_multimap * lm){
	map c = map_copy(&(lm->hash_table));
	set_multimap r = {c,lm->value_type};
	return r;
}

set_multimap set_multimap_add(set_multimap * lm1, set_multimap * lm2) {
	iterator items = map_items_iterable(&(lm2->hash_table));
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
	return (set *) map_get(&lm->hash_table,key);
}

bool set_multimap_contains_key(set_multimap * lm, void * key){
	return map_contains_key(&(lm->hash_table),key);
}

char * set_multimap_tostring(set_multimap * lm, char * mem){
	map_tostring(&(lm->hash_table),mem);
	return mem;
}

iterator set_multimap_iterable(set_multimap * sm){
	return map_items_iterable(&(sm->hash_table));
}

void set_multimap_free(set_multimap * lm){
	if(lm != NULL){
		map_free(&lm->hash_table);
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
	set_multimap_tostring(&lm1, mem);
	printf("%s\n", mem);
	iterator it2 = iterable_range_long(3, 200, 7);
	set_multimap lm2 = set_multimap_empty(&int_type, &long_type);
	while (iterable_has_next(&it2)) {
		long n = *(long*) iterable_next(&it2);
		int key = (int) n % 5;
		set_multimap_put(&lm2, &key, &n);
	}
	set_multimap_tostring(&lm2, mem);
	printf("%s\n", mem);
	set_multimap lm3 = set_multimap_add(&lm1, &lm2);
	set_multimap_tostring(&lm3, mem);
	printf("%s\n", mem);
}
