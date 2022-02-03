/*
 * ejemplos_tree.c
 *
 *  Created on: 22 dic. 2020
 *      Author: migueltoro
 */

#include "../trees/ejemplos_tree_1.h"

//Diseñe un algoritmo que dado un árbol n-ario Tree<E> y un predicado sobre E
//devuelva una lista List<Boolean> de forma que el elemento i-ésimo de la lista
//será “True” si todas las etiquetas no vacías del nivel i cumplen el predicado.

list niveles_cumplen_predicado_1(tree * t) {
	int height = tree_height(t);
	list r = list_empty(&bool_type);
	list level;
	for (int i = 0; i <= height; i++) {
		if (i == 0) {
			level = tree_level(t, 0);
		} else {
			list level_t = tree_next_level(&level);
			list_free(&level);
			level = level_t;
		}
		list lev_2 = list_filter(&level, tree_is_not_empty);
		list lev_label = list_map(&lev_2, t->label_type, tree_label_f);
		iterator it = list_iterable(&lev_label);
		bool b = iterable_all(&it, es_primo_f);
		list_add(&r, &b);
		list_free(&lev_2);
		list_free(&lev_label);
		iterable_free(&it);
	}
	list_free(&level);
	return r;
}



void niveles_cumplen_predicado_2_private(tree * t, int n, hash_table * ht, int height, bool (*p)(void * a)) {
	if (n > height) return;
	switch (tree_get_subtype(t)) {
	case Empty_Tree:
		break;
	case Leaf_Tree:
	case Nary_Tree: {
		bool r = *(bool*) hash_table_get(ht, &n);
		void * label = tree_label(t);
		r = r && p(label);
		hash_table_put(ht, &n, &r);
		int nh = tree_child_number(t);
		for (int i = 0; i < nh; i++) {
			niveles_cumplen_predicado_2_private(tree_get_child(t, i), n + 1, ht,height,p);
		}
	}
	}
}

list niveles_cumplen_predicado_2(tree * t, bool (*p)(void * a)) {
	hash_table ht = hash_table_empty(&int_type, &bool_type);
	int h = tree_height(t);
	for (int i = 0; i <= h; i++) {
		bool r = true;
		hash_table_put(&ht, &i, &r);
	}
	niveles_cumplen_predicado_2_private(t, 0, &ht, h, p);
	list ls = list_empty(&bool_type);
	for (int i = 0; i < h; i++) {
		bool * r = (bool*)hash_table_get(&ht, &i);;
		list_add(&ls,r);
	}
	hash_table_free(&ht);
	return ls;
}

bool pp(long * a){
	return es_primo(*a);
}

void test_ejemplos_trees_1() {
	printf("\n~~~~~~~~~~~~~~~~~~~~ Test Tree 1 ~~~~~~~~~~~~~~~~~~~~\n");
	char mem[300];
	char mem2[300];
	iterator it = file_iterable_string_fix("ficheros/trees_1.txt");
	while (iterable_file_has_next(&it)) {
		char* line = (char*) iterable_file_next(&it);
		tree * t = tree_parse(line);
		tree * t2 = tree_map(t, &int_type, int_parse);
		list r = niveles_cumplen_predicado_2(t2,pp);
		printf("Tree = %s, Niveles = %s\n",tree_tostring(t2,mem),list_tostring(&r,mem2));
		heap_clear(&heap_tree);
		list_free(&r);
	}
	iterable_free(&it);
}


void test__ejemplos_trees_1_1() {
	char mem[300];
	hash_table ht = hash_table_empty(&int_type, &bool_type);
	int h = 4;
	for (int i = 0; i <= h; i++) {
		bool r = true;
		hash_table_put(&ht, &i, &r);
	}
	bool r = false;
	hash_table_put(&ht, &h, &r);
	iterator it = hash_table_items_iterable(&ht);
	iterable_to_console(&it);
	printf("\n%s",tostring(hash_table_get(&ht,&h),mem,&bool_type));
	bool r1 = false;
	bool r2 = false;
	bool r3 = equals(&r1,&r2,&bool_type);
	printf("\n%s",tostring(hash_table_get(&ht,&h),mem,&bool_type));
	printf("\n%s",MSG_BOOL(r3));
}
