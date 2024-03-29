/*
 * ejemplos2.h
 *
 *  Created on: 14 oct. 2019
 *      Author: migueltoro
 */

#ifndef EJEMPLOS_FILES_H_
#define EJEMPLOS_FILES_H_

#include "../types/types.h"
#include "../types/iterables.h"
#include "../types/math2.h"
#include "../types/list.h"

long sum_primos_file(char * file);
int count_primos_file(char * file);
int count_primos_file_2(char * file);
list file_to_list(char * file);
list file_to_list_2(char * file);
list file_to_list_of_list(char * file);
bool is_palindrome(list * ls);
void test_ejemplos_files();


#endif /* EJEMPLOS_FILES_H_ */
