/*
 * accumulators.h
 *
 *  Created on: 7 jul. 2019
 *      Author: migueltoro
 */

#ifndef ACCUMULATORS_H_
#define ACCUMULATORS_H_

#include "../types/types.h"
#include "../types/hash_table.h"
#include "../types/iterables.h"
#include "../types/list.h"
#include "../types/set.h"
#include "../types/multiset.h"
#include "../types/multimap.h"
#include "../types/multiset.h"




typedef struct {
	int num;
	double sum;
	double sum_cuadrados;
	double max;
	double min;
	double media;
	double varianza;
	double desviacion_tipica;
} estadisticos;

char * estadisticos_tostring(void * in, char * mem);

void * accumulate_left(iterator * st, void * base, bool (*add)(void * out, const void * e));
void * accumulate_right(iterator * st, void * base, bool (*add)(void * out, const void * e));


void * reduce_left(iterator * st, void * base, bool (*add)(void * out, const void * e));
void * reduce_right(iterator * st, void * base, bool (*add)(void * out, const void * e));



void * iterable_min(iterator * st,int (*comparator)(const void * e1, const void * e2));
void * iterable_min_naturalorder(iterator * st);
void * iterable_max(iterator * st, int (*comparator)(const void * e1, const void * e2));
void * iterable_max_naturalorder(iterator * st);
bool iterable_all(iterator * st, bool (*p)(const void * in));
bool iterable_any(iterator * st, bool (*p)(const void * in));
void * iterable_first(iterator * st, bool (*p)(const void * in));
double iterable_sum(iterator * st);
int iterable_size(iterator * st);
double iterable_average(iterator * st);
int iterable_num_differents(iterator * st);
bool iterable_all_differents(iterator * st);

list iterable_to_list(iterator * st);
set iterable_to_set(iterator * st);
multiset iterable_to_multiset(iterator * st);
list_multimap iterable_to_list_multimap(iterator * st);
set_multimap iterable_to_set_multimap(iterator * st);
multiset iterable_to_multiset_groups(iterator * st, type * key_type, void * (*f_key)(void * out, void * in));
list_multimap iterable_grouping_list(iterator * st, type * key_type, void * (*f_key)(void * out, void * in));
set_multimap iterable_grouping_set(iterator * st, type * key_type, void * (*f_key)(void * out, void * in));
hash_table iterable_grouping_reduce(iterator * st, type * key_type, void * (*f_key)(void * out, void * in), bool (*add)(void * out, const void * e));


void test_accumulators_1();
void test_accumulators_2();
void test_accumulators_3();
void test_accumulators_4(char * file);
void test_accumulators_5(char * file);
void test_accumulators_6();

#endif /* ACCUMULATORS_H_ */
