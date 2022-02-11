/*
 * accumulators.c
 *
 *  Created on: 7 jul. 2019
 *      Author: migueltoro
 */

#include "../types/accumulators.h"

void * accumulate_left(iterator * st, void * base, bool (*add)(void * out, const void * e)) {
	bool f = false;
	while (iterable_has_next(st) && !f) {
		void * e = iterable_next(st);
		f = add(base, e);
	}
	return base;
}


bool accumulate_right_private(iterator * st, void * base, bool (*add)(void * out, const void * e)) {
	bool f;
	if (iterable_has_next(st)) {
		char se[st->type->size];
		void * e = iterable_next(st);
		copy_size(se, e, st->type->size);
		f = accumulate_right_private(st, base, add);
		if(!f) f = add(base, se);
	}
	return f;
}

void * accumulate_right(iterator * st, void * base, bool (*add)(void * out, const void * e)) {
	accumulate_right_private(st,base,add);
	return base;
}


void * reduce_left(iterator * st, void * base, bool (*add)(void * out, const void * e)) {
	bool first = true;
	bool f = false;
	while (iterable_has_next(st) && !f) {
		void * e = iterable_next(st);
		if (first) {
			copy_size(base, e, st->type->size);
			first = false;
		} else if(!f) {
			f = add(base, e);
		}
	}
	if(first) return NULL;
	else return base;
}


bool reduce_right_private(iterator * st, void * base, bool (*add)(void * out, const void * e), bool * first) {
	bool f = false;
	if (iterable_has_next(st)) {
		char se[st->type->size];
		void * e = iterable_next(st);
		copy_size(se, e,st->type->size);
		f = reduce_right_private(st, base, add, first);
		if (*first) {
			copy_size(base,e,st->type->size);
			*first = false;
		} else if(!f){
			f = add(base, se);
		}
	}
	return f;
}

void * reduce_right(iterator * st, void * base, bool (*add)(void * out, const void * e)) {
	bool first = true;
	reduce_right_private(st, base, add, &first);
	if(first) return NULL;
	else return base;
}

list iterable_to_list(iterator * st){
	list r = list_empty(st->type);
	while(iterable_has_next(st)){
		char * s = iterable_next(st);
		list_add(&r,s);
	}
	return r;
}

set iterable_to_set(iterator * st) {
	set r = set_empty(st->type);
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		set_add(&r, e);
	}
	return r;
}

multiset iterable_to_multiset(iterator * st) {
	multiset r = multiset_empty(st->type);
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		multiset_add(&r, e);
	}
	return r;
}


multiset iterable_to_multiset_groups(iterator * st, type * key_type, void * (*f_key)(void * out, void * in)){
	char mem[key_type->size];
	multiset ms = multiset_empty(key_type);
	while(iterable_has_next(st)){
		void * e = iterable_next(st);
		void * key = f_key(mem,e);
		multiset_add(&ms,key);
	}
	return ms;
}

list_multimap iterable_to_list_multimap(iterator * items) {
	type * t =  items->type;
	list_multimap lm = list_multimap_empty(types(t,0),types(t,1));
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		list_multimap_put(&(lm.hash_table), &p->key, &p->value);
	}
	return lm;
}



set_multimap iterable_to_set_multimap(iterator * items) {
	type * t = items->type;
	set_multimap lm = set_multimap_empty(types(t,0),types(t,1));
	while (iterable_has_next(items)) {
		pair * p = (pair *) iterable_next(items);
		set_multimap_put(&(lm.hash_table), &p->key, &p->value);
	}
	return lm;
}

map iterable_grouping_reduce(iterator *st, type * key_type,
		void* (*f_key)(void *out, void *in),
		bool (*add)(void *out, const void *e)) {
	type *value_type = st->type;
	char mem_key[key_type->size];
	char mem_value[value_type->size];
	map ht = map_empty(key_type, value_type);
	while (iterable_has_next(st)) {
		void *e = iterable_next(st);
		void *key = f_key(mem_key,e);
		void * a = map_get(&ht, key);
		if(a == NULL){
			map_put(&ht, key,e);
		} else {
			copy_size(mem_value,a,value_type->size);
			add(mem_value,e);
			map_put(&ht, key, mem_value);
		}
	}
	return ht;
}

map iterable_grouping_reduce_map(iterator *st, type *key_type,
		type *value_type, void* (*f_key)(void *out, void *in),
		bool (*add)(void *out, const void *e),
		void* (*f_map)(void *out, void *in)) {

	char mem_key[key_type->size];
	char mem_value[value_type->size];
	char mem_value_a[value_type->size];
	map ht = map_empty(key_type, value_type);
	while (iterable_has_next(st)) {
		void *e = iterable_next(st);
		void *key = f_key(mem_key, e);
		void *a = map_get(&ht, key);
		void *t = f_map(mem_value,e);
		if (a == NULL) {
			map_put(&ht,key,t);
		} else {
			copy_size(mem_value_a, a, value_type->size);
			add(mem_value_a, t);
			map_put(&ht, key,mem_value_a);
		}
	}
	return ht;
}

list_multimap iterable_grouping_list(iterator * st, type * key_type, void * (*f_key)(void * out, void * in)){
	char mem[key_type->size];
	type * value_type = st->type;
	list_multimap lm = list_multimap_empty(key_type,value_type);
	while(iterable_has_next(st)){
		void * e = iterable_next(st);
		void * key = f_key(mem,e);
		list_multimap_put(&lm,key,e);
	}
	return lm;
}

list_multimap iterable_grouping_list_map(iterator *st, type *key_type,
		type *value_type, void* (*f_key)(void *out, void *in),
		void* (*f_map)(void *out, void *in)) {
	char mem_key[key_type->size];
	char mem_value[value_type->size];
	list_multimap lm = list_multimap_empty(key_type, value_type);
	while (iterable_has_next(st)) {
		void *e = iterable_next(st);
		void *key = f_key(mem_key, e);
		void *t = f_map(mem_value,e);
		list_multimap_put(&lm, key, t);
	}
	return lm;
}

set_multimap iterable_grouping_set(iterator * st, type * key_type, void * (*f_key)(void * out, void * in)){
	char mem[key_type->size];
	type * value_type = st->type;
	set_multimap sm = set_multimap_empty(key_type,value_type);
	while(iterable_has_next(st)){
		void * e = iterable_next(st);
		void * key = f_key(mem,e);
		set_multimap_put(&sm,key,e);
	}
	return sm;
}


set_multimap iterable_grouping_set_map(iterator *st, type *key_type,
		type *value_type, void* (*f_key)(void *out, void *in),
		void* (*f_map)(void *out, void *in)) {
	char mem_key[key_type->size];
	char mem_value[value_type->size];
	set_multimap lm = set_multimap_empty(key_type, value_type);
	while (iterable_has_next(st)) {
		void *e = iterable_next(st);
		void *key = f_key(mem_key, e);
		void *t = f_map(mem_value,e);
		set_multimap_put(&lm, key, t);
	}
	return lm;
}


estadisticos estadisticos_ini = {0,0,0,-DBL_MAX,DBL_MAX,0,0,0};

void * add_estadisticos(estadisticos * est, const void * in){
	double e = *(double *) in;
	est->num = est->num+1;
	est->sum = est->sum + e;
	est->sum_cuadrados = est->sum_cuadrados + e*e;
	est->max = MAX(est->max,e);
	est->min = MIN(est->min,e);
	return est;
}

void * result_estadisticos(estadisticos * est_out, const estadisticos * est_in){
	est_out->num = est_in->num;
	est_out->sum = est_in->sum;
	est_out->sum_cuadrados = est_in->sum_cuadrados;
	est_out->max = est_in->max;
	est_out->min = est_in->min;
	est_out->media = est_in->sum/est_in->num;
	est_out->varianza = est_in->sum_cuadrados/est_in->num - est_out->media*est_out->media;
	est_out->desviacion_tipica = sqrt(est_out->varianza);
	return est_out;
}

char * estadisticos_tostring(void * in, char * mem){
	estadisticos e = *(estadisticos *) in;
	sprintf(mem,
	  "(num = %d,sum = %.2lf,sum_cuadrados = %.2lf,max = %.2lf,min = %.2lf,media = %.2lf,varianza = %.2lf,desviacion_tipica = %.2lf)",
	  e.num,e.sum,e.sum_cuadrados,e.max,e.min,e.media,e.varianza,e.desviacion_tipica);
	return mem;
}

void * iterable_min(iterator * st, int (*comparator)(const void * out, const void * in)){
	type * t = st->type;
	void * minvalue = malloc(t->size);
	bool first = true;
	while(iterable_has_next(st)){
		void * next = iterable_next(st);
		if(first) {
			copy_size(minvalue, next, t->size);
			first = false;
		} else if(comparator(next,minvalue) < 0) {
			copy_size(minvalue, next, t->size);
		}
	}
	if(first) {
		free(minvalue);
		minvalue = NULL;
	}
	return minvalue;
}

void* iterable_min_naturalorder(iterator *st) {
	type * t = st->type;
	void * minvalue = malloc(t->size);
	bool first = true;
	while (iterable_has_next(st)) {
		void *next = iterable_next(st);
		if (first) {
			copy_size(minvalue, next, t->size);
			first = false;
		} else if (order(next, minvalue,t) < 0) {
			copy_size(minvalue, next, t->size);
		}
	}
	if(first) {
			free(minvalue);
			minvalue = NULL;
	}
	return minvalue;
}



void * iterable_max(iterator * st,int (*comparator)(const void * out, const void * in)) {
	type * t = st->type;
	void * maxvalue = malloc(t->size);
	bool first = true;
	while (iterable_has_next(st)) {
		void * next = iterable_next(st);
		if (first) {
			copy_size(maxvalue, next, t->size);
			first = false;
		} else if (comparator(next, maxvalue) > 0) {
			copy_size(maxvalue, next, t->size);
		}
	}
	if(first) {
			free(maxvalue);
			maxvalue = NULL;
	}
	return maxvalue;
}

void * iterable_max_naturalorder(iterator * st) {
	type * t = st->type;
	void * maxvalue = malloc(t->size);
	bool first = true;
	while (iterable_has_next(st)) {
		void * next = iterable_next(st);
		if (first) {
			copy_size(maxvalue, next, t->size);
			first = false;
		} else if (order(next, maxvalue,t) > 0) {
			copy_size(maxvalue, next, t->size);
		}
	}
	if(first) {
			free(maxvalue);
			maxvalue = NULL;
	}
	return maxvalue;
}

bool iterable_all(iterator * st, bool (*p)(const void * in)){
	bool r = true;
	while(iterable_has_next(st) && r) {
		void * e = iterable_next(st);
		r = p(e);
	}
	return r;
}

bool iterable_any(iterator * st, bool (*p)(const void * in)) {
	bool r = false;
	while (iterable_has_next(st) && !r) {
		void * e = iterable_next(st);
		r = p(e);
	}
	return r;
}

void * iterable_first(iterator * st, bool (*p)(const void * in)) {
	void * r = NULL;
	while (iterable_has_next(st) && r == NULL) {
		void * e = iterable_next(st);
		if(p(e)) r = e;
	}
	return r;
}

void * iterable_last(iterator * st, bool (*p)(const void * in)) {
	void * r = NULL;
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		if(p(e)) r = e;
	}
	return r;
}

double iterable_sum(iterator * st) {
	double r = 0;
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		r = r+*(double *) e;
	}
	return r;
}

int iterable_size(iterator * st) {
	int r = 0;
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		r = r+1;
	}
	return r;
}

Number numeric_type(type *t) {
	Number r;
	int sz = t->size;
	if(sz == int_type.size) r = INTEGER;
	else if(sz == double_type.size) r = DOUBLE;
	else if(sz == long_type.size) r = LONG;
	else if(sz == float_type.size) r = FLOAT;
	else check_argument(false,__FILE__,__LINE__, "No es un tipo numerico");
	return r;
}

double iterable_average(iterator * st) {
	double r = 0;
	int n = 0;
	Number nm = numeric_type(st->type);
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		switch(nm) {
		case INTEGER: r +=  *(int *) e; break;
		case DOUBLE: r +=  *(double *) e; break;
		case LONG: r +=  *(long *) e; break;
		case FLOAT: r +=  *(float *) e; break;
		}
		n = n+1;
	}
	check_argument(n != 0, __FILE__, __LINE__,"el número de elementos es cero y es %d",n);
	return r/n;
}

int iterable_num_differents(iterator * st){
	set s = iterable_to_set(st);
	return set_size(&s);
}

bool iterable_all_differents(iterator * st) {
	int n = 0;
	set r = set_empty(st->type);
	while (iterable_has_next(st)) {
		void * e = iterable_next(st);
		set_add(&r, e);
		n = n + 1;
	}
	int n2 = set_size(&r);
	set_free(&r);
	return n == n2;
}

long * resto17(long * out, long * in){
	*out = (*in)%17;
	return out;
}

bool esmultiplo17(const long *in){
	return (*in)%17 == 0;
}

bool esmultiplo5(const long *in){
	return (*in)%5 == 0;
}

bool esmultiplo44(const long *in){
	return (*in)%44 == 0;
}


double * _random(double * out, long * in);

void test_accumulators_0(){
	char mem[4000];
	iterator st = iterable_range_long(4,500,3);
	int n = iterable_size(&st);
	printf("%d\n",n);
}

void test_accumulators_1(){
	char mem[4000];
	iterator st;
	st = iterable_range_long(4,500,3);
	multiset ht = iterable_to_multiset_groups(&st,resto17,&long_type);
	char * s = multiset_tostring(&ht,mem);
	printf("1:  \n%s\n\n",s);
	st = iterable_range_long(4,500,3);
	list_multimap ht2 = iterable_grouping_list(&st,resto17,&long_type);
	char * s2 = list_multimap_tostring(&ht2,mem);
	printf("2:  \n%s\n\n",s2);
	st = iterable_range_long(4,500,3);
	set_multimap sm = iterable_grouping_set(&st,resto17,&long_type);
	set_multimap_tostring(&sm,mem);
	printf("2.1:  \n%s\n\n",mem);
	st = iterable_range_double(4,500,3);
	estadisticos est= estadisticos_ini;
	estadisticos est_r;
	void * e = accumulate_left(&st,&est,add_estadisticos);
	printf("3:  \n%s\n\n",estadisticos_tostring(e,mem));
	st = iterable_range_double(4,500,3);
	st = iterable_range_long(4,500,3);
	bool r = iterable_all(&st,esmultiplo17);
	printf("5:  \n%s\n",bool_tostring(&r,mem,NULL));
	st = iterable_range_long(4,500,3);
	r = iterable_any(&st,esmultiplo17);
	printf("6:  \n%s\n",bool_tostring(&r,mem,NULL));
	st = iterable_range_long(7,500,3);
	long r1 = *(long *) iterable_first(&st,esmultiplo17);
	printf("7:  \n%ld\n",r1);
	iterator rr = iterable_range_long(0, 500, 2);
	iterator rr1 = iterable_map(&rr, &double_type, _random);
	set ms = iterable_to_set(&rr1);
	s = set_tostring(&ms, mem);
	printf("12: %s\n", s);
	rr = iterable_range_long(0, 500, 2);
	rr1 = iterable_map(&rr,&double_type,_random);
	multiset mms = iterable_to_multiset(&rr1);
	s = multiset_tostring(&mms, mem);
	printf("13: %s\n",s);
}

void test_accumulators_2(){
	iterator st = iterable_range_long(4,100,13);
	long r;
	iterable_max(&st,long_type.order);
	printf("%ld\n",r);
	st = iterable_range_long(4,100,13);
	r = *(long*) iterable_first(&st,esmultiplo5);
    printf("%ld\n",r);
    st = iterable_range_long(4,100,13);
    r = *(long*) iterable_last(&st,esmultiplo5);
    printf("%ld\n",r);
    st = iterable_range_long(4,100,13);
    int n = iterable_num_differents(&st);
    printf("%d\n",n);
    st = iterable_range_long(4,100,13);
    bool b = iterable_all_differents(&st);
    printf("%s\n",MSG_BOOL(b));
}

void test_accumulators_3(char * file) {
	string_fix_tam = 100;
	iterator git1 = iterable_file_string_fix(file);
	iterator git2 = iterable_filter(&git1, string_fix_not_all_space);
	string_fix_tam = 20;
	type t = string_fix_type_of_tam(string_fix_tam);
	iterator gmap = iterable_flatmap(&git2,type_copy(&t,NULL),text_to_iterable_string_fix_function);
	set st = iterable_to_set(&gmap);
	int n = set_size(&st);
	printf("%d\n", n);
	set_free(&st);
	iterables_free(3, &git1, &git2, &gmap);
}

int * flen(int * len, char * line){
	*len = strlen(line);
	return len;
}

int cmp(char * lin1, char * lin2){
	int r1 = strlen(lin1);
	int r2 = strlen(lin2);
	return order(&r1,&r2,&int_type);
}

bool max_len(char * p1, char * p2){
	int r1 = strlen(p1);
	int r2 = strlen(p2);
	int r = order(&r1,&r2,&int_type);
	if(r < 0) strcpy(p1,p2);
	return false;
}

void test_accumulators_4(char * file) {
	string_fix_tam = 100;
	iterator g1 = iterable_file_string_fix(file);
	string_fix_tam = 20;
	char max[string_fix_tam];
	type t = string_fix_type_of_tam(string_fix_tam);
	iterator g3 = iterable_flatmap(&g1,type_copy(&t,NULL),text_to_iterable_string_fix_function);
	char * r = (char *) iterable_max(&g3,cmp);
	printf("%s,%d\n",r,strlen(r));
}

void test_accumulators_5(char * file) {
	string_fix_tam = 100;
	iterator g1 = iterable_file_string_fix(file);
	iterator g2 = iterable_filter(&g1, string_fix_all_space);
	int n = 0;
	for(;iterable_has_next(&g2);iterable_next(&g2)){
		n++;
	}
	printf("%d\n",n);
}

bool str_cat(char * out, char * in){
	strcat(out,in);
	return false;
}

bool str_var_cat(char * out, char * in){
	string_var_add_string_fix(out,in);
	return false;
}

void test_accumulators_6() {
	char mem[4000];
	double r7;
	iterator st = iterable_range_long(7, 500, 3);
	reduce_left(&st, &r7, long_sum);
	printf("1:  \n%ld\n", r7);
	st = iterable_range_long(7, 500, 3);
	reduce_right(&st, &r7, long_sum);
	printf("1.1:  \n%ld\n", r7);
	st = iterable_range_long(7, 500, 3);
	reduce_left(&st, &r7, long_max);
	printf("1:  \n%ld\n", r7);
	st = iterable_range_long(7, 500, 3);
	reduce_right(&st, &r7, long_max);
	printf("2.1:  \n%ld\n", r7);
	char text[] =
			"El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	string_var emp = string_var_empty();
	void *sr = accumulate_left(&p3, &emp, str_var_cat);
	printf("10: %s\n", string_var_tostring(sr, mem, NULL));
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	emp = string_var_empty();
	sr = accumulate_right(&p3, &emp, str_var_cat);
	printf("11: %s\n", string_var_tostring(sr, mem, NULL));
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	char b[10000];
	void *r2 = accumulate_left(&p3, b, str_cat);
	printf("12: %s\n", b);
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	b[0] = '\0';
	r2 = accumulate_right(&p3, b, str_cat);
	printf("13: %s\n", b);
	printf("14: %d\n", strlen(b));
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	type t = string_fix_type_of_tam(20);
	list ls = list_empty(&t);
	r2 = accumulate_left(&p3,&ls,list_add);
	printf("15: %s\n", list_tostring(&ls,mem));
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	list_clear(&ls);
	r2 = accumulate_right(&p3,&ls,list_add);
	printf("16: %s\n",list_tostring(&ls,mem));
	printf("17: %d", list_size(&ls));
}

void test_accumulators_7() {
	iterator it1 = iterable_primos(2,3);
	iterator it2 = iterable_map(&it1,&long_type,square_long_f);
	long suma;
	long s = *(long*) reduce_left(&it2, &suma,long_sum);
	printf("%d\n",s);
	it1 = iterable_primos(2,3);
	it2 = iterable_map(&it1,&long_type,square_long_f);
	s = *(long*) reduce_right(&it2, &suma,long_sum);
	printf("%d\n",s);
}

void test_accumulators_8() {
	char text[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	char b[10000];
	void *r2 = accumulate_left(&p3, b, str_cat);
	printf("1: %s\n", b);
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	b[0] = '\0';
	r2 = accumulate_right(&p3, b, str_cat);
	printf("2: %s\n", b);
}

void test_accumulators_9() {
	char text[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	char p[20];
	iterator p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	char * s = (char *) reduce_left(&p3,p,max_len);
	printf("%s\n",s);
}

char c;

bool p_palabra(char * out, char * in){
	bool r = in[0] == c;
	if(r) strcpy(out,in);
	return r;
}

void test_accumulators_10() {
	char text[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	char p[20] = "No existe";
	c = 'x';
	iterator p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	void * s = accumulate_left(&p3,p,p_palabra);
	printf("%s\n",s);
	p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	s = accumulate_right(&p3,p,p_palabra);
	printf("%s\n",s);
}

void test_accumulators_11() {
	char mem[1000];
	char text[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator p3 = text_to_iterable_string_fix_tam(text, " ;.", 20);
	multiset r = iterable_to_multiset(&p3);
	iterator ir = multiset_items_iterable(&r);
	iterable_to_console_sep(&ir,"\n","","");
}

list lst() {
	new_rand();
	list ls = list_empty(&int_type);
	for (int i = 0; i < 100; i++) {
		int r = entero_aleatorio(0, 30);
		list_add(&ls, &r);
	}
	return ls;
}

void test_accumulators_12() {
	list ls = lst();
	iterator p3 = list_iterable(&ls);
	multiset r = iterable_to_multiset(&p3);
	iterator ir = multiset_items_iterable(&r);
	iterable_to_console_sep(&ir, "\n", "", "");
}

void test_accumulators_13() {
	char mem[5000];
	new_rand();
	iterator r = iterable_random_long(100,0,20) ;
	multiset ms = iterable_to_multiset(&r);
	iterator r3 = multiset_items_iterable(&ms);
	iterable_to_console_sep(&r3, ",", "{", "}");
}

void freq() {
	iterator r = iterable_random_long(100,0,20);
	multiset ms = iterable_to_multiset_groups(&r, &long_type, remainder_f);
	iterator r3 = multiset_items_iterable(&ms);
	iterable_to_console_sep(&r3, ",", "{", "}");
}

void test_accumulators_14() {
	remainder_base = 13;
	iterator r = iterable_random_long(100,0,20);
	multiset ms = iterable_to_multiset_groups(&r,&long_type,remainder_f);
	iterator r3 = multiset_items_iterable(&ms);
	iterable_to_console_sep(&r3, ",", "{", "}");
}

long * sum(long * out, long *in){
	*out = *out+*in;
	return sum;
}

void test_accumulators_15() {
	remainder_base = 13;
	iterator r = iterable_random_long(100,0,20);
	map ms = iterable_grouping_reduce_map(&r, &long_type, &long_type,
			remainder_f, sum, square_long_f);
	iterator r3 = map_items_iterable(&ms);
	iterable_to_console_sep(&r3, ",", "{", "}");
}

void test_accumulators_16() {
	iterator r = iterable_words_in_file("ficheros/short_quijote.txt", 100, 20," ,;.()");
	int n = iterable_size(&r);
	printf("%d",n);
}


