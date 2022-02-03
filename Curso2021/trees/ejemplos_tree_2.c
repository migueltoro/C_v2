/*
 * ejemplos_trees_2.c
 *
 *  Created on: 8 ene. 2021
 *      Author: migueltoro
 */

#include "../trees/ejemplos_tree_2.h"

//Diseñe un algoritmo recursivo que dado un árbol n-ario de tipo genérico devuelva
//un Map<Integer, List<Tree<E>>>. Dicho map en cada entrada tiene como clave el número de hijos de un árbol
//y como valor una lista con todos los árboles que tienen ese número de hijos.


list_multimap arboles(tree * t) {
	int key =tree_child_number(t);
	list_multimap r = list_multimap_empty(&int_type, &tree_type);
	list_multimap_put(&r, &key, t);
	switch (tree_get_subtype(t)) {
	case Empty_Tree: break;
	case Leaf_Tree: break;
	case Nary_Tree:
		for(int i = 0; i<key; i++){
			list_multimap hi = arboles(tree_get_child(t,i));
			list_multimap s = r;
			r = list_multimap_add(&s,&hi);
			list_multimap_free(&hi);
			list_multimap_free(&s);
		}
	}
	return r;
}



void arboles_2_private(tree * t, list_multimap * r) {
	int key =tree_child_number(t);
	list_multimap_put(r, &key, t);
	switch (tree_get_subtype(t)) {
	case Empty_Tree: break;
	case Leaf_Tree: break;
	case Nary_Tree:
		for(int i = 0; i<key; i++){
			arboles_2_private(tree_get_child(t,i),r);
		}
	}
}

list_multimap arboles_2(tree * t) {
	list_multimap r = list_multimap_empty(&int_type, &tree_type);
	arboles_2_private(t,&r);
	return r;
}


void test_ejemplos_trees_2() {
	printf("\n~~~~~~~~~~~~~~~~~~~~ Test Tree 2 ~~~~~~~~~~~~~~~~~~~~\n");
	char mem[300];
	char mem2[300];
	iterator it = file_iterable_string_fix("ficheros/trees_2.txt");
	while (iterable_file_has_next(&it)) {
		char* line = (char*) iterable_file_next(&it);
		tree * t = tree_parse(line);
		tree * t2 = tree_map(t, &int_type, int_parse);
		list_multimap r = arboles(t2);
		printf("Tree = %s, Arboles = %s\n",tree_tostring(t2,mem),list_multimap_tostring(&r,mem2));
		list_multimap_free(&r);
		heap_clear(&heap_tree);
	}
	iterable_free(&it);
}
