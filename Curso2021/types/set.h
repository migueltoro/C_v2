/*
 * set.h
 *
 *  Created on: 28 abr. 2019
 *      Author: migueltoro
 */

#ifndef SET_H_
#define SET_H_


#include "../types/types.h"
#include "../types/iterables.h"
#include "../types/list.h"
#include "map.h"

typedef struct {
	map hash_table;
} set;

set set_empty(type* type_element);
set set_of(list * ls);
void set_add(set * st, void * element);
void set_add_all(set * st, iterator * it);
void set_remove(set * st, void * element);
int set_size(set * st);
bool set_contains(set * st, void * element);

char * set_tostring(set * st, char * mem);
iterator set_iterable(set * st);
set set_map(set * st, type * type_out, void * (*f)(void * out, void * in));
set set_filter(set * st, bool (*p)(void * in));
list set_tolist(const set * s);

//Los volcamos a listas y ordenamos por o. natural
bool set_equals(const set * s1, const set * ls2);
set * set_parse(set * out, char * text);
set set_parse_s(char * text);

extern type set_type;


void set_free(set * st);
void set_free_2(set * st, void (*f)(void * in));

set complete_set();
void test_set_1();
void test_set_2();
void test_set_3();

#endif /* SET_H_ */
