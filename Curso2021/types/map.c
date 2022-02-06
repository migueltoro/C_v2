/*
 * hash_table.c
 *
 *  Created on: 4 nov. 2018
 *      Author: migueltoro
 */

#include "map.h"

#include "../types/list.h"

int _primes[] = {13, 23, 41, 61, 83, 151, 199, 263, 383, 503, 641, 769, 911, 1049, 1559, 1709, 2069,5923,11587,56527};
int _next_prime = 0;
int _nprimes = 20;


int get_index_block(map * table, void * key);
int find_data_entry(map * table, int bq, void * key);
void ini_data(map * table);
int rehash(map * table);
void * hash_table_put_private(map * table, void * key, void * value);

map map_empty(type * key_type, type * value_type){
	map t;
	t.key_type = key_type;
	t.value_type = value_type;
	t.load_factor_limit = 0.75;
	int capacity_blocks = _primes[_next_prime];
	t.capacity_blocks = capacity_blocks;
	t.capacity_data = (int) (t.load_factor_limit* capacity_blocks + 1);
	ini_data(&t);
	return t;
}

void ini_data(map * table) {
	table->blocks = (int *) malloc(table->capacity_blocks*sizeof(int));
	table->data = (entry *) malloc(table->capacity_data*sizeof(entry));
	table->size = 0;
	table->first_free_data = 0;
	for (int i = 0; i < table->capacity_blocks; i++) {
		table->blocks[i] = -1;
	}
	for (int i = 0; i < table->capacity_data; i++) {
		table->data[i].next = i+1;
	}
	table->data[table->capacity_data-1].next = -1;
}

int get_index_block(map * table, void * key) {
	unsigned long int hash_index = hash_code(key,table->key_type);
	int index = (int) (hash_index % (table->capacity_blocks));
	return index;
}

int find_data_entry(map * table, int bq, void * key) {
	int r = -1;
	int next = table->blocks[bq];
	while (next >= 0) {
		if (equals(key,table->data[next].key,table->key_type)) {
			r = next;
			break;
		}
		next = table->data[next].next;
	}
	return r;
}

int map_size(map * table){
	return table->size;
}

void * map_put_pointer(map * table, void * key, void * value){
	rehash(table);
	return hash_table_put_private(table,key,value);
}

void * map_put(map * table, void * key, void * value){
	void * k = copy(key,NULL,table->key_type);
    void * v = copy(value,NULL,table->value_type);
    return map_put_pointer(table,k,v);
}

map * map_put_all(map * table, iterator * items){
	while(iterable_has_next(items)){
		pair * p = (pair *) iterable_next(items);
		map_put(table,p->key,p->value);
	}
	return table;
}

map map_copy(map * table) {
	map r = map_empty(table->key_type,table->value_type);
	iterator items = map_items_iterable(table);
	while (iterable_has_next(&items)) {
		pair * p = (pair *) iterable_next(&items);
		map_put(&r, p->key, p->value);
	}
	return r;
}

void * map_get(map * table, void * key){
	int index = get_index_block(table,key);
	int pos = find_data_entry(table,index,key);
	void * r = NULL;
	if(pos >=0){
		r = table->data[pos].value;
	}
	return r;
}


bool map_contains_key(map * table, void * key) {
	int index = get_index_block(table, key);
	int pos = find_data_entry(table, index, key);
	bool r = false;
	if (pos >= 0) {
		r = true;
	}
	return r;
}

int rehash(map * table) {
	if (table->size < table->capacity_data) return 0;
	_next_prime = _next_prime + 1;
	check_argument(_next_prime < _nprimes,__FILE__,__LINE__,"se ha acabado los números primos disponibles %d",_next_prime);
	int old_capacity_blocks = table->capacity_blocks;
	int old_capacity_data = table->capacity_data;
	int * old_blocks = table->blocks;
	entry * old_data = table->data;
	int capacity_blocks = _primes[_next_prime];
	table->capacity_blocks = capacity_blocks;
	table->capacity_data = (int) (0.75* capacity_blocks + 1);
	ini_data(table);
	for (int i = 0; i < old_capacity_blocks; i++) {
		check_element_index( i,old_capacity_blocks,__FILE__,__LINE__);
		int first = old_blocks[i];
		if (first < 0) continue;
		int j = first;
		while (j >= 0) {
			check_element_index(j,old_capacity_data,__FILE__,__LINE__);
			hash_table_put_private(table, old_data[j].key, old_data[j].value);
			j = old_data[j].next;
		}
	}
	free(old_blocks);
	free(old_data);
	return 1;
}

entry * ocupa_primera_libre(map * table, int bq) {
	int first_free = table->first_free_data;
	table->first_free_data = table->data[first_free].next;
	if (table->blocks[bq] < 0) {
		table->data[first_free].next = -1;
	} else {
		table->data[first_free].next = table->blocks[bq];
	}
	table->blocks[bq] = first_free;
	return table->data+first_free;
}

void libera(map * table, int bq, int index_data) {
	if (table->blocks[bq] == index_data) {
		table->blocks[bq] = table->data[index_data].next;
	} else {
		int i = table->blocks[bq];
		while (table->data[i].next != index_data) {
			i = table->data[i].next;
		}
		table->data[i].next = table->data[index_data].next;
	}
	table->data[index_data].next = table->first_free_data;
	table->first_free_data = index_data;
}

void * hash_table_put_private(map * table, void * key, void * value) {
	int index = get_index_block(table, key);
	int pos = find_data_entry(table, index, key);
	if (pos >= 0) {
		table->data[pos].value = value;
	} else {
		table->size = table->size + 1;
		entry * e = ocupa_primera_libre(table,index);
		e->key = key;
		e->value = value;
	}
	return key;
}

void * map_remove(map * table, void * key) {
	int index = get_index_block(table, key);
	int index_data = find_data_entry(table,index,key);
	if (index_data >= 0) {
		table->size = table->size - 1;
		libera(table, index, index_data);
	}
	return key;
}

map * map_remove_all(map * table, iterator * keys){
	while(iterable_has_next(keys)){
		void * key = iterable_next(keys);
		map_remove(table,key);
	}
	return table;
}

typedef struct{
	map * ht;
	int nb;
	int i;
	int j;
}dp_hash_table;

bool iterable_hash_table_has_next(iterator * c_iterable) {
	dp_hash_table * dp = (dp_hash_table *) c_iterable->dps;
	return dp->i < dp->nb;
}

void * iterable_hash_table_see_next(iterator * c_iterable){
	dp_hash_table * dp = (dp_hash_table *) c_iterable->dps;
	map * table = dp->ht;
	pair * state = (pair *)c_iterable->state;
	state->key = table->data[dp->j].key;
	state->value = table->data[dp->j].value;
    return c_iterable->state;
}

void next_state(dp_hash_table * dp){
	map * table = dp->ht;
	int i = dp->i;
	int j = dp->j;
	while(i<dp->nb){
		if(j<0) j = table->blocks[i];
		else j = table->data[j].next;
		if(j>=0) break;
		i++;
	}
	dp->i = i;
	dp->j = j;
}

void * iterable_hash_table_next(iterator * c_iterable){
	dp_hash_table * dp = (dp_hash_table *) c_iterable->dps;
	map * table = dp->ht;
	pair * state = (pair *)c_iterable->state;
	state->key = table->data[dp->j].key;
	state->value = table->data[dp->j].value;
	next_state(dp);
	return c_iterable->state;
}

iterator map_items_iterable(map * ht){
	dp_hash_table dh = {ht,ht->capacity_blocks,0,-1};
	int size_dh = sizeof(dp_hash_table);
	type t = generic_type_2(&pair_type,ht->key_type,ht->value_type);
	iterator s_hash_table = iterable_create(type_copy(&t,NULL),iterable_hash_table_has_next,iterable_hash_table_next,iterable_hash_table_see_next,NULL,&dh,size_dh);
	next_state(s_hash_table.dps);
	return s_hash_table;
}


char * map_tostring(map * table, char * mem) {
	char m1[Tam_String];
	char m2[Tam_String];
	char m[Tam_String];
	bool first = true;
	strcpy(mem, "{");
	iterator st = map_items_iterable(table);
	while (iterable_has_next(&st)) {
		pair * next = (pair *) iterable_next(&st);
		char * k = tostring(next->key,m1,table->key_type);
		char * v = tostring(next->value,m2,table->value_type);
		sprintf(m,"(%s:%s)",k,v);
		if (first) {
			first = false;
			strcat(mem, m);
		} else {
			strcat(mem, ",");
			strcat(mem, m);
		}
	}
	strcat(mem, "}");
	return mem;
}

void map_toconsole(map * table){
	printf("\n");
	char mdata[500];
	char mkey[256];
	char mvalue[256];
	int i,j;
	int blocks_not_empty = 0;
	for(i = 0; i < table->capacity_blocks; i++){
		int first = table->blocks[i];
		if(first < 0) continue;
		j = first;
		sprintf(mdata,"%d: ",i);
		blocks_not_empty++;
		printf("%s",mdata);
		printf("[");
		while(j>=0) {
			void * key = table->data[j].key;
			void * value = 	table->data[j].value;
			sprintf(mdata,"(%s,%s)",tostring(key,mkey,table->key_type),
					tostring(value,mvalue,table->value_type));
			printf("%s",mdata);
			j = table->data[j].next;
		}
		printf("]\n");
	}
	printf("}");
	sprintf(mdata,"  Effective load factor = %f, blocks_not_empty = %d, capacity: %d, size: %d",
			table->size/((double)blocks_not_empty),
			blocks_not_empty,
			table->capacity_blocks,
			table->size
			);
	printf("%s",mdata);
}

void map_free(map *table) {
	type * tk = table->key_type;
	type * tv = table->value_type;
	for (int i = 0; i < table->capacity_data; i++) {
		entry e = table->data[i];
		tk->free(e.key,tk);
		tv->free(e.value,tv);
	}
	type_free(table->key_type);
	type_free(table->value_type);
	free(table->blocks);
	free(table->data);
	free(table);
}


map complete_table1() {
	int tam = 100;
	map ht = map_empty(&long_type,&double_type);
	new_rand();
	for (int i = 0; i < tam; i++) {
		long a1 = i;
		double a2 = double_aleatorio(0, 1000);
		map_put(&ht,&a1,&a2);
	}
	for (int i = 3; i < tam; i++) {
		long a1 = i;
		double a2 = double_aleatorio(0, 1000);
		map_put(&ht,&a1,&a2);
	}
	long a1 = 5;
	map_remove(&ht,&a1);
	return ht;
}

map complete_table2() {
	int tam = 100;
	map ht = map_empty(&long_type,&double_type);
	new_rand();
	for (int i = 0; i < tam; i++) {
		long a1 = i;
		double a2 = double_aleatorio(1000, 2000);
		map_put(&ht,&a1,&a2);
	}
	for (int i = 3; i < tam; i++) {
		long a1 = i;
		double a2 = double_aleatorio(1000, 2000);
		map_put(&ht,&a1,&a2);
	}
	long a1 = 5;
	map_remove(&ht,&a1);
	return ht;
}


void test_map() {
	char mem[3000];
	printf("Hash Table test\n\n");
	printf("2:\n");
	map ht = complete_table1();
	map ht2 = map_copy(&ht);
	map_toconsole(&ht2);
	printf("%s\n",map_tostring(&ht2,mem));
	printf("\n6: \n");
	iterator iht = map_items_iterable(&ht2);
	iterable_to_console_sep(&iht,"\n","","\n");
}


