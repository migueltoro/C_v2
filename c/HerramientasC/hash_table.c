/*
 * hash_table.c
 *
 *  Created on: 7 ago. 2018
 *      Author: migueltoro
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hash_table.h"

memory_heap hash_table_memory = {0,0,NULL};

memory_heap * get_hash_table_memory(){
	if(hash_table_memory.elements == NULL) {
		hash_table_memory = memory_heap_create();
	}
	return &hash_table_memory;
}

void * get_mem_hash_table(int size){
	return memory_heap_tam_memory(get_hash_table_memory(),size);
}

void * get_value_hash_table(int size, void * value){
	return memory_heap_memory_for_value(get_hash_table_memory(),size, value);
}

type hash_table_type = {2,NULL,sizeof(hash_table),NULL,NULL,NULL,NULL,NULL,NULL,"hash_table_type"};

int _primes[] = {13, 23, 41, 61, 83, 151, 199, 263, 383, 503, 641, 769, 911, 1049, 1559};
int _next_prime = 0;
int _nprimes = 15;

parameters_hash_table initial_parameters = {13,0.75,(int)(0.75*13 +1), 0, 0};

int get_index_block(hash_table * table, void * key);
entry_data * find_entry_data(hash_table * table, int index_block, void * key);
entry_data * get_entry_data(hash_table * table, int index);
void * hash_table_put_private(hash_table * table, void * key, void * value);
void ini_data(hash_table * table);
int rehash(hash_table * table);

tuple2 * hash_table_get_entry_from_index(hash_table * table, int index_data);

void ini_data(hash_table * table) {
	int capacity = table->parameters.capacity;
	int capacity_data = table->parameters.capacity_data;
	table->blocks = (int *) get_mem_hash_table(capacity * sizeof(int));
	table->data = (entry_data *) get_mem_hash_table(capacity_data * sizeof(entry_data));
	for (int i = 0; i < capacity; i++) {
		table->blocks[i] = -1;
	}
	for (int i = 0; i < capacity_data; i++) {
		get_entry_data(table,i)->next = i+1;
	}
	get_entry_data(table,capacity_data -1)->next = -1;
}

hash_table hash_table_create(type key, type value) {
    hash_table table;
    table.key_type = key;
    table.value_type = value;
    table.parameters = initial_parameters;
    ini_data(&table);
    return table;
}


int rehash(hash_table * table) {
	if (table->parameters.size < table->parameters.capacity_data) return 0;
	_next_prime = _next_prime + 1;
	assert(_next_prime < _nprimes && "se ha acabado los n�meros primos disponibles");
	parameters_hash_table old_parameters = table->parameters;
	entry_data * old_data = table->data;
	int * old_blocks = table->blocks;
	int capacity = _primes[_next_prime];
	parameters_hash_table new_initial_parameters = {capacity, 0.75,(int) (0.75 *capacity + 1), 0, 0 };
	table->parameters = new_initial_parameters;
	ini_data(table);
	for (int i = 0; i < old_parameters.capacity; i++) {
		assert(i < old_parameters.capacity && "el �ndice no est� en rango de los bloques disponibles");
		int first = old_blocks[i];
		if (first < 0) continue;
		int j = first;
		while (j >= 0) {
			assert(j < old_parameters.capacity_data && "el �ndice no est� en el rango del array de datos");
			entry_data * entry = old_data+j;
			void * key = entry->key;
			void * value = entry->value;
			hash_table_put_private(table,key,value);
			j = entry->next;
		}
	}
	free(old_data);
	free(old_blocks);
	return 1;
}

string_buffer * hash_table_tostring(string_buffer * buffer, hash_table * table){
	string_buffer_add(buffer,"\n");
	char mdata[256];
	char mkey[256];
	char mvalue[256];
	int i,j;
	int blocks_not_empty = 0;
	for(i = 0; i < table->parameters.capacity; i++){
		int first = table->blocks[i];
		if(first < 0) continue;
		blocks_not_empty++;
		j = first;
		sprintf(mdata,"%d: ",i);
		string_buffer_add(buffer,mdata);
		string_buffer_add(buffer,"[");
		while(j>=0) {
			entry_data * entry = get_entry_data(table,j);
			void * key = entry->key;
			void * value = 	entry->value;
			sprintf(mdata,"(%s,%s)",table->key_type.tostring(mkey,key),table->value_type.tostring(mvalue,value));
			string_buffer_add(buffer,mdata);
			j = entry->next;
		}
		string_buffer_add(buffer,"]\n");
	}
	string_buffer_add(buffer,"}");
	sprintf(mdata,"  Effective load factor = %f, capacity: %d, size: %d",
			table->parameters.size/((double)blocks_not_empty),
			table->parameters.capacity,
			table->parameters.size
			);
	string_buffer_add(buffer,mdata);
	string_buffer_close(buffer);
    return buffer;
}

int get_index_block(hash_table * table, void * key){
    char mem[256];
    table->key_type.tostring(mem,key);
    unsigned long int hash_index = hash(mem);
    int index = (int)(hash_index%(table->parameters.capacity));
    assert(index >=0 && index < table->parameters.capacity && "no es el �ndice de un bloque posible");
    return index;
}


entry_data * find_entry_data(hash_table * table, int index_block, void * key) {
	entry_data * r = NULL;
	entry_data * s = NULL;
	int next = table->blocks[index_block];
	while (next > 0) {
		s = get_entry_data(table, next);
		if (table->key_type.equals(key, s->key)) {
			r = s;
			break;
		}
		next = s->next;
	}
	return r;
}

void * hash_table_get(hash_table * table, void * key) {
	int index = get_index_block(table,key);
	return find_entry_data(table,index,key);
}

tuple2 * hash_table_get_entry(hash_table * table, void * key){
	tuple2 e;
	e.key = key;
	e.value = hash_table_get(table,key);
	return tuple2_type.pointer(&e,get_hash_table_memory());
}

tuple2 * hash_table_get_entry_from_index(hash_table * table, int index_data){
	tuple2 e;
	entry_data ed = table->data[index_data];
	e.key = ed.key;
	e.value = ed.value;
	return tuple2_type.pointer(&e,get_hash_table_memory());
}

int hash_table_contains(hash_table * table, void * key) {
	return hash_table_get(table,key)?1:0;
}

entry_data * get_entry_data(hash_table * table, int index){
	assert(index < table->parameters.capacity_data && "no es un �ndice a una entrada del array de datos");
	return table->data+index;
}

void * hash_table_put(hash_table * table, void * key, void * value){
	rehash(table);
	return hash_table_put_private(table,key,value);
}

void * hash_table_put_private(hash_table * table, void * key, void * value){
	int index = get_index_block(table,key);
	entry_data * entry = find_entry_data(table,index,key);
	if(entry){
		entry->value = value;
	} else {
		int first_free = table->parameters.first_free_data;
		entry = get_entry_data(table,first_free);
		table->parameters.first_free_data = entry->next;
		table->parameters.size = table->parameters.size +1;
		entry->key = key;
		entry->value = value;
		if(table->blocks[index] < 0) {
			table->blocks[index] = first_free;
			entry->next = -1;
		} else {
			entry->next = table->blocks[index];
			table->blocks[index] = first_free;
		}
	}
	return key;
}

void * hash_table_remove(hash_table * table, void * key) {
	int index = get_index_block(table,key);
	entry_data * entry = find_entry_data(table,index,key);
	int old_first_free_data = table->parameters.first_free_data;
	int old_first_block = table->blocks[index];
	if(entry){
			int old_index = -1;
			int next_index = table->blocks[index];
			while(next_index >= 0){
				entry_data * next_data = get_entry_data(table,next_index);
				if(next_data == entry){
					if(old_index <= 0)
						table->blocks[index]= entry->next;
					else
						get_entry_data(table,old_index)->next = entry->next;
					break;
				}
				old_index = next_index;
				next_index = next_data->next;
			}
			assert(next_index >=0);
		entry->next = old_first_free_data;
		table->parameters.first_free_data = old_first_block;
	}
	return key;
}

bool iterate_hash_table(generator * ge) {
	hash_table_generator_parameters * p = (hash_table_generator_parameters *) ge->parameters;
	START(ge);
	for(p->index_block = 0; p->index_block < p->table->parameters.capacity; p->index_block ++) {
		for(p->index_data = p->table->blocks[p->index_block]; p->index_data >= 0;
				p->index_data = get_entry_data(p->table,p->index_data)->next) {
			if(p->index_data < 0) continue;
			entry_data * entry = get_entry_data(p->table,p->index_data);
			hash_table_generator_values * values = (hash_table_generator_values *) ge->values;
			values->data = entry;
			YIELD(ge);
		}
	}
	FINISH;
	return false;
}

generator hash_table_generator(hash_table_generator_parameters * p) {
	generator ge = { 0, iterate_hash_table, malloc(sizeof(hash_table_generator_values)), NULL };
	ge.parameters = p;
return ge;
}

void hash_table_entries_buffer(hash_table * table, string_buffer * buffer) {
	char mem[Tam_String];
	hash_table_generator_parameters pp = { 0, 0, table };
	generator gg = hash_table_generator(&pp);
	string_buffer_add_prefix(buffer);
	while (gg.iterate_function(&gg)) {
		entry_data * e = ((hash_table_generator_values *) gg.values)->data;
		sprintf(mem, "(%d,%.1lf)", *(int*) e->key, *(double*) e->value);
		string_buffer_add(buffer, mem);
	}
	string_buffer_add_sufix(buffer);
	printf("\n\n_______________");
}

void hash_table_free(hash_table * table){
	free(table->blocks);
	free(table->data);
}

