/*
 * sorted_list.c
 *
 *  Created on: 1 nov. 2018
 *      Author: migueltoro
 */

#include "../types/list.h"

void swap_in_list(list * ls, int a, int b);


void swap_in_list(list * ls, int a, int b){
	void * tmp = ls->elements[a];
	ls->elements[a] = ls->elements[b];
	ls->elements[b] = tmp;
}

#define tam_default 20

void list_grow(list * list) {
	if (list->size == list->tam) {
		list->tam = 2 * list->tam;
		list->elements = realloc(list->elements,list->tam *sizeof(void *));
		check_not_null(list->elements,__FILE__,__LINE__,"Problemas de memoria en el creciminento de la lista");
	}
}

list list_empty(type * type_element){
//	type * t = type_copy(type_element);
	list r = {false,type_element,0,tam_default,malloc(tam_default*sizeof(void *)),memory_heap_create()};
	return r;
}

list list_empty_tam(type * type_element,int tam){
	list r = {false,type_element,0,tam,malloc(tam*sizeof(void *)),memory_heap_create()};
	return r;
}

list list_of(void * data, int size, type * type_element){
	list r = {false,type_element,0,size,malloc(size*sizeof(void *)),memory_heap_create()};
	char * d = (char *) data;
	for(int i=0; i<size;i++){
		r.elements[i] = d+i*r.type_element->size;
	}
	r.size = size;
	r.tam = size;
	return r;
}

list list_of_int(int n, ...) {
	list r = list_empty(&int_type);
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		int e = va_arg(valist, int);
		list_add(&r, &e);
	}
	va_end(valist);
	return r;
}

list list_of_long(int n, ...) {
	list r = list_empty(&long_type);
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		long e = va_arg(valist, long);
		list_add(&r, &e);
	}
	va_end(valist);
	return r;
}

list list_of_double(int n, ...) {
	list r = list_empty(&double_type);
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		double e = va_arg(valist, double);
		list_add(&r, &e);
	}
	va_end(valist);
	return r;
}

list list_of_string(int n, ...) {
	list r = list_empty(&array_char_type);
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		char * e = va_arg(valist, char *);
		list_add(&r, e);
	}
	va_end(valist);
	return r;
}

list list_of_type(type * type, int n, ...) {
	list r = list_empty(type);
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		void * e = va_arg(valist, void *);
		list_add(&r, e);
	}
	va_end(valist);
	return r;
}


list list_copy(list * in){
	list r = list_empty(in->type_element);
	int n = list_size(in);
	for(int i = 0; i<n; i++){
		list_add(&r,list_get(in,i));
	}
	return r;
}



list list_of_file_type(char * file, type * type) {
	iterator it = file_iterable_pchar(file);
	list ls = list_empty(type);
	char e[type->size];
	while (iterable_has_next(&it)) {
		char * line = (char *) iterable_next(&it);
		parse(e, line, type);
		list_add(&ls,e);
	}
	return ls;
}

list list_of_list_of_file_type(char * file, type * type){
	list res = list_empty(&list_type);
	iterator it1 = file_iterable_pchar(file);
	char e[type->size];
	while(iterable_has_next(&it1)) {
	      char* linea = (char*)iterable_next(&it1);
	      list ls = list_empty(type);
	      iterator it2 = text_to_iterable_pchar(linea, " ,");
	      while(iterable_has_next(&it2)) {
	    	  	char * tx = iterable_next(&it2);
	            parse(e,tx,type);
                list_add(&ls,e);
	      }
	      list_add(&res, &ls);
	}
	return res;
}

void * list_to_array(list * ls, void * array) {
	char * base = (char *) array;
	int n = list_size(ls);
	type * t = ls->type_element;
	int size = t->size;
	for (int i = 0; i < n; i++) {
		void * e = list_get(ls, i);
		copy(base + i * size, e, size);
	}
	return array;
}

 void * list_of_list_to_2_array(list * ls, void * array){
	char * base = (char *) array;
	int nf = list_size(ls);
	type * t = ((list *) list_get(ls,0))->type_element;
	int size = t->size;
	int nc = list_size((list *) list_get(ls,0));
	for(int i = 0; i<nf; i++){
	      list * lsi = (list *) list_get(ls,i);
	      for(int j = 0; j<nc; j++){
	    	  	void * e = list_get(lsi,j);
	    	  	copy(base+(i*nc+j)*size,e,size); //array[i][j] = e;
	      }
	}
	return array;
}

list list_sublist(list * ls, int a, int b){
	check_position_index(a,ls->size,__FILE__,__LINE__);
	check_position_index(b,ls->size,__FILE__,__LINE__);
	check_argument(b>a,__FILE__,__LINE__,"limites inconsistentes a = %d, b = %d",a,b);
	list r = {true,ls->type_element,b-a,b-a,ls->elements+a,ls->hp};
	return r;
}

void * list_get(list * list, const int index){
	check_element_index(index,list->size,__FILE__,__LINE__);
	return list->elements[index];
}

char * list_get_string(list * ls, const int index, char * mem){
	return tostring(list_get(ls,index),mem,ls->type_element);
}

int list_size(list * ls){
	return ls->size;
}

void list_add_pointer(list * list, void * element) {
	check_argument(!list->is_view,__FILE__,__LINE__,"no se puede modificar una vista");
	list_grow(list);
	list->elements[list->size] = element;
	list->size = list->size + 1;
}

void list_add(list * ls, void * element){
	check_argument(!ls->is_view,__FILE__,__LINE__,"no se puede modificar una vista");
	void * e = memory_heap_copy_and_mem(&ls->hp,element,ls->type_element->size);
	list_add_pointer(ls,e);
}

list * list_add_all(list * ls, iterator * it){
	while(iterable_has_next(it)){
		void * e = iterable_next(it);
		list_add(ls,e);
	}
	return ls;
}

list * list_add_list(list * ls, list * add_list){
	iterator it = list_iterable(add_list);
	list * r = list_add_all(ls,&it);
	iterable_free(&it);
	return r;
}

void list_set_pointer(list * list, int index, void * e) {
	check_argument(!list->is_view,__FILE__,__LINE__,"no se puede modificar una vista");
	list->elements[index] = e;
}

void * list_set(list * list, const int index, const void * e) {
	check_argument(!list->is_view,__FILE__,__LINE__,"no se puede modificar una vista");
	void * res = list->elements[index];
	void * cp = memory_heap_copy_and_mem(&list->hp,e,list->type_element->size);
	list_set_pointer(list,index, cp);
	return res;
}


void list_add_left(list * ls, void * element){
	check_argument(!ls->is_view,__FILE__,__LINE__,"no se puede modificar una vista");
	void * e = memory_heap_copy_and_mem(&ls->hp,element,ls->type_element->size);
	list_add_pointer(ls,e);
	void * last_element = list_get(ls,list_size(ls)-1);
	for(int i = list_size(ls)-1; i>0;i--){
		ls->elements[i] = ls->elements[i-1];
	}
	ls->elements[0] = last_element;
}


list list_filter(list * ls, bool (*predicate)(void * e)){
	list r = list_empty(ls->type_element);
	for(int i =0; i< ls->size; i++){
		void * e = list_get(ls,i);
		if(predicate(e)){
			list_add(&r,e);
		}
	}
	return r;
}

list list_map(list * ls, type * type_element, void * (*f)(void * out, void * in)) {
	list r = list_empty(type_element);
	char temp[type_element->size];
	for (int i = 0; i < ls->size; i++) {
		void * e = list_get(ls, i);
		f(temp,e);
		list_add(&r, temp);
	}
	return r;
}


typedef struct{
	list * ls;
	int i;
}dependencies_list;

bool iterable_list_has_next(iterator * current_iterable) {
	dependencies_list * dp = (dependencies_list *) current_iterable->dependencies;
	return dp->i < list_size(dp->ls);
}

void * iterable_list_see_next(iterator * current_iterable){
	dependencies_list * dp = (dependencies_list *) current_iterable->dependencies;
    return list_get(dp->ls,dp->i);
}

void * iterable_list_next(iterator * current_iterable){
	dependencies_list * dp = (dependencies_list *) current_iterable->dependencies;
	int old_i = dp->i;
	dp->i = dp->i +1;
	return list_get(dp->ls,old_i);
}

iterator list_iterable(list * ls){
	dependencies_list dl = {ls,0};
	int size_dl = sizeof(dependencies_list);
	iterator s_list = iterable_create(ls->type_element,iterable_list_has_next,iterable_list_next,iterable_list_see_next,NULL,&dl,size_dl);
	return s_list;
}

char * list_tostring(list * ls, char * mem){
	iterator st = list_iterable(ls);
	iterable_tostring_sep(&st,",","[","]",mem);
	iterable_free(&st);
	return mem;
}

bool list_contains(list * list, const void * e) {
	bool res = false;
	for(int i=0; !res && i<list_size(list); i++) {
		res = equals(list_get(list, i), e, list->type_element);
	}
	return res;
}

bool list_equals(const list * ls1, const list * ls2) {
//	bool mismo_tipo = type_equals(ls1->type_element, ls2->type_element);
	bool res = ls1->size==ls2->size;
	if(res) {
		int i=0;
		while(i<ls1->size && res) {
			void* e1 = list_get(ls1, i);
			void* e2 = list_get(ls2, i++);
			res = equals(e1, e2,ls1->type_element);
		}
	}
	return res;
}

pchar list_delimiters = "[ ,]";

list * list_parse(list * out, char * text) {
	iterator it = text_to_iterable_pchar(text,list_delimiters);
	while(iterable_has_next(&it)){
		void * e = iterable_next(&it);

		list_add(out,e);
	}
	iterable_free(&it);
	return out;
}

list list_parse_s(char * text) {
	list res = list_empty(&array_char_type);
	iterator it = text_to_iterable_pchar(text, list_delimiters);
	while(iterable_has_next(&it)){
			void * e = iterable_next(&it);
			list_add(&res,e);
	}
	iterable_free(&it);
	return res;
}

type list_type = {list_equals, list_tostring, NULL, list_parse, sizeof(list)};


void write_list_to_file(char * file, list * list, char * tostring(const void * source, char * mem)) {
	char mem[256];
	FILE * f = fopen(file, "wt");
	for (int i = 0; i < list->size; i++) {
		fprintf(f, "%s\n", tostring(list_get(list, i), mem));
	}
	fclose(f);
}

list list_of_file(char * file){
	list r = list_empty(&array_char_type);
	iterator f = file_iterable_pchar(file);
	while(iterable_has_next(&f)){
		char * s = iterable_next(&f);
		remove_eol_s(s);
		list_add(&r,s);
	}
	return r;
}

void list_free(list * list) {
	if (list != NULL) {
		type_free(list->type_element);
		free(list->elements);
		memory_heap_free(&list->hp);
	}
}

void list_free_2(list * ls, void (*f)(void * in)) {
	if (ls != NULL) {
		type_free(ls->type_element);
		for (int i = 0; i < ls->size; i++) {
			f(list_get(ls, i));
		}
		list_free(ls);
	}
}


int bs_g(list * ls, int i, int j, void* key, int (*order)(const void * e1, const void * e2)) {
	assert(j >= i);
	int r;
	int k;
	if (j - i == 0) {
		r = -1;
	} else {
		k = (i + j) / 2;
		int r1 = order(key, list_get(ls,k));
		if (r1 == 0) {
			r = k;
		} else if (r1 < 0) {
			r = bs_g(ls, i, k, key, order);
		} else {
			r = bs_g(ls, k + 1, j, key, order);
		}
	}
	return r;
}

int bs(list * ls, void* key, int (*order)(const void * e1, const void * e2)){
		return bs_g(ls,0,ls->size,key,order);
}

void * piv(list * ls, int i, int j){
	void *  pivote = list_get(ls,get_entero_aleatorio(i, j));
	return pivote;
}

bool inv(list * ls, void * p, int a, int b, int c, int (*order)(const void * e1, const void * e2)) {
	bool r = true;
	for (int i = 0; i < ls->size; i++) {
		if (i < a) {
			r = r && order(list_get(ls, i), p) < 0;
		} else if (i>=a && i < b) {
			r = r && order(list_get(ls, i), p) == 0;
		} else if (i >= c) {
			r = r && order(list_get(ls, i), p) > 0;
		}
	}
	return r;
}

int_pair bh(list * ls, void * pivot, int i, int j, int (*order)(const void * e1, const void * e2)) {
	int a =i, b=i, c=j;
	while (c - b > 0) {
		void * elem = list_get(ls,b);
		int r = order(elem, pivot);
		if (r < 0) {
			swap_in_list(ls,a,b);
			a++;
			b++;
		} else if (r > 0) {
			swap_in_list(ls,b,c-1);
			c--;
		} else {
			b++;
		}
	}
	int_pair r = {a, b};
	return r;
}


void * k_esimo_g(list * ls, int k, int i, int j, int (*order)(const void * e1, const void * e2)) {
	assert(j >= i);
	void * r;
	if (j - i <= 1) {
		r = list_get(ls,i);
	} else {
		void * pivote = piv(ls, i, j);
		int_pair p = bh(ls, pivote, i, j, order);
		if(k>=p.a && k <p.b) r = list_get(ls,p.a);
		else if(k<p.a) r = k_esimo_g(ls,k,i,p.a,order);
		else r = k_esimo_g(ls,k,p.b,j,order);
	}
	return r;
}

void * k_esimo(list * ls,int k, int (*order)(const void * e1, const void * e2)){
	int n = list_size(ls);
	return k_esimo_g(ls,k,0,n,order);
}

void basic_sort_g(list * ls, int inf, int sup, int (*order)(const void * e1, const void * e2)) {
	for (int i = inf; i < sup; i++) {
		for (int j = i + 1; j < sup; j++) {
			if (order(list_get(ls, i), list_get(ls, j)) > 0) {
				swap_in_list(ls,i,j);
			}
		}
	}
}

void basic_sort(list * ls, int (*order)(const void * e1, const void * e2)){
	return basic_sort_g(ls,0,ls->size,order);
}

void list_quick_sort_g(list * ls, int i, int j, int (*order)(const void * e1, const void * e2)) {
	assert(j >= i);
	if (j - i <= 4) {
		basic_sort_g(ls, i, j, order);
	} else {
		void * pivote = piv(ls,i,j);
		int_pair p = bh(ls, pivote, i, j, order);
		list_quick_sort_g(ls, i, p.a, order);
		list_quick_sort_g(ls, p.b, j, order);
	}
}

void list_quick_sort(list * ls, int (*order)(const void * e1, const void * e2)){
	list_quick_sort_g(ls,0,ls->size,order);
}

list merge_list(list * ls1, list * ls2, int (*order)(const void * e1, const void * e2)) {
	list ls3 = list_empty(ls1->type_element);
	int s1 = ls1->size;
	int k1 = 0;
	int s2 = ls2->size;
	int k2 = 0;
	while (k1 < s1 || k2 <s2) {
		if (k1 < s1 && k2 < s2) {
			void * e1 = list_get(ls1,k1);
			void * e2 = list_get(ls2,k2);
			if (order(e1, e2) <= 0) {
				list_add_pointer(&ls3,e1);
				k1++;
			} else {
				list_add_pointer(&ls3,e2);
				k2++;
			}
		} else if (k2 == s2) {
			void * e1 = list_get(ls1,k1);
			list_add_pointer(&ls3,e1);
			k1++;
		} else {
			void * e2 = list_get(ls2,k2);
			list_add_pointer(&ls3,e2);
			k2++;
		}
	}
	return ls3;
}

void list_merge_g(list * ls, int i, int k, int j, int (*order)(const void * e1, const void * e2)) {
	int n = j-i;
	int s1 = k-i;
	int k1 = 0;
	int s2 = j-k;
	int k2 = 0;
	int k3 = 0;
	void * tmp[n];
	void * e1;
	void * e2;
	while (k1 < s1 || k2 < s2) {
		if (k1 < s1 && k2 < s2) {
			e1 = list_get(ls, i+k1);
			e2 = list_get(ls, k+k2);
			if (order(e1, e2) <= 0) {
				tmp[k3] = e1;
				k1++;
			} else {
				tmp[k3] = e2;
				k2++;
			}
		} else if (k2 == s2) {
			void * e1 = list_get(ls, i+k1);
			tmp[k3] = e1;
			k1++;
		} else {
			void * e2 = list_get(ls, k+k2);
			tmp[k3] = e2;
			k2++;
		}
		k3++;
	}
	int s = 0;
	while (s<n) {
		ls->elements[i+s] = tmp[s];
		s++;
	}
}


void list_merge_sort_g(list * ls, int i, int j, int (*order)(const void * e1, const void * e2)) {
	assert(j >= i);
	if (j - i <= 4) {
		basic_sort_g(ls, i, j, order);
	} else {
		int k = (j+i)/2;
		list_merge_sort_g(ls, i, k, order);
		list_merge_sort_g(ls, k, j, order);
		list_merge_g(ls,i,k,j,order);
	}
}

void list_merge_sort(list * ls, int (*order)(const void * e1, const void * e2)){
	list_merge_sort_g(ls,0,ls->size,order);
}

type * _list_type;

int _tmp_order(const void * e1, const void * e2){
	return order(e1,e2,_list_type);
}

int_pair bh_naturalorder(list * ls, void * pivot, int i, int j){
	_list_type = ls->type_element;
	return bh(ls,pivot,i,j,_tmp_order);
}
void list_quick_sort_naturalorder(list * ls){
	_list_type = ls->type_element;
	return list_quick_sort(ls,_tmp_order);
}

void basic_sort_naturalorder(list * ls){
	_list_type = ls->type_element;
	return basic_sort(ls,_tmp_order);
}
int bs_naturalorder(list * ls, void * element){
	_list_type = ls->type_element;
	return bs(ls,element,_tmp_order);
}

void list_merge_sort_naturalorder(list * ls){
	_list_type = ls->type_element;
	return list_merge_sort(ls,_tmp_order);
}
list merge_list_naturalorder(list * ls1, list * ls2){
	_list_type = ls1->type_element;
	return merge_list(ls1,ls2,_tmp_order);
}

void * k_esimo_naturalorder(list * ls,int k){
	_list_type = ls->type_element;
	return k_esimo(ls,k,_tmp_order);
}


list complete_list() {
	list ls1 = list_empty(&double_type);
	for (int i = 0; i < 50; i++) {
		double r = 1. * get_entero_aleatorio(0, 100);
		list_add(&ls1, &r);
	}
	return ls1;
}

void test_list_1() {
	char mem[3000];
	list ls1 = complete_list();
	list ls2 = complete_list();
	double a = -340.51;
	list_add(&ls2, &a);
	list_add(&ls1, &a);
	list ls2s = list_sublist(&ls2,10,20);
	list_quick_sort_naturalorder(&ls1);
	list_quick_sort_naturalorder(&ls2);
	char * s = list_tostring(&ls1, mem);
	printf("ls1 = %s\n", s);
	s = list_tostring(&ls2, mem);
	printf("ls2 = %s\n", s);
	s = list_tostring(&ls2s, mem);
	printf("ls2s = %s\n", s);
	list ls3 = merge_list_naturalorder(&ls1, &ls2s);
	s = list_tostring(&ls3, mem);
	printf("ls3 = %s\n", s);
	double a3 = 34000.55;
	list_add_left(&ls3,&a3);
	s = list_tostring(&ls3, mem);
	printf("2 -- ls3 = %s\n", s);
	double d[] = {2.,3.,4.5,5.7,8.9,-3.1};
	list ls4 = list_of(d,6,&double_type);
	s = list_tostring(&ls4, mem);
	printf("ls4 = %s\n", s);
	pchar as[] = {"Hola","Juan","Antonio","Pepe","Juan","Diaz"};
	list ls5 = list_of(as,6,&array_char_type);
	list_quick_sort_naturalorder(&ls5);
	s = list_tostring(&ls5,  mem);
	printf("ls5 = %s\n", s);
	list ls = list_of_file("ficheros/prueba.txt");
	char * s1 = list_get(&ls,2);
	printf("s1 = %s\n", s1);
	s = list_tostring(&ls,mem);
	printf("ls1 = %s\n", s);
	list_free(&ls5);
	list_free(&ls);
	list_free(&ls1);
	list_free(&ls2);
	list_free(&ls3);
	list_free(&ls4);
}

void test_list_2(void) {
	puts("\n\n------------------------------ TEST List2 ------------------------------");
	int size = 10;
	int enteros[size];
	for(int i=0; i<size; i++) {
		*(enteros+i) = i;
	}
	list ls1 = list_of(enteros, size, &int_type);
	list ls2 = list_of(enteros, size, &int_type);

	char mem1[100];
	char mem2[100];
	printf("\n�Son iguales ls1 y ls2?: %s; ls1=%s; ls2=%s\n", MSG_BOOL(list_equals(&ls1, &ls2)),
			list_tostring(&ls1, mem1), list_tostring(&ls2, mem2));

	int valor = 11;
	list_set(&ls2, 9, &valor);
	printf("�Son iguales ls1 y ls2?: %s; ls1=%s; ls2=%s\n", MSG_BOOL(list_equals(&ls1, &ls2)),
			list_tostring(&ls1, mem1), list_tostring(&ls2, mem2));

	list ls3 = list_empty(&double_type);
	for(int i=0; i<10; i++) {
		double x = (i+1.)/(i+2.);
		list_add(&ls3, &x);
	}

	printf("�Son iguales ls1 y ls3?: %s; ls1=%s; ls3=%s\n", MSG_BOOL(list_equals(&ls1, &ls3)),
			list_tostring(&ls1, mem1), list_tostring(&ls3, mem2));

	printf("�Contiene ls1 el valor %d?: %s\n", valor, MSG_BOOL(list_contains(&ls1, &valor)));
	printf("�Contiene ls2 el valor %d?: %s\n", valor, MSG_BOOL(list_contains(&ls2, &valor)));
	printf("�Contiene ls3 el valor %d?: %s\n", valor, MSG_BOOL(list_contains(&ls3, &valor)));
	double valor2 = 0.75;
	printf("�Contiene ls3 el valor %.2lf?: %s\n", valor2, MSG_BOOL(list_contains(&ls3, &valor2)));
	valor2 = 1.5;
	list_set(&ls3, 9, &valor2);
	printf("Ahora ls3 es: %s;\n", list_tostring(&ls3, mem1));
	printf("�Contiene ls3 el valor %.2lf?: %s\n", valor2, MSG_BOOL(list_contains(&ls3, &valor2)));

	pchar texto1 = "[Estas, son, pruebas, de, nuevas, funciones, para, el, tipo, list]";
	list ls4 = list_empty(&array_char_type);
	list_parse(&ls4, texto1);
	printf("Dada la cadena \"%s\", list_parse ha obtenido la lista:\n%s\n", texto1, list_tostring(&ls4, mem1));

	pchar texto2 = "[Creo, que, todo, funciona, bien]";
	list ls5 = list_parse_s(texto2);
	printf("Dada la cadena \"%s\", list_parse_s ha obtenido la lista:\n%s\n", texto2, list_tostring(&ls5, mem1));
}

void test_list_3() {
	char mem[1000];
	list ls = list_of_file("ficheros/numeros.txt");
	list lm = list_map(&ls,&int_type,int_type.tp.parse);
	list_tostring(&lm, mem);
	printf("1: %s\n", mem);
	list ls2 = list_of_int(5, 6, 7, 8, 9, 10);
	list_tostring(&ls2, mem);
	printf("2: %s\n", mem);
	int q = 11;
	list_add(&ls2,&q);
	list_tostring(&ls2, mem);
	printf("3: %s\n", mem);
	list cp = list_copy(&ls2);
	iterator it = list_iterable(&cp);
	list_add_all(&ls2,&it);
	list_add_list(&ls2,&cp);
	list_tostring(&ls2, mem);
	printf("4: %s\n", mem);
	list ls3 = list_of_double(5, 6., 7., 8., 9., 10.);
	list_tostring(&ls3, mem);
	printf("5: %s\n", mem);
	list ls4 = list_of_string(5, "6", "7", "8", "9", "10");
	list_tostring(&ls4, mem);
	printf("6: %s\n", mem);
	list ls5 = list_of_string(5, "(2.,3.)", "(2.,3.)", "(2.,3.)", "(2.,3.)", "(2.,3.)");
	list ls6 = list_map(&ls5,&punto_type,punto_type.tp.parse);
	list_tostring(&ls6, mem);
	printf("7: %s\n", mem);
	int a[] = {6, 7, 8, 9, 10};
	int b = 14;
	int c = 17;
	list ls7 = list_of_type(&int_type,7,a,a+1,a+2,a+3,a+4,&b,&c);
	list_tostring(&ls7, mem);
	printf("8: %s\n", mem);
}

void test_list_4() {
	char mem[3000];
	list ls = list_of_file_type("ficheros/numeros.txt", &int_type);
	list_tostring(&ls, mem);
	printf("%s\n", mem);
	list ls2 = list_of_list_of_file_type("ficheros/datos_entrada.txt",&long_type);
	list_tostring(&ls2, mem);
	printf("%s\n", mem);
	int n = list_size(&ls2);
	int m = list_size(list_get(&ls2,0));
	printf("%d,%d\n",n,m);
	long a[n][m];
	list_of_list_to_2_array(&ls2, a);
	int i, j;
	for (i = 0; i < 49; i++) {
		for (j = 0; j < 5; j++)
			printf("%4d ", a[i][j]);
		printf("\n");
	}
}

void test_list_5() {
	char mem[5000];
	new_rand();
	list ls1 = list_empty(&double_type);
	for (int i = 0; i < 100; i++) {
		double r = 1. * get_entero_aleatorio(0, 1000);
		list_add(&ls1,&r);
	}
	int k = 53;
	double d = *(double*) k_esimo(&ls1,k,double_type.tp.order);
	printf("k = %d, k_esimo = %.2f\n",k,d);
	list_merge_sort(&ls1, double_type.tp.order);
	iterator it1 = list_iterable(&ls1);
	iterator it2 = iterable_enumerate(&it1);
	char * s = iterable_tostring(&it2, mem);
	printf("ls1 = %s\n", s);

}


void test_list_6() {
	char mem[1000];
	pchar texto1 =
			"[Estas, son, pruebas, de, nuevas, funciones, para, el, tipo, list]";
	list ls = list_empty(&array_char_type);
	list_parse(&ls, texto1);
	printf("%s\n", list_tostring(&ls, mem));
	pchar_copy(texto1,
			"{Estas; son; pruebas; de; nuevas; funciones; para; el; tipo; list}");
	ls = list_empty(&array_char_type);
	pchar_copy(list_delimiters, "{ ;}");
	list_parse(&ls, texto1);
	printf("%s\n", list_tostring(&ls, mem));
	pchar_copy(texto1,
			"{(3.,4.); (5.6,-7.8); (5.6,-7.8); (5.6,-7.8); (5.6,-7.8); (5.6,-7.85); (5.6,-17.8); (5.,-7.8); (5.6,7.9); (-5.6,-7.8)}");
	ls = list_empty(&array_char_type);
	pchar_copy(list_delimiters, "{ ;}");
	list_parse(&ls, texto1);
	printf("%s\n", list_tostring(&ls, mem));
	pchar_copy(texto1,
			"{[(23.,5.)_(4.,7.)];[(5.6,-7.8)_(5.6,-7.8)]}");
	ls = list_empty(&array_char_type);
	pchar_copy(list_delimiters, "{ ;}");
	pchar_copy(pair_delimiters, "[ _]");
	list_parse(&ls, texto1);
	tmp_type = generic_type_2(&pair_mem_type,&punto_type,&punto_type);
	printf("\n%d\n",tmp_type->num_types);
	list r = list_map(&ls,tmp_type,parse_type);
	printf("3: %s\n", list_tostring(&r, mem));
	list_quick_sort_naturalorder(&r);
	printf("3: %s\n", list_tostring(&r, mem));
	memory_heap_free(&pair_memory_heap);
}
