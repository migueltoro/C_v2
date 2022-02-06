/*
 * palabras.h
 *
 *  Created on: 22 jul. 2019
 *      Author: migueltoro
 */

#ifndef LIBRO_H_
#define LIBRO_H_

#include "../types/iterables.h"
#include "../types/accumulators.h"

int numero_de_palabras(char * file);
multiset frecuencias_de_palabras(char * file);

void test_palabras();

#endif /* LIBRO_H_ */
