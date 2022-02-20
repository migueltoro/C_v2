/*
 * iterable.c
 *
 *  Created on: 1 jul. 2019
 *      Author: migueltoro
 */

#include "../types/iterables.h"

iterator iterable_create(
		type *t,
		bool (*has_next)(struct st *iterator),
		void* (*next)(struct st *iterator),
		void *dp,
		int size_dp,
		void (*free_dp)(void *in)) {
	void *state = copy_new(NULL,NULL,t);
	void *a_state = copy_new(NULL,NULL,t);
	iterator r = {t,state,a_state,dp,size_dp,has_next,next,free_dp};
	return r;
}

iterator * iterable_copy_new(iterator * it) {
	iterator * r = (iterator *) heap_copy(it,NULL,sizeof(iterator));
	r->state = copy_new(it->state,NULL,it->type);
	r->a_state = copy_new(it->state,NULL,it->type);
	r->type = type_copy(it->type,NULL);
	if (it->dp_size > 0) {
		r->dp = malloc(it->dp_size);
		copy_size(r->dp,it->dp,it->dp_size);
	}
	return r;
}

bool iterable_has_next(iterator * st) {
	return st->has_next(st);
}

void * iterable_next(iterator * st) {
	check_argument(iterable_has_next(st),__FILE__,__LINE__,"no hay disponible un siguiente estado");
	return st->next(st);
}

bool has_next_false(iterator * st){
	return false;
}

iterator iterable_empty(type * t){
	iterator r = iterable_create(t,has_next_false,NULL,NULL,0,free);
	return r;
}

typedef struct{
	iterator * dp_it;
	void * (*map_function)(void * target, const void * source);
}dp_map;

void free_dp_map(dp_map * in){
	iterable_free(in->dp_it);
	free(in);
}

bool iterable_map_has_next(iterator * c_it){
	dp_map * dp = (dp_map *) c_it->dp;
	return iterable_has_next(dp->dp_it);
}

void * iterable_map_next(iterator * c_it) {
	dp_map * dp = (dp_map *) c_it->dp;
	iterator * it = dp->dp_it;
	void * r = iterable_next(it);
	return dp->map_function(c_it->state, r);
}

iterator iterable_map(iterator * dp_it, type * type, void * (*map_function)(void * out, const void * in)) {
	dp_map dp = {iterable_copy_new(dp_it),map_function};
	int size_dp = sizeof(dp_map);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator r = iterable_create(type,iterable_map_has_next,iterable_map_next,dp_p,size_dp,free_dp_map);
	return r;
}

typedef struct{
	iterator * dp_it;
	iterator * a_it;
	iterator * (*map_function)(void * source);
}dp_flatmap;

void dp_flatmap_free(dp_flatmap * dp){
	iterable_free(dp->dp_it);
	iterable_free(dp->a_it);
	free(dp);
}

bool iterable_flatmap_has_next(iterator * c_it){
	dp_flatmap * dp = (dp_flatmap *) c_it->dp;
	iterator * a_it = dp->a_it;
	iterator * dp_it = dp->dp_it;
	return iterable_has_next(a_it) || iterable_has_next(dp_it) ;
}

void* iterable_flatmap_next(iterator *c_it) {
	dp_flatmap *dp = (dp_flatmap*) c_it->dp;
	iterator *dp_it = dp->dp_it;
	if (!iterable_has_next(dp->a_it)) {
		do {
			void *e = iterable_next(dp_it);
			dp->a_it = dp->map_function(e);
		} while(!iterable_has_next(dp->a_it));
	}
	return iterable_next(dp->a_it);
}


iterator iterable_flatmap(iterator *dp_it, type *type,
		iterator* (*map_function)(void *in)) {
	iterator a_it = iterable_empty(type);
	dp_flatmap dp = { iterable_copy_new(dp_it), &a_it, map_function };
	do {
		void *e = iterable_next(dp.dp_it);
		dp.a_it = map_function(e);
	} while (!iterable_has_next(dp.a_it));
	int size_dp = sizeof(dp_flatmap);
	dp_flatmap *dp_p = malloc(size_dp);
	memcpy(dp_p, &dp, size_dp);
	iterator r = iterable_create(type, iterable_flatmap_has_next,
			iterable_flatmap_next, dp_p, size_dp, dp_flatmap_free);
	return r;
}


typedef struct {
	iterator * dp_it;
	bool (*filter_predicate)(void * source);
	bool has_next;
} dp_filter;

void free_dp_filter(dp_filter * in){
	iterable_free(in->dp_it);
	free(in);
}

void next_depending_state(iterator * c_iterable) {
	dp_filter * dp = (dp_filter *) c_iterable->dp;
	iterator * dp_it = dp->dp_it;
	dp->has_next = false;
	while (iterable_has_next(dp_it)) {
		void * r = iterable_next(dp_it);
		if (dp->filter_predicate(r)) {
			dp->has_next = true;
			copy(c_iterable->state,r,c_iterable->type);
			break;
		}
	}
}

bool iterable_filter_has_next(iterator * c_iterable) {
	dp_filter * dp = (dp_filter *) c_iterable->dp;
	return dp->has_next;
}

void * iterable_filter_next(iterator * c_iterable) {
	copy(c_iterable->a_state,c_iterable->state,c_iterable->type);
	next_depending_state(c_iterable);
	return c_iterable->a_state;
}


iterator iterable_filter(iterator * dp_it, bool (*filter_predicate)(void *)) {
	dp_filter dp = {iterable_copy_new(dp_it),filter_predicate,true};
	int size_dp = sizeof(dp_filter);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator r_it = iterable_create(dp_it->type,iterable_filter_has_next,iterable_filter_next,dp_p,size_dp,free_dp_filter);
	next_depending_state(&r_it);
	return r_it;
}

typedef struct {
	iterator * dp_it1;
	iterator * dp_it2;
}dp_zip;

bool iterable_zip_has_next(iterator * c_iterable) {
	dp_zip * dp = (dp_zip *) c_iterable->dp;
	return iterable_has_next(dp->dp_it1) && iterable_has_next(dp->dp_it2);
}

void * iterable_zip_next(iterator * c_it) {
	dp_zip * dp = (dp_zip *) c_it->dp;
	void * e1 = iterable_next(dp->dp_it1);
	void * e2 = iterable_next(dp->dp_it2);
	pair p = pair_of(e1,e2);
	copy(c_it->state,&p,c_it->type);
	return c_it->state;
}

iterator iterable_zip(iterator * dp_it1, iterator * dp_it2){
	dp_zip dp = {iterable_copy_new(dp_it1),dp_it2};
	int size_dzip = sizeof(dp_zip);
	int size_dp = sizeof(dp_map);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	type t = generic_type_2(&pair_type,dp_it1->type,dp_it2->type);
	iterator new_st = iterable_create(type_copy(&t,NULL),iterable_zip_has_next,iterable_zip_next,dp_p,size_dzip,NULL);
	return new_st;
}

void * consecutive_old_element;
type * consecutive_t_element;

pair * f_consecutive_pair(pair * out, void * in) {
	copy(out->key,consecutive_old_element,consecutive_t_element);
	copy(out->value,in,consecutive_t_element);
	copy(consecutive_old_element,in,consecutive_t_element);
	return out;
}

iterator iterable_consecutive_pairs(iterator * dp_it){
	type t = generic_type_2(&pair_type,dp_it->type,dp_it->type);
	consecutive_t_element = type_copy(dp_it->type,NULL);
	iterator r = iterable_empty(&t);
	if(iterable_has_next(dp_it)) {
		void * e = iterable_next(dp_it);
		consecutive_old_element = copy_new(e,NULL,dp_it->type);
	}
	if(iterable_has_next(dp_it)) {
		r = iterable_map(iterable_copy_new(dp_it),type_copy(&t,NULL),f_consecutive_pair);
	}
	return r;
}

int counter_enum;
type * enumerate_t_element;

enumerate * _f_pair_enumerate(enumerate * out, void * in) {
	out->counter = counter_enum;
	copy(out->value,in,enumerate_t_element);
	counter_enum ++;
	return out;
}

iterator iterable_enumerate(iterator * st, int n){
	counter_enum = n;
	enumerate_t_element = type_copy(st->type,NULL);
	type pt = generic_type_1(&enumerate_type,st->type);
	enumerate_t_element = type_copy(st->type,NULL);
	iterator r = iterable_map(iterable_copy_new(st),type_copy(&pt,NULL),_f_pair_enumerate);
	return r;
}

typedef struct {
	long a;
	long b;
	long c;
}dp_range_long;

bool iterable_range_long_has_next(iterator * c_it){
	dp_range_long * d = (dp_range_long *) c_it->dp;
	return *(long *)c_it->state < d->b;
}

void * iterable_range_long_next(iterator * c_it){
	dp_range_long * d = (dp_range_long *) c_it->dp;
	copy_0(c_it->a_state,c_it->state,&long_type);
	*((long*) c_it->state) = *((long*) c_it->state) +d->c;
	return c_it->a_state;
}

iterator iterable_create(
		type * type,
		bool (*has_next)(struct st * iterator),
		void * (*next)(struct st * iterator),
		void * dependencies,
		int size_dependencies,
		void (*free_dependencies)(void * in));

iterator iterable_range_long(long a, long b, long c){
	dp_range_long dp = {a,b,c};
	int size_dp = sizeof(dp_range_long);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator r_it = iterable_create(&long_type,iterable_range_long_has_next,iterable_range_long_next,dp_p,size_dp,free);
	*((long*) r_it.state) = a;
	return r_it;
}

typedef struct {
	double a;
	double b;
	double c;
}dp_range_double;

bool iterable_range_double_has_next(iterator * current_iterable){
	dp_range_double * dp = (dp_range_double *) current_iterable->dp;
	return *(double *)current_iterable->state < dp->b;
}

void * iterable_range_double_next(iterator * c_it){
	dp_range_double * dp = (dp_range_double *) c_it->dp;
	copy_0(c_it->a_state,c_it->state,&double_type);
	*((double*) c_it->state) = *((double*) c_it->state) +dp->c;
	return c_it->a_state;
}

iterator iterable_range_double(double a, double b, double c){
	dp_range_double dp = {a,b,c};
	int size_dp = sizeof(dp_range_double);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator new_st = iterable_create(&double_type,iterable_range_double_has_next,iterable_range_double_next,dp_p,size_dp,free);
	*((double*) new_st.state) = a;
	return new_st;
}


typedef struct {
	bool (*hash_next)(void * element);
	void * (*has_next)(void * out, void * in);
}dp_iterate;



bool iterable_iterate_has_next(iterator * current_iterable){
	dp_iterate * d = (dp_iterate *) current_iterable->dp;
	return d->hash_next(current_iterable->state);
}

void * iterable_iterate_next(iterator * c_it){
	dp_iterate * d = (dp_iterate *) c_it->dp;
	copy(c_it->a_state,c_it->state,c_it->type);
	d->has_next(c_it->state, c_it->state);
	return c_it->a_state;
}

iterator iterable_iterate(type * type,
		void * initial_value,
		bool (*has_next)(void * element),
		void * (*next)(void * out, void * in)) {
	dp_iterate dp = {has_next, next};
	int size_dp = sizeof(dp_iterate);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator r_it = iterable_create(type,iterable_iterate_has_next,iterable_iterate_next,dp_p,size_dp,free);
	copy_size(r_it.state,initial_value,type->size);
	return r_it;
}

iterator iterable_random_long(long n, long a, long b) {
	entero_aleatorio_long_a = a;
	entero_aleatorio_long_b = b;
	menor_que_long_ref = n;
	inc_long_ref = 1;
	int e = 0;
	iterator r = iterable_iterate(&long_type, &e, menor_que_long, inc_long_f);
	iterator r2 = iterable_map(iterable_copy_new(&r), &long_type, entero_aleatorio_long_f);
	return r2;
}

iterator iterable_primos(int a, int b) {
	if (!es_primo(a))
		a = siguiente_primo(a);
	menor_que_long_ref = b;
	iterator r = iterable_iterate(&long_type,&a,menor_que_long, siguiente_primo_f);
	return r;
}

typedef struct{
	char * text;
	char * token;
	char * delimiters;
	char * saveptr[1];
}dp_split;

bool iterable_split_has_next(iterator * c_it) {
	dp_split * dp = (dp_split *) c_it->dp;
	return dp->token != NULL;
}

void * iterable_split_next(iterator * c_it){
	dp_split * dp = (dp_split *) c_it->dp;
	char * old = dp->token;
	dp->token = strtok_r2(NULL,dp->delimiters,dp->saveptr);
	return old;
}

void dependencies_split_free(dp_split * ds){
	free(ds->text);
	free(ds);
}

iterator text_to_iterable_string_fix(char * text, const char * delimiters){
	return text_to_iterable_string_fix_tam(text,delimiters,string_fix_tam);
}

iterator text_to_iterable_string_fix_tam(char * text, const char * delimiters, int tam) {
	type t = string_fix_type_of_tam(tam);
	if(string_fix_all_space(text)) return iterable_empty(type_copy(&t,NULL));
	dp_split dp;
	int size_dp = sizeof(dp_split);
	dp.text = malloc(strlen(text)+2);
	dp.text = strcpy(dp.text,text);
	dp.delimiters = delimiters;
	dp.token = strtok_r2(dp.text,delimiters,dp.saveptr);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	iterator r = iterable_create(type_copy(&t,NULL),iterable_split_has_next,
			iterable_split_next,dp_p,size_dp,dependencies_split_free);
	return r;
}

string_fix text_to_iterable_delimiters = " ,;.()";

int string_fix_function_tam;

iterator * text_to_iterable_string_fix_function(char * text) {
	iterator it = text_to_iterable_string_fix_tam(text, text_to_iterable_delimiters,string_fix_function_tam);
	return iterable_copy_new(&it);
}

typedef struct{
	FILE * file;
	bool has_next;
}dp_file;

void free_dependencies_file(dp_file * df){
	fclose(df->file);
	free(df);
}

bool iterable_file_has_next(iterator * c_iterable) {
	dp_file * dp = (dp_file *) c_iterable->dp;
	return dp->has_next;
}

void * iterable_file_next(iterator * c_it){
	dp_file * dp = (dp_file *) c_it->dp;
	type * t = c_it->type;
	copy(c_it->a_state,c_it->state,c_it->type);
	char * r = fgets(c_it->state, t->size, dp->file);
	dp->has_next = r!=NULL;
	remove_eol(c_it->a_state);
	return c_it->a_state;
}

iterator iterable_file_string_fix(char * file) {
	return iterable_file_string_fix_tam(file,string_fix_tam);
}

iterator iterable_file_string_fix_tam(char * file, int n) {
	FILE * st = fopen(file,"r");
	char  ms[Tam_String];
	if(st==NULL) sprintf(ms,"no se encuentra el fichero %s",file);
	check_not_null(st,__FILE__,__LINE__,ms);
	dp_file dp = {st,false};
	int size_dp = sizeof(dp_file);
	void * dp_p = malloc(size_dp);
	memcpy(dp_p,&dp,size_dp);
	type t = string_fix_type_of_tam(n);
	iterator it_file = iterable_create(type_copy(&t,NULL),iterable_file_has_next,iterable_file_next,
			dp_p,size_dp,free_dependencies_file);
	dp_file * dp2 = (dp_file *)it_file.dp;
	char * r = fgets(it_file.state,n,dp2->file);
	dp2->has_next = r!=NULL;
	return it_file;
}

iterator iterable_words_in_file(char *file, int line_tam, int word_tam, char * sep) {
	type t = string_fix_type_of_tam(word_tam);
	strcpy(text_to_iterable_delimiters,sep);
	iterator r1 = iterable_file_string_fix_tam(file, line_tam);
	string_fix_function_tam = word_tam;
	iterator r2 = iterable_flatmap(iterable_copy_new(&r1), type_copy(&t,NULL),text_to_iterable_string_fix_function);
	return r2;
}

int word_tam_expand;
int counter_expand;
type * type_expand;

enumerate * word_to_enumerate(enumerate * out, char * word){
	*out = enumerate_of(counter_expand,word);
	return out;
}

iterator * enumerate_expand_f(enumerate * in){
	int counter = in->counter;
	char * value = in->value;
	counter_expand = counter;
	iterator r = text_to_iterable_string_fix_tam(value,text_to_iterable_delimiters,word_tam_expand);
	iterator r2 = iterable_map(&r,type_expand,word_to_enumerate);
	return iterable_copy_new(&r2);
}

iterator iterable_words_and_line_in_file(char *file, int init,int line_tam, int word_tam, char *sep) {
	type t = string_fix_type_of_tam(word_tam);
	type t2 = generic_type_1(&enumerate_type,&t);
	type_expand = type_copy(&t2,NULL);
	strcpy(text_to_iterable_delimiters, sep);
	word_tam_expand = word_tam;
	iterator r1 = iterable_file_string_fix_tam(file, line_tam);
	iterator r2 = iterable_enumerate(iterable_copy_new(&r1),init);
	iterator r3 = iterable_flatmap(iterable_copy_new(&r2),type_expand,enumerate_expand_f);
	return r3;
}

char * iterable_tostring(iterator * st,
		char * mem){
	return iterable_tostring_sep(st,",","{","}",mem);
}

char * iterable_tostring_sep(iterator * st,char * sep,char * prefix,char * suffix,char * mem){
	char m[Tam_String];
	bool first = true;
	strcpy(mem,prefix);
	while(iterable_has_next(st)){
		void *  next = iterable_next(st);
		char * ns = tostring(next,m,st->type);
		if(first) first = false;
		else strcat(mem,sep);
		strcat(mem,ns);
	}
	strcat(mem,suffix);
	return mem;
}

string_var iterable_tostring_sep_big(iterator * st,char * sep,char * prefix,char * suffix){
	string_fix m;
	bool first = true;
	string_var s = string_var_empty();
	string_var_add_string_fix(&s,prefix);
	while(iterable_has_next(st)){
		void *  next = iterable_next(st);
		char * ns = tostring(next,m,st->type);
		if(first) first = false;
		else string_var_add_string_fix(&s,sep);
		string_var_add_string_fix(&s,ns);
	}
	string_var_add_string_fix(&s,suffix);
	return s;
}

string_var iterable_tostring_big(iterator * st){
	return iterable_tostring_sep_big(st,",","{","}");
}

void iterable_to_console(iterator * st){
	iterable_to_console_sep(st,",", "{", "}");
}

void iterable_to_console_sep(iterator * st, char * sep, char * prefix, char * suffix) {
	char m[Tam_String];
	bool first = true;
	printf("%s",prefix);
	while (iterable_has_next(st)) {
		void * next = iterable_next(st);
		char * ns = tostring(next, m, st->type);
		if (first) first = false;
		else printf("%s",sep);
		printf("%s",ns);
	}
	printf("%s",suffix);
}

void write_iterable_to_file(char * file, iterator * st){
	char mem[256];
	FILE * f = fopen(file, "wt");
	while (iterable_has_next(st)) {
			fprintf(f, "%s\n", tostring(iterable_next(st),mem,st->type));
	}
	fclose(f);
}

void iterable_free(iterator * st) {
	if (st != NULL) {
		if(st->type !=NULL) type_free(st->type);
		free(st->state);
		free(st->a_state);
		if (st->free_dp != NULL)
			st->free_dp(st->dp);
		else
			free(st->dp);
	}
}

void iterables_free(int n, ...) {
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++) {
		iterator * it = va_arg(valist, iterator *);
		iterable_free(it);
	}
	va_end(valist);
}


bool ft(double_pair * in){
	char mem[50];
	Cuadrante c = punto_cuadrante(in);
	return c == PRIMERO;
}

void test_iterables_0() {
	char mem[500];
	menor_que_long_ref = 3000;
	iterator r = iterable_range_long(0, 140, 7);
	iterator * r2 = iterable_copy_new(&r);
	iterator r3 = iterable_map(r2, &long_type, square_long_f);
	iterator r4 = iterable_filter(&r3, menor_que_long);
	iterator r5 = iterable_consecutive_pairs(&r4);
	iterator r6 = iterable_enumerate(&r5,0);
	iterator r7 = iterable_range_long(10,100,2);
	iterator r8 = iterable_zip(&r7,&r6);
	iterable_to_console_sep(&r8,",","{","}");
}

iterator * expand(iterator * out, long * in){
	iterator r = iterable_random_long(*in,0,300);
	*out = r;
	return out;
}

void test_iterables_0_1() {
	iterator it;
	int a = 10;
	expand(&it,&a);
	iterable_to_console_sep(&it,",","{","}");
}

void test_iterables_0_2() {
	new_rand();
	iterator r = iterable_range_long(1, 10, 1);
	iterator r2 = iterable_flatmap(&r,&long_type,expand);
	printf("%d\n",iterable_size(&r2));
//	iterable_to_console_sep(&r2,",","{","}");
}

void test_iterables_0_3() {
	char mem[500];
	long a = 2;
	menor_que_long_ref = 1000;
	iterator r = iterable_iterate(&long_type,&a,menor_que_long,siguiente_primo_f);
	iterable_to_console_sep(&r,",","{","}");
}

void test_iterables_1() {
	char mem[500];
	iterator r = iterable_file_string_fix_tam("ficheros/prueba.txt",30);
	iterator r2 = iterable_map(&r, &pair_double_type, punto_parse);
	iterator r3 = iterable_filter(&r2, ft);
	iterable_tostring(&r3, mem);
	printf("%s\n", mem);
}

void test_iterables_2() {
	char delimiters[] = " ,;.()";
	char text1[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator sp = text_to_iterable_string_fix_tam(text1,delimiters,40);
	iterable_to_console(&sp);
	printf("\n_______________\n");
	char text2[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator * p3 = text_to_iterable_string_fix_function(text2);
	iterable_to_console(p3);
	printf("\n_______________\n");
	char text3[] = "Quédese eso del barbero a mi cargo, dijo SAncho, y al de vuestra merced se quede el procurar venir a ser rey y el hacerme conde. Así será, respondió Don Quijote. ";
	p3 = text_to_iterable_string_fix_function(text3);
	iterable_to_console(p3);
}

void test_iterables_2_1(){
	type t = string_fix_type_of_tam(10);
	iterator it = iterable_empty(&t);
	iterator * r = iterable_copy_new(&it);
	printf("%d,%d",r->type->size,r->dp_size);
}

void test_iterables_3(){
	type t = string_fix_type_of_tam(10);
	string_fix_copy(text_to_iterable_delimiters," ,",&string_fix_type);
	iterator r = iterable_file_string_fix_tam("ficheros/quijote.txt",50);
	string_fix_function_tam = 15;
	iterator r2 = iterable_flatmap(&r,&t,text_to_iterable_string_fix_function);
//	iterable_to_console_sep(&r2,",","{","}");
	printf("%d\n",iterable_size(&r2));
	printf("\n_________________\n");
	char text[] = "(23....,45.)";
	iterator it = text_to_iterable_string_fix_tam("(23....,45.)"," ,.()",20);
	iterable_to_console_sep(&it,",","{","}");
}

iterator * new_it() {
	iterator r, r0, r1, r2, r3;
	r = iterable_range_long(2, 100, 9);
	r0 = iterable_filter(&r,es_primo_f);
	r1 = iterable_map(&r0,&long_type,square_long_f);
	r2 = iterable_consecutive_pairs(&r1);
	r3 = iterable_enumerate(&r2,0);
	return iterable_copy_new(&r3);
}

void test_iterables_4() {
	iterator * it = new_it();
	iterable_to_console(it);
}

void test_iterables_5() {
	iterator r = iterable_words_in_file("ficheros/quijote.txt", 100, 20," ,;.()");
	printf("%d\n",iterable_size(&r));
}


