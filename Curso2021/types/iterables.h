/*
 * iterables.h
 *
 *  Created on: 1 jul. 2019
 *      Author: migueltoro
 */



#ifndef ITERABLES_H_
#define ITERABLES_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#include "../types/math2.h"
#include "../types/types.h"
#include "../types/preconditions.h"
#include "../types/math2.h"
#include "heap.h"


typedef struct st {
	type * type;
	struct st * dp_iterable;
	void * state;
	void * a_state;
	bool (*has_next)(struct st * iterator);
	void * (*next)(struct st * iterator);
	void * (*see_next)(struct st * iterator);
	void (*free_dp)(void * in);
	void * dps;
} iterator;


//extern memory_heap memory_heap_iterable;

bool iterable_has_next(iterator * st);
void * iterable_see_next(iterator * st);
void * iterable_next(iterator * st);

iterator * iterable_copy(iterator * it);

iterator iterable_empty();
iterator iterable_range_long(long a, long b, long c);
iterator iterable_range_double(double a, double b, double c);
iterator iterable_iterate(type * type,void * initial_value, bool (*hash_next)(void * element), void * (*next)(void * out, void * in));
iterator file_iterable_string_fix(char * file);
iterator file_iterable_string_fix_tam(char * file, int num_chars_per_line_max);
iterator text_to_iterable_string_fix(char * text, const char * delimiters);
iterator text_to_iterable_string_fix_tam(char * text, const char * delimiters, int tam);
iterator * text_to_iterable_string_fix_function(iterator * out, char * text);
iterator iterable_random_int(int n, int a, int b);
iterator iterable_words_in_file(char * file, int line_tam, int word_tam, char * sep);
iterator iterable_primos(int a, int b);


extern string_fix text_to_iterable_delimiters;

iterator iterable_map(iterator * st,type * type, void * (*map_function)(void * out, const void * in));
iterator iterable_filter(iterator * st, bool (*map_filter)(void * in));
iterator iterable_flatmap(iterator * st, type * type, iterator * (*map_function)(iterator * out, void * in));
iterator iterable_consecutive_pairs(iterator * st);
iterator iterable_enumerate(iterator * st);


char * iterable_tostring(iterator * st, char * mem);
char * iterable_tostring_sep(iterator * st, char * sep, char * prefix, char * suffix, char * mem);
void iterable_to_console(iterator * st);
void iterable_to_console_sep(iterator * st, char * sep, char * prefix, char * suffix);
string_var iterable_tostring_sep_big(iterator * st,char * sep,char * prefix,char * suffix);
string_var iterable_tostring_big(iterator * st);

void write_iterable_to_file(char * file, iterator * st);

iterator iterable_create(
		type * type,
		bool (*has_next)(struct st * iterator),
		void * (*next)(struct st * iterator),
		void * (*see_next)(struct st * iterator),
		void (*free_dependencies)(void * in),
		void * dependencies,
		int size_dependencies);

void iterable_copy_state_to_auxiliary(iterator * st);

void iterable_free(iterator * in);

void test_iterables_1();
void test_iterables_2();
void test_iterables_3();
void test_iterables_5();
void test_iterables_6();
void test_iterables_7();
void test_iterables_8();

#endif /* ITERABLES_H_ */
