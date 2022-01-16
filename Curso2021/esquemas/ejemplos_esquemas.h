/*
 * ejemplos_esquemas.h
 *
 *  Created on: 9 ene. 2021
 *      Author: migueltoro
 */

#ifndef EJEMPLOS_ESQUEMAS_H_
#define EJEMPLOS_ESQUEMAS_H_

#include "../types/types.h"
#include "../types/iterables.h"
#include "../types/math2.h"
#include "../types/list.h"
#include "../types/accumulators.h"
#include "../types/multiset.h"

multiset frecuencias_en_fichero_1(char * file,type * t);
list_multimap grupos_list_en_fichero_1(char * file, type * element_type, type * key_type,
		   void * f_key(void *out, void * in));
set_multimap grupos_set_en_fichero_1(char * file, type * element_type, type *key_type,
		   void * f_key(void *out, void * in));

void test_esquemas();

#endif /* EJEMPLOS_ESQUEMAS_H_ */
