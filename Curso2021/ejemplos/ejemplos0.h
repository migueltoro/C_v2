/*
 * ejemplos.h
 *
 *  Created on: 19 nov. 2018
 *      Author: migueltoro
 */

#ifndef EJEMPLOS0_H_
#define EJEMPLOS0_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#include "../types/types.h"
#include "../types/iterables.h"
#include "../types/heap.h"
#include "../types/list.h"
#include "../types/map.h"
#include "../types/math2.h"


double n_problema1(list ls);

list n_problema2(list ls);

bool n_problema3(list ls, bool (*p)(int e));

bool n_problema4(list ls, bool (*p)(int e));

int n_problema5(list ls);

double n_problema6(list ls, double umbral);

pair_double * n_problema8(list ls, int (*cmp)(const void *, const void *));

map n_problema11(list ls);

list n_problema61(char * file);

void n_problema59(char * file,long n);

void test_ejemplos_1();
void test_ejemplos1();

#endif /* EJEMPLOS0_H_ */
