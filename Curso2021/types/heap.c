/*
 * memory_heap.c
 *
 *  Created on: 5 nov. 2018
 *      Author: migueltoro
 */


#include "heap.h"

void grow_memory_heap(heap * heap) {
	if (heap->size == heap->tam) {
		heap->tam = 2* heap->tam;
		heap->elements = (void *) realloc(heap->elements, sizeof(void *)*heap->tam);
//		assert(heap->elements != NULL && "no se ha podido reubicar la memoria");
	}
}

heap heap_empty() {
	heap r = {0, 10, malloc(10 * sizeof(void *)), 0, 0};
	return r;
}

heap * memory_heap_add(heap * heap, void * element) {
	grow_memory_heap(heap);
	heap->elements[heap->size] = element;
	heap->size = heap->size + 1;
	return heap;
}

void * heap_copy_and_mem(heap * heap, void * source, int size_element){
	check_argument(size_element >= 0,__FILE__,__LINE__,"El tamaño a copiar debe ser mayor o igual a cero");
	if(source == NULL || size_element == 0 ) return NULL;
	void * element = (void *) malloc(size_element);
	memcpy(element,source,size_element);
	memory_heap_add(heap, element);
	heap->size_memory += size_element;
	heap->num_access_heap++;
//	printf("\n%d==%d=%d\n",size_element,heap->size_memory,heap->num_access_memory_heap);
	return element;
}

void * heap_get_memory(heap * heap,int size){
	if(size == 0) return NULL;
	void * element = (void *) malloc(size);
	memory_heap_add(heap, element);
	heap->size_memory = heap->size_memory +size;
	heap->num_access_heap++;
//	printf("\n%d==%d==%d\n",size,heap->size_memory,heap->num_access_memory_heap);
	return element;
}

int heap_size_memory(heap * heap){
	return heap->size_memory;
}

void heap_free(heap * heap) {
	for (int i = 0; i < heap->size; i++) {
		free(heap->elements[i]);
	}
	free(heap->elements);
}

void heap_clear(heap * heap) {
	for (int i = 0; i < heap->size; i++) {
			free(heap->elements[i]);
		}
	free(heap->elements);
	heap->size=0;
	heap->tam=10;
	heap->size_memory = 0;
	heap->num_access_heap = 0;
	heap->elements=malloc(10 * sizeof(void *));
}

bool heap_isnull(heap * heap){
	return heap->elements == NULL;
}

heap global_heap;
