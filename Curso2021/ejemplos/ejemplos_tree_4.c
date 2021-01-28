/*
 * ejemplos_tree_4.c
 *
 *  Created on: 8 ene. 2021
 *      Author: migueltoro
 */


#include "ejemplos_tree_4.h"

//Implemente una función booleana que, dados un árbol binario de caracteres y una lista de caracteres,
//determine si existe un camino en el árbol de la raíz a una hoja que sea igual a la lista.

bool etiqueta_igual(binary_tree* t, list * ls, int i){
	return !binary_tree_is_empty(t) && *(char*)list_get(ls,i) == *(char*) binary_tree_label(t);
}

bool existe_camino(binary_tree* t, list * ls){
	int n = list_size(ls);
	bool r;
	switch(binary_tree_get_subtype(t)){
	case Empty_Binary_Tree: r = n==0; break;
	case Leaf_Binary_Tree: r = n==1 && etiqueta_igual(t,ls,0); break;
	case Binary_Tree:
		r = n>1 && etiqueta_igual(t,ls,0);
		binary_tree* left = binary_tree_left(t);
		binary_tree* right = binary_tree_right(t);
		bool le = etiqueta_igual(left,ls,1);
		bool re = etiqueta_igual(right,ls,1);
		list sl = list_sublist(ls,1,n);
		r = r && (existe_camino(left,&sl) || existe_camino(right,&sl));
	}
	return r;
}

list list_parse_2(char * text) {
	iterator it = text_to_iterable_pchar(text," ,[]");
	list r = list_empty(&char_type);
	while (iterable_has_next(&it)) {
		char * e1 = (char*) iterable_next(&it);
		if(pchar_all_space(e1)) continue;
		char e = e1[0];
		list_add(&r,&e);
	}
	return r;
}

void test_ejemplos_trees_4() {
	printf("\n~~~~~~~~~~~~~~~~~~~~ Test Tree 4 ~~~~~~~~~~~~~~~~~~~~\n");
	char mem[300];
	char mem1[300];
	iterator it = file_iterable_pchar("ficheros/trees_4.txt");
	while (iterable_file_has_next(&it)) {
		char* line = (char*) iterable_file_next(&it);
		iterator it2 = text_to_iterable_pchar(line,"#");
		char * tree_line = iterable_next(&it2);
		char * list_line = iterable_next(&it2);
		binary_tree * t = binary_tree_parse_m(tree_line);
		binary_tree * t2 = binary_tree_map_m(t, &char_type, char_parse);
		list ls = list_parse_2(list_line);
		bool r = existe_camino(t2,&ls);
		printf("Tree = %s, Lista %s, Cumple = %s\n",binary_tree_tostring(t2,mem),
				list_tostring(&ls,mem1), MSG_BOOL(r));
        memory_heap_clear(&memory_heap_binary_tree);
	}
	iterable_free(&it);
}
