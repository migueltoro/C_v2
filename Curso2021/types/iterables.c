/*
 * iterable.c
 *
 *  Created on: 1 jul. 2019
 *      Author: migueltoro
 */

#include "../types/iterables.h"

iterator iterable_create(
		type * type,
		bool (*has_next)(struct st * iterator),
		void * (*next)(struct st * iterator),
		void * (*see_next)(struct st * iterator),
		void (*free_dp)(void * in),
		void * dps,
		int size_dp);


// memory_heap memory_heap_iterable = {0,0,NULL,0};

void iterable_copy_state_to_auxiliary(iterator * st){
	check_not_null(st->a_state,__FILE__,__LINE__,"el estado auxiliar del iterator es null");
	memcpy(st->a_state,st->state,st->type->size);
}

iterator * iterable_copy(iterator * it) {
	return copy_and_mem(it,sizeof(iterator));
}

bool iterable_has_next(iterator * st) {
	return st->has_next(st);
}

void * iterable_see_next(iterator * st){
	check_argument(iterable_has_next(st),__FILE__,__LINE__,"no hay disponible un siguiente estado");
	return st->see_next(st);
}

void * iterable_next(iterator * st) {
	check_argument(iterable_has_next(st),__FILE__,__LINE__,"no hay disponible un siguiente estado");
	return st->next(st);
}

iterator iterable_create(
		type * t,
		bool (*has_next)(struct st * iterator),
		void * (*next)(struct st * iterator),
		void * (*see_next)(struct st * iterator),
		void (*free_dp)(void * in),
		void * dps,
		int size_dp){
	void * state = malloc(t->size);
	void * a_state = malloc(t->size);
	iterator r = {t,NULL,state,a_state,has_next,next,see_next,free_dp,copy_and_mem(dps,size_dp)};
	return r;
}

bool has_next_false(iterator * st){
	return false;
}

iterator iterable_empty(){
	iterator r = {0,NULL,0,NULL,NULL,has_next_false,NULL,NULL,NULL,NULL};
	return r;
}

typedef struct{
	void * (*map_function)(void * target, const void * source);
}dp_map;

bool iterable_map_has_next(iterator * current_iterable){
	return iterable_has_next(current_iterable->dp_iterable);
}

void * iterable_map_see_next(iterator * current_iterable){
	dp_map * d = (dp_map *) current_iterable->dps;
	iterator * depending_iterable = current_iterable->dp_iterable;
	return d->map_function(depending_iterable->state,iterable_see_next(depending_iterable));
}

void * iterable_map_next(iterator * current_iterable) {
	dp_map * d = (dp_map *) current_iterable->dps;
	iterator * depending_iterable = current_iterable->dp_iterable;
	void * r = iterable_next(depending_iterable);
	return d->map_function(current_iterable->state, r);
}


iterator iterable_map(iterator * depending_iterable, type * type, void * (*map_function)(void * out, const void * in)) {
	dp_map dp = {map_function};
	int size_dp = sizeof(dp_map);
	iterator r = iterable_create(type,iterable_map_has_next,iterable_map_next,iterable_map_see_next,NULL,&dp,size_dp);
	r.dp_iterable = iterable_copy(depending_iterable);
	return r;
}


typedef struct{
	iterator actual_iterable;
	iterator * (*map_function)(iterator * target, void * source);
}dp_flatmap;

bool iterable_flatmap_has_next(iterator * current_iterable){
	dp_flatmap * d = (dp_flatmap *) current_iterable->dps;
	iterator act = d->actual_iterable;
	return iterable_has_next(&act);
}

void * iterable_flatmap_see_next(iterator * current_iterable){
	dp_flatmap * d = (dp_flatmap *) current_iterable->dps;
	iterator st = d->actual_iterable;
	return iterable_see_next(&st);
}

void * iterable_flatmap_next(iterator * c_iterable) {
	dp_flatmap * d = (dp_flatmap *) c_iterable->dps;
	iterator * depending_iterable = c_iterable->dp_iterable;
	copy(c_iterable->a_state,iterable_next(&d->actual_iterable),c_iterable->type->size);
	while(!iterable_has_next(&d->actual_iterable)){
		if(iterable_has_next(depending_iterable)){
			iterable_free(&d->actual_iterable);
			d->map_function(&d->actual_iterable,iterable_next(depending_iterable));
		}
		else break;
	}
	return c_iterable->a_state;
}


iterator iterable_flatmap(iterator * depending_iterable, type * type,
		iterator * (*map_function)(iterator * out, void * in)) {
	iterator actual_iterable;
	dp_flatmap dp = {actual_iterable, map_function};
	int size_dp = sizeof(dp_flatmap);
	do {
		if(iterable_has_next(depending_iterable)){
			map_function(&dp.actual_iterable, iterable_next(depending_iterable));
		} else break;
	} while (!iterable_has_next(&dp.actual_iterable) && iterable_has_next(depending_iterable));
	iterator r = iterable_create(type, iterable_flatmap_has_next,
			iterable_flatmap_next, iterable_flatmap_see_next, NULL, &dp, size_dp);
	r.dp_iterable = iterable_copy(depending_iterable);
	return r;
}


typedef struct {
	bool (*filter_predicate)(void * source);
	bool has_next;
} dp_filter;

void next_depending_state(iterator * c_iterable) {
	dp_filter * dp = (dp_filter *) c_iterable->dps;
	iterator * depending_iterable = c_iterable->dp_iterable;
	dp->has_next = false;
	while (iterable_has_next(depending_iterable)) {
		void * r = iterable_next(depending_iterable);
		if (dp->filter_predicate(r)) {
			dp->has_next = true;
			copy(c_iterable->state,r,c_iterable->type->size);
			break;
		}
	}
}

bool iterable_filter_has_next(iterator * current_iterable) {
	dp_filter * d = (dp_filter *) current_iterable->dps;
	return d->has_next;
}

void * iterable_filter_see_next(iterator * current_iterable){
    return current_iterable->state;
}

void * iterable_filter_next(iterator * current_iterable) {
	iterable_copy_state_to_auxiliary(current_iterable);
	next_depending_state(current_iterable);
	return current_iterable->a_state;
}


iterator iterable_filter(iterator * depending_iterable, bool (*filter_predicate)(void *)) {
	dp_filter df = {filter_predicate,true};
	int size_df = sizeof(dp_filter);
	iterator new_st = iterable_create(depending_iterable->type,iterable_filter_has_next,iterable_filter_next,iterable_filter_see_next,NULL,&df,size_df);
	new_st.dp_iterable = iterable_copy(depending_iterable);
	next_depending_state(&new_st);
	return new_st;
}


static int pairs_type_size;

pair * _f_consecutive_pair(pair * out, void * in) {
	static bool first = true;
	static char key[Tam_String];
	static char value[Tam_String];
	if(first){;
		copy(value,in,pairs_type_size);
		first = false;
	} else {
		copy(key,value,pairs_type_size);
		copy(value,in,pairs_type_size);
	}
	out->key = key;
	out->value = value;
	return out;
}

iterator iterable_consecutive_pairs(iterator * st){
	iterator r;
	check_argument(st->type->size<=Tam_String,__FILE__,__LINE__,"No hay suficiente memoria");
	pair p;
	pairs_type_size = st->type->size;
	if(iterable_has_next(st)) {
		void * e = iterable_next(st);
		_f_consecutive_pair(&p,e);
	} else {
		r = iterable_empty();
	}
	if(iterable_has_next(st)) {
		type t = generic_type_2(&pair_type,st->type,st->type);
		r = iterable_map(st,copy_and_mem(&t,sizeof(type)),_f_consecutive_pair);
	} else {
		r = iterable_empty();
	}
	r.dp_iterable = iterable_copy(st);
	return r;
}

pair_enumerate * _f_pair_enumerate(pair_enumerate * out, void * in) {
	static int n;
	if (in == NULL) {
		n = 0;
		return out;
	}
	out->counter = n;
	out->value = in;
	n = n+1;
	return out;
}

iterator iterable_enumerate(iterator * st){
	pair_enumerate p;
	pairs_type_size = st->type->size;
	_f_pair_enumerate(&p,NULL);
	type pt = generic_type_1(&pair_enumerate_type,st->type);
	iterator r = iterable_map(st,copy_and_mem(&pt,sizeof(type)),_f_pair_enumerate);
	r.dp_iterable = iterable_copy(st);
	return r;
}

typedef struct {
	long a;
	long b;
	long c;
}dp_range_long;

bool iterable_range_long_has_next(iterator * c_iterable){
	dp_range_long * d = (dp_range_long *) c_iterable->dps;
	return *(long *)c_iterable->state < d->b;
}

void * iterable_range_long_see_next(iterator * current_iterable){
    return current_iterable->state;
}

void * iterable_range_long_next(iterator * current_iterable){
	dp_range_long * d = (dp_range_long *) current_iterable->dps;
	iterable_copy_state_to_auxiliary(current_iterable);
	*((long*) current_iterable->state) = *((long*) current_iterable->state) +d->c;
	return current_iterable->a_state;
}

iterator iterable_range_long(long a, long b, long c){
	dp_range_long dr = {a,b,c};
	int size_dr = sizeof(dp_range_long);
	iterator new_st = iterable_create(&long_type,iterable_range_long_has_next,iterable_range_long_next,iterable_range_long_see_next,NULL,&dr,size_dr);
	*((long*) new_st.state) = a;
	return new_st;
}

typedef struct {
	double a;
	double b;
	double c;
}dp_range_double;

bool iterable_range_double_has_next(iterator * current_iterable){
	dp_range_double * d = (dp_range_double *) current_iterable->dps;
	return *(double *)current_iterable->state < d->b;
}

void * iterable_range_double_see_next(iterator * current_iterable){
    return current_iterable->state;
}

void * iterable_range_double_next(iterator * current_iterable){
	dp_range_double * d = (dp_range_double *) current_iterable->dps;
	iterable_copy_state_to_auxiliary(current_iterable);
	*((double*) current_iterable->state) = *((double*) current_iterable->state) +d->c;
	return current_iterable->a_state;
}

iterator iterable_range_double(double a, double b, double c){
	dp_range_double dr = {a,b,c};
	int size_dr = sizeof(dp_range_double);
	iterator new_st = iterable_create(&double_type,iterable_range_double_has_next,iterable_range_double_next,iterable_range_double_see_next,NULL,&dr,size_dr);
	*((double*) new_st.state) = a;
	return new_st;
}


typedef struct {
	void * initial_value;
	bool (*hash_next)(void * element);
	void * (*has_next)(void * out, void * in);
}dp_iterate;

bool iterable_iterate_has_next(iterator * current_iterable){
	dp_iterate * d = (dp_iterate *) current_iterable->dps;
	return d->hash_next(current_iterable->state);
}

void * iterable_iterate_see_next(iterator * current_iterable){
    return current_iterable->state;
}

void * iterable_iterate_next(iterator * current_iterable){
	dp_iterate * d = (dp_iterate *) current_iterable->dps;
	iterable_copy_state_to_auxiliary(current_iterable);
	d->has_next(current_iterable->state, current_iterable->state);
	return current_iterable->a_state;
}

iterator iterable_iterate(type * type,
		void * initial_value,
		bool (*has_next)(void * element),
		void * (*next)(void * out, void * in)) {
	dp_iterate di = {initial_value, has_next, next};
	int size_di = sizeof(dp_iterate);
	iterator new_st = iterable_create(type,iterable_iterate_has_next,
			iterable_iterate_next,iterable_iterate_see_next,NULL,&di,size_di);
	copy(new_st.state,initial_value,type->size);
	return new_st;
}

typedef struct{
	char * text;
	char * token;
	char * delimiters;
	char * saveptr[1];
}dp_split;

bool iterable_split_has_next(iterator * current_iterable) {
	dp_split * dp = (dp_split *) current_iterable->dps;
	return dp->token != NULL;
}

void * iterable_split_see_next(iterator * current_iterable){
	dp_split * dp = (dp_split *) current_iterable->dps;
    return dp->token;
}

void * iterable_split_next(iterator * current_iterable){
	dp_split * dp = (dp_split *) current_iterable->dps;
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
	dp_split ds;
	int size_ds = sizeof(dp_split);
	ds.text = malloc(strlen(text)+2);
	ds.text = strcpy(ds.text,text);
	ds.delimiters = delimiters;
	ds.token = strtok_r2(ds.text,delimiters,ds.saveptr);
	type t = string_fix_type_of_tam(tam);
	iterator r = iterable_create(type_copy(&t), iterable_split_has_next,
			iterable_split_next, iterable_split_see_next,dependencies_split_free, &ds,size_ds);
	return r;
}

string_fix text_to_iterable_delimiters = " ,;.()";

iterator * text_to_iterable_string_fix_function(iterator * out, char * text) {
	iterator it = text_to_iterable_string_fix_tam(text, text_to_iterable_delimiters,string_fix_tam);
	*out = it;
	return out;
}

typedef struct{
	FILE * file;
	bool has_next;
}dp_file;

void free_dependencies_file(dp_file * df){
	fclose(df->file);
}

bool iterable_file_has_next(iterator * c_iterable) {
	dp_file * dp = (dp_file *) c_iterable->dps;
	return dp->has_next;
}

void * iterable_file_see_next(iterator * c_iterable){
    return c_iterable->state;
}

void * iterable_file_next(iterator * c_iterable){
	dp_file * dp = (dp_file *) c_iterable->dps;
	type * t = c_iterable->type;
	iterable_copy_state_to_auxiliary(c_iterable);
	char * r = fgets(c_iterable->state, t->size, dp->file);
	dp->has_next = r!=NULL;
	remove_eol(c_iterable->a_state);
	return c_iterable->a_state;
}

iterator file_iterable_string_fix(char * file) {
	return file_iterable_string_fix_tam(file,string_fix_tam);
}

iterator file_iterable_string_fix_tam(char * file, int n) {
	FILE * st = fopen(file,"r");
	char  ms[Tam_String];
	if(st==NULL) sprintf(ms,"no se encuentra el fichero %s",file);
	check_not_null(st,__FILE__,__LINE__,ms);
	dp_file df = {st,false};
	int size_df = sizeof(dp_file);
	type t = string_fix_type_of_tam(n);
	iterator it_file = iterable_create(type_copy(&t),iterable_file_has_next,iterable_file_next,
			iterable_file_see_next,free_dependencies_file,&df,size_df);
	dp_file * dp = (dp_file *)it_file.dps;
	char * r = fgets(it_file.state,n,dp->file);
	dp->has_next = r!=NULL;
	return it_file;
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
		if(st->dp_iterable != NULL) free(st->dp_iterable);
		if(st->type !=NULL) type_free(st->type);
		free(st->state);
		free(st->a_state);
		if (st->free_dp != NULL)
			st->free_dp(st->dps);
		else
			free(st->dps);
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


bool ft(punto * in){
	char mem[50];
	Cuadrante c = punto_cuadrante(in);
	return c == PRIMERO;
}

long * cuadrado(long * out,long * in){
	*out = (*in)*(*in);
	return out;
}


void test_iterables_1() {
	printf("\n_______________\n");
	char delimiters[] = " ,;.()";
	char text1[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator sp = text_to_iterable_string_fix_tam(text1, delimiters,20);
	iterable_to_console(&sp);
	printf("\n_______________\n");
	char text2[] = "El    Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las negociaciones de la ministra María Jesús Montero. ";
	iterator p3;
	text_to_iterable_string_fix_function(&p3, text2);
	iterable_to_console(&p3);
	printf("\n_______________\n");
	char text3[] = "Quédese eso del barbero a mi cargo, dijo SAncho, y al de vuestra merced se quede el procurar venir a ser rey y el hacerme conde. Así será, respondió Don Quijote. ";
	text_to_iterable_string_fix_function(&p3, text3);
	iterable_to_console(&p3);
}

void test_iterables_2() {
	char mem[500];
	menor_que_long_ref = 500;
	iterator r = iterable_range_long(0,1400,7);
	iterator r2 = iterable_map(&r,&long_type,cuadrado);
	iterator r3 = iterable_filter(&r2,menor_que_long);
	iterable_tostring(&r3,mem);
	printf("\n%s\n",mem);
	iterator fit = file_iterable_string_fix("ficheros/prueba.txt");
	iterator fmap = iterable_map(&fit,&punto_type,punto_parse);
	iterator ff = iterable_filter(&fmap,ft);
	iterable_tostring(&ff,mem);
	printf("\n%s\n",mem);
}

void test_iterables_3(){
	type t = string_fix_type_of_tam(10);
	string_fix_copy(text_to_iterable_delimiters," ,",&string_fix_type);
	iterator fit = file_iterable_string_fix("ficheros/datos_entrada.txt");
	iterator fit3 = iterable_filter(&fit,string_fix_not_all_space);
	iterator fmap = iterable_flatmap(&fit3,&t,text_to_iterable_string_fix_function);
	iterable_to_console_sep(&fmap,"\n","{","}");
	printf("\n_________________\n");
	char text[] = "(23....,45.)";
	iterator it = text_to_iterable_string_fix_tam("(23....,45.)"," ,.()",20);
	iterable_to_console_sep(&it,",","{","}");
}


void test_iterables_5() {
	long e0 =2;
	menor_que_long_ref = 1000;
	iterator it = iterable_iterate(&long_type,&e0, menor_que_long, siguiente_primo_f);
	iterable_to_console(&it);
}

void test_iterables_6() {
	char mem[6000];
	iterator p3;
	iterator r = iterable_range_long(0, 1000, 9);
	iterator r2 = iterable_consecutive_pairs(&r);
	iterator r3 = iterable_enumerate(&r2);
	iterable_to_console(&r3);
}

iterator * new_it() {
	iterator r, r0, r1, r2, r3;
	r = iterable_range_long(2, 100, 9);
	r0 = iterable_filter(iterable_copy(&r),es_primo_f);
	r1 = iterable_map(iterable_copy(&r0),&long_type,square_long_f);
	r2 = iterable_consecutive_pairs(iterable_copy(&r1));
	r3 = iterable_enumerate(iterable_copy(&r2));
	return iterable_copy(&r3);
}

void test_iterables_7() {
	iterator * it = new_it();
	iterable_to_console(it);
}

int num_palabras() {
	type t = string_fix_type_of_tam(15);
	iterator t1 = file_iterable_string_fix_tam("ficheros/quijote.txt",45);
	iterator t2 = iterable_filter(iterable_copy(&t1), string_fix_not_all_space);
	iterator t3 = iterable_flatmap(iterable_copy(&t2),type_copy(&t),text_to_iterable_string_fix_function);
	iterator t4 = iterable_enumerate(iterable_copy(&t3));
	iterable_to_console_sep(&t4, "\n", "", "");
	return 0;
}



void test_iterables_8() {
	int n = num_palabras();
	printf("===%d\n", n);
}

