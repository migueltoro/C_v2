/*
 * hash_table.h
 *
 *  Created on: 4 nov. 2018
 *      Author: migueltoro
 */

#ifndef MAP_H_
#define MAP_H_


#include "../types/iterables.h"


typedef struct {
	void * key;
	void * value;
	int next;
} entry;

typedef struct {
	type * key_type;
	type * value_type;
    int * blocks;
	entry * data;
	int size;
	int capacity_blocks;
	int capacity_data;
	int first_free_data;
	float load_factor_limit;
	string_var st;
} map;

map map_empty(type * key_type, type * value_type);
int map_size(map * table);
void * map_put_pointer(map * table, void * key, void * value);
void * map_put(map * table, void * key, void * value);
map * map_put_all(map * table, iterator * items);
map map_copy(map * table);

void * map_remove(map * table, void * key);
map * map_remove_all(map * table, iterator * keys);
void * map_get(map * table, void * key);
bool map_contains_key(map * table, void * key);


char * map_tostring(map * table, char * mem);
iterator map_items_iterable(map * ht);
iterator map_keys_iterable(map * ht);


void map_toconsole(map * table);

void map_free(map * table);

map complete_map();
void test_map();
void test_map_2();
#endif /* MAP_H_ */
