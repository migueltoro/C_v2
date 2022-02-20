/*
 * ejemplos_esquemas.c
 *
 *  Created on: 9 ene. 2021
 *      Author: migueltoro
 */

#include "../esquemas/ejemplos_esquemas.h"

//Encontrar las frecuencias con las que se repiten los números de un fichero

multiset frecuencias_en_fichero_1(char * file, type * t) {
	iterator it = iterable_file_string_fix(file);
	multiset ms = multiset_empty(t);
	char e[t->size];
	while (iterable_has_next(&it)) {
		char * line = (char *) iterable_next(&it);
		iterator it2 = iterable_split_text_tam(line, " ,",string_fix_tam);
		while (iterable_has_next(&it2)) {
			char * num = (char *) iterable_next(&it2);
			parse(e, num, t);
			multiset_add(&ms,e);
		}
	}
	return ms;
}



list_multimap grupos_list_en_fichero_1(char * file, type *element_type, type *key_type,
		void * f_key(void *out, void * in)) {
	iterator it = iterable_file_string_fix(file);
	list_multimap ms = list_multimap_empty(key_type,element_type);
	char e[element_type->size];
	char key[key_type->size];
	while (iterable_has_next(&it)) {
		char * line = (char *) iterable_next(&it);
		iterator it2 = iterable_split_text_tam(line, " ,",string_fix_tam);
		while (iterable_has_next(&it2)) {
			char * num = (char *) iterable_next(&it2);
			parse(e, num, element_type);
			f_key(key,e);
			list_multimap_put(&ms, key,e);
		}
	}
	return ms;
}

set_multimap grupos_set_en_fichero_1(char * file, type * element_type, type * key_type,
		void * f_key(void *out, void * in)) {
	iterator it = iterable_file_string_fix(file);
	set_multimap ms = set_multimap_empty(key_type,element_type);
	char e[element_type->size];
	char key[key_type->size];
	while (iterable_has_next(&it)) {
		char * line = (char *) iterable_next(&it);
		iterator it2 = iterable_split_text_tam(line, " ,",string_fix_tam);
		while (iterable_has_next(&it2)) {
			char * num = (char *) iterable_next(&it2);
			parse(e, num, element_type);
			f_key(key,e);
			set_multimap_put(&ms, key,e);
		}
	}
	return ms;
}


void * parse_g(void * out, char * txt){
	return parse(out,txt,&long_type);
}


long * rest(long * out, long * in) {
	*out = (*in) %5;
	return out;
}

long * id(long * out, long * in) {
	*out = *in;
	return out;
}

void test_esquemas() {
	char mem[3500];
	multiset m = frecuencias_en_fichero_1("ficheros/numeros_2.txt", &long_type);
	printf("1: \n%s\n", multiset_tostring(&m, mem));
	multiset_free(&m);
	list_multimap lm = grupos_list_en_fichero_1("ficheros/numeros_2.txt",&long_type, &long_type, rest);
	iterator it = list_multimap_iterable(&lm);
	printf("2:\n");
	iterable_to_console_sep(&it, "\n", "", "");
	list_multimap_free(&lm);
	iterable_free(&it);
	set_multimap sm = grupos_set_en_fichero_1("ficheros/numeros_2.txt",&long_type, &long_type, rest);
	iterator it2 = set_multimap_iterable(&sm);
	printf("3:\n");
	string_var s= iterable_tostring_sep_big(&it2, "\n", "", "");
	printf("4:\n\%s",string_var_data(&s));
	set_multimap_free(&sm);
	iterable_free(&it2);
	string_var_free(&s,&string_var_type);
}
