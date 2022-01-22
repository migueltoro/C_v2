/*
 * memory_heap.h
 *
 *  Created on: 5 nov. 2018
 *      Author: migueltoro
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "../types/preconditions.h"

;
typedef struct {
	int size;
	int tam;
	void ** elements;
	int size_memory;
	int num_access_heap;
} heap;


// #define heap_initial {0,10,malloc(10*sizeof(void *))}

heap heap_empty();
void * heap_copy_and_mem(heap * heap, void * source, int size);
void * heap_get_memory(heap * heap,int size);
int heap_size_memory(heap * heap);

void heap_free(heap * heap);
void heap_clear(heap * heap);
bool heap_isnull(heap * heap);

extern heap global_heap;
extern heap scope_heap;


#endif /* HEAP_H_ */
