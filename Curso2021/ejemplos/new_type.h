/*
 * tupla.h
 *
 *  Created on: 30 nov. 2020
 *      Author: migueltoro
 */

#ifndef TUPLA_H_
#define TUPLA_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../types/types.h"


typedef struct {
	int li;
	int ls;
	int suma;
}smax;

smax smax_of(int li,int ls,int suma);
int * smax_parse(smax * out, char * text, type * t);
char * smax_tostring(const smax * e, char * mem, type * t);
bool smax_equals(const smax * e1, const smax * e2, type * t);
int smax_naturalorder(const smax * e1,const smax * e2, type * t);

extern type smax_type;

#endif /* TUPLA_H_ */
