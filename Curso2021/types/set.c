/*
 * set.c
 *
 *  Created on: 28 abr. 2019
 *      Author: migueltoro
 */

#include "../types/set.h"
#include "../types/list.h"
#include "../types/math2.h"


set set_empty(type * type_element){
	map t = map_empty(type_element,&null_type);
	return t;
}

set set_of(list * ls){
	set st = set_empty(ls->type);
	for(int i =0;i <ls->size;i++){
		set_add(&st,list_get(ls,i));
	}
	return st;
}

void set_add(set * st, void * element){
	map_put(st,element,NULL);
}

void set_remove(set * st, void * element) {
	map_remove(st,element);
}

void set_add_all(set * st, iterator * it){
	while(iterable_has_next(it)){
		void * e = iterable_next(it);
		set_add(st,e);
	}
}

int set_size(set * st){
	return map_size(st);
}

bool set_contains(set * st, void * element){
	return map_contains_key(st,element);
}

iterator set_iterable(set * st){
	iterator r = map_items_iterable(st);
	iterator im = iterable_map(iterable_copy_new(&r),type_copy(st->key_type,NULL),pair_key);
	return im;
}

set set_map(set * st, type * type_out, void * (*f)(void * out, void * in)){
	iterator it = set_iterable(st);
	set r = set_empty(type_out);
	char mem[type_out->size];
	while(iterable_has_next(&it)){
		void * e = iterable_next(&it);
		void * t = f(mem,e);
		set_add(&r,t);
	}
	return r;
}

set set_filter(set * st, bool (*p)(void * in)) {
	iterator it = set_iterable(st);
	set r = set_empty(st->key_type);
	while (iterable_has_next(&it)) {
		void * e = iterable_next(&it);
		if(p(e)) set_add(&r,e);
	}
	return r;
}

char * set_tostring(set * st, char * mem){
	iterator it = set_iterable(st);
	string_var s = iterable_tostring_sep_var(&it,",","{","}");
	st->st = s;
	iterable_free(&it);
	return s.data;
}

list set_tolist(const set * s) {
	list res = list_empty(s->key_type);
	iterator it = set_iterable(s);
	while(iterable_has_next(&it)) {
		list_add(&res, iterable_next(&it));
	}
	list_quick_sort_naturalorder(&res);
	return res;
}

bool set_equals(const set * s1, const set * s2) {
//	bool mismo_tipo = type_equals(&s1->type_element, &s2->type_element);
	bool res = set_size(s1)==set_size(s2);
//	bool res = mismo_tipo && mismo_size;
	if(res) {
		list ls1 = set_tolist(s1);
		list ls2 = set_tolist(s2);
		res = list_equals(&ls1, &ls2);
	}
	return res;
}

set * set_parse(set * out, char * text) {
	iterator it = iterable_split_text_tam(text, "{ ,}",string_fix_tam);
	char tmp[out->key_type->size];
	tmp_type = out->key_type;
	while(iterable_has_next(&it)) {
		void * txt = iterable_next(&it);
		parse_st(tmp,txt);
		set_add(out, tmp);
	}
	iterable_free(&it);
	return out;
}

set set_parse_s(char * text) {
	set res = set_empty(&string_fix_type);
	iterator it = iterable_split_text_tam(text, "{ ,}",string_fix_tam);
	while(iterable_has_next(&it)) {
		set_add(&res, iterable_next(&it));
	}
	iterable_free(&it);
	return res;
}

set set_copy(set *s) {
	set res = set_empty(s->key_type);
	iterator it = set_iterable(s);
	while (iterable_has_next(&it)) {
		set_add(&res, iterable_next(&it));
	}
	return res;
}

set * set_copy_p(set *s){
	set r = set_copy(s);
	set * r2 = heap_copy(&r,NULL,sizeof(set));
	return r2;
}

type set_type = {"set",set_equals, set_tostring, NULL, set_parse, set_free,
		copy_new_0,copy_0,sizeof(set),1,NULL};


void set_free(set * st){
	map_free(st);
}

set complete_set() {
	int tam = 50;
	set st = set_empty(&double_type);
	for (int i = 0; i < tam; i++) {
		double a2 = double_aleatorio(0, 1000) / 2;
		set_add(&st, &a2);
	}
	return st;
}

void test_set_1() {
	char mem[1000];
	new_rand();
	set st = complete_set();
	printf("%d\n", set_size(&st));
	set_tostring(&st, mem);
	printf("%s\n", mem);
	set_free(&st);
}

void test_set_2() {
	puts("\n\n------------------------------ TEST Set2 ------------------------------");

	int size = 10;
	int enteros[size];

	for (int i = 0; i < size; i++) {
		enteros[i] = i;
	}
	list ls = list_of(enteros, size, &int_type);
	set s1 = set_of(&ls);
	set s2 = set_of(&ls);

	char mem1[512];
	char mem2[512];
	printf("\n¿Son iguales s1 y s2?: %s; s1=%s; s2=%s\n",
			MSG_BOOL(set_equals(&s1, &s2)), set_tostring(&s1, mem1),
			set_tostring(&s2, mem2));

	int valor = 11;
	set_add(&s2, &valor);
	printf("¿Son iguales s1 y s2?: %s; s1=%s; s2=%s\n",
			MSG_BOOL(set_equals(&s1, &s2)), set_tostring(&s1, mem1),
			set_tostring(&s2, mem2));

	char * texto1 = "{Estas, son, pruebas, de, nuevas, funciones, para, el, tipo, set}";
	set s4 = set_empty(&string_fix_type);
	set_parse(&s4, texto1);
	printf("Dada la cadena \"%s\", set_parse ha obtenido el conjunto:\n%s\n",
			texto1, set_tostring(&s4, mem1));

	char * texto2 = "{Creo, que, todo, todo, todo, funciona, bien, bien}";
	set s5 = set_parse_s(texto2);
	printf("Dada la cadena \"%s\", set_parse_s ha obtenido la lista:\n%s\n",
			texto2, set_tostring(&s5, mem1));
}

void test_set_3() {
	char mem[1000];
	iterator it = iterable_range_long(3, 100, 5);
	iterator it2 = *iterable_copy_new(&it);
	set s = set_empty(&long_type);
	set_add_all(&s,&it);
	set_add_all(&s,&it2);
	set s2 = set_map(&s,&long_type,square_long_f);
	printf("%s\n",set_tostring(&s2, mem));
	set s3 = set_filter(&s,es_primo_f);
	printf("%s\n",set_tostring(&s3, mem));
}
