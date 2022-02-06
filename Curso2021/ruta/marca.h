/*
 * marca.h
 *
 *  Created on: 12 jul. 2019
 *      Author: migueltoro
 */

#ifndef MARCA_H_
#define MARCA_H_

#include "../coordenadas/coordenadas.h"
#include "../types/types.h"
#include "../types/dates.h"
#include "../types/list.h"
#include "../types/iterables.h"
#include "../types/accumulators.h"


typedef struct {
	time_t time;
	coordenadas_3d coordenadas;
}marca;

marca marca_of(time_t time, coordenadas_3d coordenadas);
marca * marca_parse(marca * out, const char * in, type * t);
char * marca_tostring(marca * m, char * mem, type * t);

bool marca_equals(const marca * e1, const marca * e2, type * t);
int marca_naturalorder(const marca * e1, const marca * e2, type * t);


extern type marca_type;

void test_marca();
#endif /* MARCA_H_ */
