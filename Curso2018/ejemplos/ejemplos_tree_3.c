/*
 * ejemplos_tree_3.c
 *
 *  Created on: 8 ene. 2021
 *      Author: migueltoro
 */

#include "ejemplos_tree_3.h"

//Diseñe un algoritmo que dado un árbol binario de enteros devuelva cierto en caso de que para
//cada nodo que tenga 2 hijos no vacíos se cumpla que su etiqueta es igual a la suma de las etiquetas
//de las raíces de sus 2 hijos.



bool suma_igual(binary_tree * t){
	bool r = true;
	switch(binary_tree_get_subtype(t)){
	case Empty_Binary_Tree: break;
	case Leaf_Binary_Tree: break;
	case Binary_Tree:
		if(!binary_tree_is_empty(binary_tree_left(t)) && !binary_tree_is_empty(binary_tree_right(t))){
			int lv = *(int*)binary_tree_label(binary_tree_left(t));
			int rv = *(int*)binary_tree_label(binary_tree_right(t));
			int label = *(int*)binary_tree_label(t);
			r = label == lv+rv;
		}
		r = r && suma_igual(binary_tree_left(t)) && suma_igual(binary_tree_right(t));
	}
	return r;
}

void test_trees_3() {
	printf("\n~~~~~~~~~~~~~~~~~~~~ Test Tree 3 ~~~~~~~~~~~~~~~~~~~~\n");
	char mem[300];
	memory_heap_binary_tree = memory_heap_create();
	iterator it = file_iterable_pchar("ficheros/trees_3.txt");
	while (iterable_file_has_next(&it)) {
		char* line = (char*) iterable_file_next(&it);
		binary_tree * t = binary_tree_parse_m(line);
		binary_tree * t2 = binary_tree_map_m(t, &int_type, int_parse);
		bool r = suma_igual(t2);
		printf("Tree = %s, Cumple = %s\n",binary_tree_tostring(t2,mem),MSG_BOOL(r));
		memory_heap_clear(&memory_heap_binary_tree);
	}
	iterable_free(&it);
}
