/*
 * sorted_list.h
 *
 *  Created on: 1 nov. 2018
 *      Author: migueltoro
 */

#ifndef LIST_H_
#define LIST_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../types/iterables.h"
#include "../types/math2.h"
#include "../types/types.h"
#include "heap.h"

typedef struct {
	bool is_view;
	type * type_element;
	int size;
	int tam;
	void ** elements;
	heap hp;
} list;

void swap_in_list(list * ls, int a, int b);

list list_empty(type * element);
list list_empty_tam(type * element, int tam);
list list_of(void * data, int size, type * type);
list list_of_file(char * file);
list list_of_file_type(char * file, type * type);
list list_of_list_of_file_type(char * file, type * type);
list list_of_int(int n, ...);
list list_of_long(int n, ...);
list list_of_double(int n, ...);
list list_of_string(int n, ...);
list list_of_type(type * type, int n, ...);

list list_sublist(list * ls, int a, int b);
void * list_get(list * ls, const int index);
char * list_get_string(list * ls, const int index, char * mem);
int list_size(list * ls);
void list_add_pointer(list * ls, void * element);
void list_add(list * ls, void * element);
list * list_add_all(list * ls, iterator * it);
list * list_add_list(list * ls, list * add_list);
void list_add_left(list * ls, void * element);
void * list_set(list * list, const int index, const void * e);
void list_set_pointer(list * list, int index, void * element);


list list_copy(list * in);
list list_filter(list * ls, bool (*predicate)(void * e));
list list_map(list * ls, type * type_element, void * (*f)(void * out, void * in));

char * list_tostring(list * ls, char * mem);
bool list_contains(list * list, const void * e);
bool list_equals(const list * ls1, const list * ls2);
list * list_parse(list * out, char * text);
list list_parse_s(char * text);
extern string_fix list_delimiters;

void * list_to_array(list * ls, void * array);
void * list_of_list_to_2_array(list * ls, void * array);

void write_list_to_file(char * file, list * list, char * tostring(const void * source, char * mem));

iterator list_iterable(list * ls);

extern type list_type;

int_pair bh(list * ls, void * pivot, int i, int j, int (*order)(const void * e1, const void * e2));
void list_quick_sort(list * ls, int (* order)(const void * e1, const void * e2));
void basic_sort(list * ls, int (*order)(const void * e1, const void * e2));
int bs(list * ls, void * element, int (* order)(const void * e1, const void * e2));
void list_merge_sort(list * ls, int (*order)(const void * e1, const void * e2));
list merge_list(list * ls1, list * ls2, int (* order)(const void * e1, const void * e2));

void * k_esimo(list * ls,int k, int (*order)(const void * e1, const void * e2));

int_pair bh_naturalorder(list * ls, void * pivot, int i, int j);
void list_quick_sort_naturalorder(list * ls);
void basic_sort_naturalorder(list * ls);
int bs_naturalorder(list * ls, void * element);
void list_merge_sort_naturalorder(list * ls);
list merge_list_naturalorder(list * ls1, list * ls2);
void * k_esimo_naturalorder(list * ls,int k);

void list_free(list * ls);
void list_free_2(list * ls, void (*f)(void * in));
void test_list_1();
void test_list_2();
void test_list_3();
void test_list_4();
void test_list_5();

#endif /* LIST_H_ */
