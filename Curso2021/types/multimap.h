/*
 * multimap.h
 *
 *  Created on: 5 ene. 2021
 *      Author: migueltoro
 */

#ifndef MULTIMAP_H_
#define MULTIMAP_H_

#include "../types/types.h"
#include "../types/hash_table.h"
#include "../types/iterables.h"
#include "../types/list.h"
#include "../types/set.h"


typedef struct {
	hash_table hash_table;
	type * value_type;
} list_multimap;


list_multimap list_multimap_empty(type * key_type, type * value_type);
int list_multimap_size(list_multimap * table);
void * list_multimap_put(list_multimap * table, void * key, void * value);
list_multimap * list_multimap_put_all(list_multimap * table, iterator * items);
list_multimap list_multimap_add(list_multimap * t1, list_multimap * t2);
list_multimap list_multimap_of(iterator * items);

void * list_multimap_remove(list_multimap * table, void * key);
list_multimap * list_multimap_remove_all(list_multimap * table, iterator * keys);
list * list_multimap_get(list_multimap * table, void * key);
bool list_multimap_contains_key(list_multimap * table, void * key);

iterator list_multimap_iterable(list_multimap * lm);
char * list_multimap_tostring(list_multimap * lm, char * mem);
void list_multimap_toconsole(list_multimap * lm);

void list_multimap_free(list_multimap * lm);

void test_list_multimap();

typedef struct {
	hash_table hash_table;
	type * value_type;
} set_multimap;

set_multimap set_multimap_empty(type * key_type, type * value_type);
int set_multimap_size(set_multimap * table);
void * set_multimap_put(set_multimap * table, void * key, void * value);
set_multimap * set_multimap_put_all(set_multimap * table, iterator * items);
set_multimap set_multimap_add(set_multimap * t1, set_multimap * t2);

void * set_multimap_remove(set_multimap * table, void * key);
set_multimap * set_multimap_remove_all(set_multimap * table, iterator * keys);
set * set_multimap_get(set_multimap * table, void * key);
bool set_multimap_contains_key(set_multimap * table, void * key);

char * set_multimap_tostring(set_multimap * lm, char * mem);
iterator set_multimap_iterable(set_multimap * lm);

void set_multimap_free(set_multimap * lm);

void test_set_multimap();


#endif /* MULTIMAP_H_ */
