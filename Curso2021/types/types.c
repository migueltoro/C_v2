/*
 * types.c
 *
 *  Created on: 4 nov. 2018
 *      Author: migueltoro
 */

#include "../types/types.h"

/* Hash a string */
unsigned long int hash(const char *key) {

	unsigned long int hashval = 0 ;
	int i = 0;
	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 5;
		hashval += key[ i ];
		i++;
	}
	return hashval;
}

unsigned long int hash_code(void * in, type * t){
	char mem[256];
	char * r = tostring(in,mem,t);
	unsigned long int hash_index = hash(r);
	return hash_index;
}

// generic types


type* type_copy(type *t, heap *hp) {
	type * r;
	switch (t->num_types) {
	case 0: r = heap_copy(t,hp,sizeof(type));break;
	case 1:{
		type * r0 = heap_copy(t->types[0],hp,sizeof(type));
		r = heap_copy(t,hp,sizeof(type));
		r->types[0] = r0;
		break;
	}
	case 2:{
		type * r0 = heap_copy(t->types[0],hp,sizeof(type));
		type * r1 = heap_copy(t->types[1],hp,sizeof(type));
		r = heap_copy(t,hp,sizeof(type));
		r->types[0] = r0;
		r->types[1] = r1;
		break;
	}
	}
	return r;

}

bool type_equals(const type *t1, const type *t2) {
	bool r = strcmp(t1->name, t2->name) == 0 && t1->num_types == t2->num_types;
	switch (t1->num_types) {
	case 0:
		break;
	case 1:
		r = type_equals(t1->types[0], t2->types[0]);
		break;
	case 2:
		r = type_equals(t1->types[0], t2->types[0])
				&& type_equals(t1->types[1], t2->types[1]);
		break;
	}
	return r;
}

char * type_tostring(type * t, char * mem){
	switch (t->num_types) {
		case 0: strcpy(mem,t->name);break;
		case 1: sprintf(mem,"%s(%s)",t->name,type_tostring(t->types[0],mem));break;
		case 2: sprintf(mem,"%s(%s,%s)",t->name,type_tostring(t->types[0],mem),type_tostring(t->types[1],mem));break;
		}
		return mem;
}

type generic_type_1(type *base, type * key_type) {
	type rb = *base;
	rb.types[0] = key_type;
	return rb;
}

type generic_type_2(type *base, type * key_type, type * value_type) {
	type rb = *base;
	rb.types[0] = key_type;
	rb.types[1] = value_type;
	return rb;
}

void type_free(type * t) {
	if (t->num_types == 1)
		free(t->types[0]);
	else if (t->num_types == 2) {
		free(t->types[0]);
		free(t->types[1]);
	}
	free(t);
}

int num_types(type * t){
	return t->num_types;
}

type * types(type * base, int i){
	return base->types[i];
}

// functions

bool equals(const void *e1, const void *e2, type *type) {
	return type->equals(e1,e2,type);
}

char* tostring(const void *e, char *mem, type *type) {
	return type->tostring(e,mem,type);
}

int order(const void *e1, const void *e2, type *type) {
	return type->order(e1,e2,type);
}

void* parse(void *out, char *text, type *type) {
	return type->parse(out,text,type);
}

void free_mem(void * e, type * type){
	type->free(e,type);
}

void * copy_new(void * in, heap * h, type * type){
	return type->copy_new(in,h,type);
}

void copy(void * out, void * in, type * type){
	type->copy(out,in,type);
}

type * tmp_type;

void set_tmp_type(type * t){
	tmp_type = t;
}

bool equals_st(const void * e1, const void * e2) {
	return tmp_type->equals(e1,e2,&tmp_type);
}

char * tostring_st(const void * e, char * mem) {
	return tmp_type->tostring(e,mem,&tmp_type);
}

int order_st(const void * e1, const void * e2){
	return tmp_type->order(e1,e2,&tmp_type);
}

void * parse_st(void * out, char * text){
	return tmp_type->parse(out,text,&tmp_type);
}

// utilities

void * copy_size(void * out, void * in, int size){
	check_not_null(in,__FILE__,__LINE__,"puntero in null");
	check_not_null(out,__FILE__,__LINE__,"puntero out null");
	memcpy(out,in,size);
	return out;
}

void * copy_in_pointer(void ** out, void * in, int size){
	check_not_null(in,__FILE__,__LINE__,"puntero null");
	check_not_null(out,__FILE__,__LINE__,"puntero null");
	void * r = *out;
	memcpy(r,in,size);
	return out;
}

void * swap(void * out, void * in, int size){
	check_not_null(in,__FILE__,__LINE__,"puntero null");
	check_not_null(out,__FILE__,__LINE__,"puntero null");
	check_argument(size>0,__FILE__,__LINE__,"size debe ser mayor que cero");
	char tmp[size];
	memcpy(tmp,in,size);
	memcpy(in,out,size);
	memcpy(out,tmp,size);
	free(tmp);
	return out;
}

void free_0(void * e, type * type){
	if(e!=NULL) free(e);
}

void* copy_new_0(void *in, heap *hp, type * t) {
	if(t->size >0 ) return heap_copy(in,hp,t->size);
	else return NULL;
}

void copy_0(void *out, void *in, type * t) {
	if(t->size >0 ) memcpy(out,in,t->size);
}

void * reduce_one_indirection(void * in){
	void ** r = (void **) in;
	void * out = * r;
	return out;
}

// int type

int int_parse_s(char * text){
	int p;
	sscanf(text,"%d",&p);
	return p;
}

int * int_parse(int * out, char * text){
	sscanf(text,"%d",out);
	return out;
}

char * int_tostring(const int * e, char * mem){
    sprintf(mem,"%d",*e);
    return mem;
}

bool int_equals(const int * e1, const int * e2){
    return *e1 == *e2;
}

int int_naturalorder(const int * e1,const int * e2){
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type int_type = {"int",int_equals,int_tostring,int_naturalorder,int_parse,free_0,copy_new_0,copy_0,sizeof(int),0,NULL};

// char type

char * char_parse(char * out, char * text){
	sscanf(text,"%c",out);
	return out;
}

char * char_tostring(const char * e, char * mem){
    sprintf(mem,"%c",*e);
    return mem;
}

bool char_equals(const char * e1, const char * e2){
	char r1 = *e1;
	char r2 = *e2;
	return r1 == r2;
}

int char_naturalorder(const char * e1,const char * e2){
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type char_type = {"char",char_equals, char_tostring, char_naturalorder, char_parse,free_0,copy_new_0,copy_0, sizeof(char),0,NULL};

// bool type

bool * bool_parse(bool * out, char * in) {
	sscanf(in,"%d",out);
	return out;
}

char * bool_tostring(const bool * e, char * out) {
	bool r = *e;
	sprintf(out,"%s",r?"true":"false");
    return out;
}

bool bool_equals(const bool * e1, const bool * e2) {
	bool r1 = *e1;
	bool r2 = *e2;
	return r1 == r2;
}

int bool_naturalorder(const bool * e1,const bool * e2) {
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}


type bool_type = {"bool",bool_equals, bool_tostring, bool_naturalorder, bool_parse,free_0,copy_new_0,copy_0,sizeof(bool),0,NULL};

// long type

long long_parse_s(char * text){
	long p;
	sscanf(text,"%ld",&p);
	return p;
}

long * long_parse(long * out, char * text){
	sscanf(text,"%ld",out);
	return out;
}

char * long_tostring(const long * e, char * mem){
    sprintf(mem,"%ld",*e);
    return mem;
}

bool long_equals(const long * e1,const long * e2){
    return *e1 == *e2;
}

int long_naturalorder(const long * e1,const long * e2){
    long a = *e1;
    long b = *e2;
    long r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}


type long_type = {"long",long_equals,long_tostring,long_naturalorder,long_parse,free_0,copy_new_0,copy_0,sizeof(long),0,NULL};

// long long type

long long long_long_parse_s(char * text){
	long long p;
	sscanf(text,"%lld",&p);
	return p;
}

long long * long_long_parse(long long * out, char * text){
	sscanf(text,"%lld",out);
	return out;
}

char * long_long_tostring(const long long * e, char * mem){
    sprintf(mem,"%lld",*e);
    return mem;
}

bool long_long_equals(const long long * e1,const long long * e2){
    return *e1 == *e2;
}

int long_long_naturalorder(const long long * e1,const long long * e2){
    long a = *e1;
    long b = *e2;
    long r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type long_long_type = {"long_long",long_long_equals,long_long_tostring,long_long_naturalorder,
		long_long_parse,free_0,copy_new_0,copy_0,sizeof(long long),0,NULL};

// float type

float float_parse_s(char * text){
	float p;
	sscanf(text,"%f",&p);
	return p;
}

float * float_parse(float * out, char * text){
	sscanf(text,"%f",out);
	return out;
}

char * float_tostring(const float * e, char * mem){
    sprintf(mem,"%.2f",*e);
    return mem;
}

bool float_equals(const float * e1, const float * e2){
    return *e1 == *e2;
}

int float_naturalorder(const float * e1, const float * e2){
    float a = *e1;
    float b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type float_type = {"float",float_equals,float_tostring,float_naturalorder,float_parse,free_0,
		copy_new_0,copy_0,sizeof(float),0,NULL};

//double type

double double_parse_s(char * text){
	double p;
	sscanf(text,"%lf",&p);
	return p;
}

double * double_parse(double * out, char * text){
	sscanf(text,"%lf",out);
	return out;
}

char * double_tostring(const double * e, char * mem){
    sprintf(mem,"%0.2lf",*e);
    return mem;
}

bool double_equals(const double * e1, const double * e2){
    return *e1 == *e2;
}

int double_naturalorder(const double * e1, const double * e2){
    double a = *e1;
    double b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type double_type = {"double",double_equals,double_tostring,double_naturalorder,double_parse,free_0,
		copy_new_0,copy_0,sizeof(double),0,NULL};

// int_pair

pair_int pair_int_parse_s(char * text){
	pair_int p;
	sscanf(text,"(%d,%d)",&p.a,&p.b);
	return p;
}

pair_int * pair_int_parse(pair_int * out, char * text){
	sscanf(text,"(%d,%d)",&out->a,&out->b);
	return out;
}

char * pair_int_tostring(const pair_int * p, char * mem){
	sprintf(mem,"(%d,%d)",p->a,p->b);
	return mem;
}

bool pair_int_equals(const pair_int * p1, const pair_int * p2){
	return p1->a == p2->a && p1->b == p2->b;
}

int pair_int_naturalorder(const pair_int * p1, const pair_int * p2){
	pair_int np1 = *p1;
	pair_int np2 = *p2;
	int r  = int_naturalorder(&np1.a,&np2.a);
	if(r==0) r  = int_naturalorder(&np1.b,&np2.b);
	return r;
}

pair_int pair_int_of(int a, int b){
	pair_int r = {a,b};
	return r;
}


type pair_int_type = {"pair_int",pair_int_equals,pair_int_tostring,pair_int_naturalorder,pair_int_parse,free_0,
		copy_new_0,copy_0,sizeof(pair_int),0,NULL};

/////
// int_pair

pair_long pair_long_parse_s(char * text){
	pair_long p;
	sscanf(text,"(%ld,%ld)",&p.a,&p.b);
	return p;
}

pair_long * pair_long_parse(pair_long * out, char * text){
	sscanf(text,"(%ld,%ld)",&out->a,&out->b);
	return out;
}

char * pair_long_tostring(const pair_long * p, char * mem){
	sprintf(mem,"(%ld,%ld)",p->a,p->b);
	return mem;
}

bool pair_long_equals(const pair_long * p1, const pair_long * p2){
	return p1->a == p2->a && p1->b == p2->b;
}

int pair_long_naturalorder(const pair_long * p1, const pair_long * p2){
	pair_long np1 = *p1;
	pair_long np2 = *p2;
	int r  = long_naturalorder(&np1.a,&np2.a);
	if(r==0) r  = long_naturalorder(&np1.b,&np2.b);
	return r;
}


type pair_long_type = {"pair_long",pair_long_equals,pair_long_tostring,pair_long_naturalorder,pair_long_parse,free_0,
		copy_new_0,copy_0,sizeof(pair_long),0,NULL};



pair_double pair_double_parse_s(char * text){
	pair_double pt;
	sscanf(text,"(%lf,%lf)",&pt.x,&pt.y);
	return pt;
}

pair_double * pair_double_parse(pair_double * out, char * text){
	sscanf(text,"(%lf,%lf)",&out->x,&out->y);
	return out;
}

char * pair_double_tostring(const pair_double * p, char * mem){
	sprintf(mem,"(%.2lf,%.2lf)",p->x,p->y);
	return mem;
}

bool pair_double_equals(const pair_double * p1, const pair_double * p2){
	return p1->x == p2->x && p1->y == p2->y;
}

int pair_double_naturalorder(const pair_double * p1, const pair_double * p2){
	double d1 = punto_distancia_al_origen(p1);
	double d2 = punto_distancia_al_origen(p2);
	return double_naturalorder(&d1,&d2);
}

type pair_double_type = {"pair_double",pair_double_equals,pair_double_tostring,pair_double_naturalorder,pair_double_parse,free_0,
		copy_new_0,copy_0,sizeof(pair_double),0,NULL};

pair_double double_pair_of(double x, double y){
	pair_double r = {x,y};
	return r;
}


Cuadrante punto_cuadrante(const pair_double * p) {
	Cuadrante r;
	if(p->x >=0 && p->y>=0){
		r = PRIMERO;
	} else if(p->x < 0 && p->y>=0) {
		r = SEGUNDO;
	} else if(p->x < 0 && p->y<0) {
		r = TERCERO;
	} else {
		r = CUARTO;
	}
	return r;
}

punto punto_of(double x, double y){
	punto r = {x,y};
	return r;
}

double punto_distancia_al_origen(const punto * p){
	double x2 = p->x*p->x;
	double y2 = p->y*p->y;
	return sqrt(x2+y2);
}

int punto_ord_distancia_al_origen(const punto * p1, const punto * p2){
	double r1 = punto_distancia_al_origen(p1);
	double r2 = punto_distancia_al_origen(p2);
	return double_naturalorder(&r1,&r2);
}

type punto_type = {"punto",pair_double_equals,pair_double_tostring,
		punto_distancia_al_origen,pair_double_parse,free_0,
		copy_new_0,copy_0,sizeof(pair_double),0,NULL};

// pair type

void * pair_key(void * key, pair * in){
	return in->key;
}

void * pair_value(void * value, pair * in){
	return in->value;
}

pair pair_of(void * key, void * value){
	pair r = {key,value};
	return r;
}

pair * pair_of_value(pair * p, void * value){
	p->key = value;
	p->value = value;
	return p;
}

char pair_delimiters[20] = " ,()";
heap pair_heap = {0,0,NULL};


pair * pair_parse(pair * out, char * text, type * t){
	char * tokens[20];
	string_fix tt;
	string_fix_copy(tt,text,&string_fix_type);
	int n = split_text(tt,pair_delimiters, tokens);
	check_argument(n==2,__FILE__,__LINE__,"el numero de tokens no es 2");
	if(heap_isnull(&pair_heap)) pair_heap = heap_empty();
	out->key = heap_get_memory(&pair_heap,t->types[0]->size);
	out->value = heap_get_memory(&pair_heap,t->types[1]->size);
	parse(out->key,tokens[0],t->types[0]);
	parse(out->value,tokens[1],t->types[1]);
	return out;
}

char * pair_tostring(const pair * e, char * mem, type * t){
	char m1[Tam_String];
	char m2[Tam_String];
	tostring(e->key,m1,t->types[0]);
	tostring(e->value,m2,t->types[1]);
	sprintf(mem,"(%s,%s)",m1,m2);
	return mem;
}

bool pair_equals(const pair * e1, const pair * e2, type * t){
	return equals(e1->key,e2->key,t->types[0]) &&
			equals(e1->value,e2->value, t->types[1]);
}

int pair_naturalorder(const pair * e1, const pair * e2, type * t){
	int r = order(e1->key,e2->key,t->types[0]);
	if(r==0) r = order(e1->value,e2->value,t->types[1]);
	return r;
}


void pair_free(pair * p, type * t){
	type * t0 = t->types[0];
	type * t1 = t->types[1];
	t0->free(p->key,t0);
	t1->free(p->value,t1);
	free(p);
}

pair * pair_copy_new(pair * p, heap * hp, type * t) {
	type * t0 = t->types[0];
	type * t1 = t->types[1];
	void * key = p!=NULL?p->key:NULL;
	void * value = p!=NULL?p->value:NULL;
	void * k = t0->copy_new(key,hp,t0);
	void * v = t1->copy_new(value,hp,t1);
	pair pc = pair_of(k,v);
	return (pair *) heap_copy(&pc,hp,sizeof(pair));
}

void pair_copy(pair * out, pair * in, type * t) {
	type * t0 = t->types[0];
	type * t1 = t->types[1];
	t0->copy(out->key,in->key,t0);
	t1->copy(out->value,in->value,t1);
}


type pair_type = {"pair",pair_equals,pair_tostring,pair_naturalorder,pair_parse,pair_free,
		pair_copy_new,pair_copy,sizeof(pair),2,NULL};

// pair_enumerate

enumerate enumerate_of(int counter, void * value){
	enumerate p = {counter,value};
	return p;
}

enumerate * enumerate_parse(enumerate * out, char * text, type * t) {
	char * tokens[20];
	string_fix tt;
	string_fix_copy(tt, text,&string_fix_type);
	int n = split_text(tt, pair_delimiters, tokens);
	check_argument(n == 2, __FILE__, __LINE__, "el numero de tokens no es 2");
	out->value = heap_get_memory(&pair_heap,t->types[0]->size);
	sscanf(tokens[0],"%d",&out->counter);
	parse(out->value, tokens[1], t->types[0]);
	return out;
}

char * enumerate_tostring(const enumerate * e, char * mem, type * t){
	char a[Tam_String];
	tostring(e->value,a,t->types[0]);
	sprintf(mem,"(%d,%s)",e->counter,a);
	return mem;
}
bool enumerate_equals(const enumerate * e1, const enumerate * e2, type * t){
	bool k = equals(e1->value,e2->value,t->types[0]);
	return k && e1->counter == e2->counter;
}

int enumerate_naturalorder(const enumerate * e1, const enumerate * e2, type * t){
	int k = order(e1->value,e2->value,t->types[0]);
	if(k == 0) k = order(&e1->counter,&e2->counter,&int_type);
    return k;
}

void enumerate_free(enumerate * e, type * t){
	type * te = t->types[0];
	te->free(e->value,te);
	free(e);
}

enumerate * enumerate_copy_new(enumerate * e, heap * hp, type * t) {
	type * t0 = t->types[0];
	void * v = e!=NULL?e->value:NULL;
	int c = e!=NULL?e->counter:0;
	void * value = t0->copy_new(v,hp,t0);
	enumerate en = enumerate_of(c,value);
	return (enumerate *) heap_copy(&en,hp,sizeof(enumerate));
}

void enumerate_copy(enumerate * out, enumerate * in, type * t) {
	type * t0 = t->types[0];
	t0->copy(out->value,in->value,t0);
	memcpy(&out->counter,&in->counter,int_type.size);
}


type enumerate_type = {"enumerate",enumerate_equals,enumerate_tostring,enumerate_naturalorder,
		enumerate_parse,enumerate_free,enumerate_copy_new,enumerate_copy,sizeof(enumerate),1,NULL};

// optional type

optional * optional_parse(optional * out, char * text, type * t){
	parse(out->value,text,t->types[0]);
	return out;
}
char * optional_tostring(const optional * e, char * mem, type * t){
	if(e->value == NULL){
		strcpy(mem,"null");
	} else {
		tostring(e->value,mem,t->types[0]);
	}
	return mem;
}
bool optional_equals(const optional * e1, const optional * e2, type * t){
	return equals(e1->value,e2->value,t->types[0]);
}
int optional_naturalorder(const optional * e1, const optional * e2, type * t){
	return order(e1->value,e2->value,t->types[0]);
}


optional optional_of(void * in){
	optional r = {in};
	return r;
}

void optional_free(optional * e, type * t){
	type * te = t->types[0];
	te->free(e->value,te);
	free(e);
}

optional * optional_copy_new(optional * e, heap * hp, type * t) {
	type * te = t->types[0];
	void * v = e!=NULL?e->value:NULL;
	void * value = te->copy_new(v,hp,te);
	optional en = optional_of(value);
	return (optional *) heap_copy(&en,hp,sizeof(optional));
}

void optional_copy(optional * out, optional * in, type * t) {
	type * te = t->types[0];
	copy(out->value,in->value,te);
}

type optional_type = {"optional",optional_equals,optional_tostring,optional_naturalorder,optional_parse,optional_free, optional_copy_new, optional_copy,sizeof(optional),1,NULL};

// string type

#define INITIAL_TAM 250
#define INC_TAM 500


string_var string_var_empty(){
	void * data = malloc(INITIAL_TAM);
	string_var st = {INITIAL_TAM,0,data};
	strcpy(st.data,"");
	return st;
}

char * string_var_data(string_var * in){
	return in->data;
}

bool string_var_is_empty(string_var * in){
	return strlen(in->data) == 0;
}

bool string_var_not_is_empty(string_var * in){
	return strlen(in->data) > 0;
}

int string_var_size(string_var * in){
	return strlen(in->data);
}

string_var string_var_of_fix(const char * initial){
	string_var bf = string_var_empty();
	string_var_add_string_fix(&bf,initial);
	return bf;
}

void * string_var_add_string_fix(string_var * out, const char * s) {
	check_not_null(s,__FILE__,__LINE__,"Cadena null");
	int n = strlen(s);
	if(out->size+n>out->tam){
		int tm = MAX(out->size+n,out->tam+INC_TAM);
		out->data = realloc(out->data,tm);
		check_not_null(out->data,__FILE__,__LINE__,"No ha sido posible aumentar la memoria");
		out->tam = tm;
	}
	if(out->size == 0) strcpy(out->data,s);
	else strcat(out->data,s);
	out->size = out->size +n;
	return out->data;
}

void * string_var_add_string_var(string_var * out, const string_var * in) {
	return string_var_add_string_fix(out,in->data);
}

void string_var_clear(string_var * in){
	free(in->data);
	void * data = malloc(INITIAL_TAM);
	in->tam = INITIAL_TAM;
	in->size = 0;
	in->data = data;
	strcpy(in->data,"");
}

void string_var_free(string_var * in, type * t){
	string_var * st = (string_var * ) in;
	free(st->data);
}

string_var * string_var_parse(string_var * out, char * text){
	string_var_add_string_fix(out,text);
	return out;
}

char * string_var_tostring(const string_var * e, char * mem) {
	strcpy(mem,e->data);
	return mem;
}

bool string_var_equals(const string_var * e1, const string_var * e2) {
	return strcmp(e1->data, e2->data) == 0;
}

int string_var_naturalorder(const string_var * e1, const string_var * e2) {
	return strcmp(e1->data, e2->data);
}

string_var * string_var_copy_new(string_var * e, heap * hp, type * t){
	return NULL;
}

string_var * string_var_copy(string_var * out, string_var * in, type * t){
	return NULL;
}


type string_var_type = {"string_var",string_var_equals,string_var_tostring,string_var_naturalorder,string_var_parse,string_var_free,
		string_var_copy_new,string_var_copy,sizeof(string_var),0,NULL};


// string_fix_type
// Las variables de este tipo se declaran char v[Tam_String]
// y se referencian como char *

char * remove_eol(char * in){
	int len = strlen(in);
	if (len>0 && in[len-1] == 10) {
		in[len-1] = '\0';
	}
	return in;
}


int split_text(const char * in, const char * delimiters, char ** tokens){
	int i = 0;
	char * token = strtok(in,delimiters);
	while(token != NULL) {
		tokens[i] = token;
		token = strtok(NULL,delimiters);
		i++;
	}
	return i;
}

char * string_fix_substring(char * out, char * in, int from, int to){
	to = MIN(strlen(in),to);
	int n = to - from;
	strncpy(out,in+from,n);
	*(out+n) = '\0';
	return out;
}

int string_fix_size(const char * in){
	return strlen(in);
}

char char_get(const char * in, int i){
	int n = strlen(in);
	check_element_index(i,n,__FILE__,__LINE__);
	return in[i];
}

char char_set(char * in_out, int i, char c){
	int n = strlen(in_out);
	check_element_index(i,n,__FILE__,__LINE__);
	char r = in_out[i];
	in_out[i]=c;
	return r;
}

//versión reentrante de strtok

char* strtok_r2(char *str, const char *delim, char **save_pointer) {
	char *ret;
	if (str == NULL) {
		str = *save_pointer;
	}
	str += strspn(str, delim);
	if (*str == '\0') {
		return NULL;
	}
	ret = str;
	str += strcspn(str, delim);
	if (*str) {
		*str++ = '\0';
	}
	*save_pointer = str;
	return ret;
}

char * string_fix_concat(char * out, const char * in, type * t){
	int n1 = strlen(in);
	int n2 = strlen(out);
	check_argument(n1+n2<t->size,__FILE__,__LINE__,"La cadena no cabe en la memoria reservada");
	strcat(out,in);
	return out;
}

char * string_fix_copy(char * out, const char * in, type * t){
	int n1 = strlen(in);
	check_argument(n1<t->size,__FILE__,__LINE__,"La cadena no cabe en la memoria reservada");
	strcpy(out,in);
	return out;
}

char * string_fix_parse(char * out, char * text){
	strcpy(out,text);
	return out;
}

char * string_fix_tostring(const char * e, char * mem){
	strcpy(mem,e);
	return mem;
}

bool string_fix_equals(const char * e1, const char * e2){
	return strcmp(e1,e2) == 0;
}

int string_fix_naturalorder(const char * e1,const char * e2){
	return strcmp(e1,e2);
}

int string_fix_tam = Tam_String;

type string_fix_type = {"string_fix",string_fix_equals,string_fix_tostring,string_fix_naturalorder,string_fix_parse,free_0,
		copy_new_0,copy_0,Tam_String,0,NULL};

type string_fix_type_of_tam(int numchars) {
	type r = string_fix_type;
	r.size = numchars;
	return r;
}

bool string_fix_is_empty(char * in){
	return strlen(in) == 0;
}
bool string_fix_not_is_empty(char * in){
	return strlen(in) > 0;
}

bool string_fix_all_space(char * in){
	bool r = true; int i =-1;
	while(in[++i] != '\0' && r)
		r = isspace(in[i]);
	return r;
}

bool string_fix_not_all_space(char * in){
	return !string_fix_all_space(in);
}



// null type

type null_type = {"null",NULL,NULL,NULL,NULL,free_0,copy_new_0,copy_0,0};



void test_string(){
	char mem[256];
	char delimiters[] = " ,;.";
	char text[600] = "El Gobierno abre la puerta a no;llevar los Presupuestos.Generales de 2019 al Congreso si no logra los apoyos suficientes para sacarlos adelante. Esa opción que ya deslizaron fuentes próximas al presidente la ha confirmado la portavoz, Isabel Celaá, en la rueda de prensa posterior a la reunión del gabinete en la que ha asegurado que el Consejo de Ministras tomará la decisión sobre llevar o no las cuentas públicas al Parlamento una vez concluyan las    negociaciones de la ministra María Jesús Montero. ";
	char * tokens[100];
	int n = split_text(text, delimiters,tokens);
	printf("1: %d\n",n);
	for(int i =0;i<n;i++){
		printf("2: %d,%s\n",i,tokens[i]);
	}
	char te[] = "(3,4.7,-6)";
	int a;
	double b;
	int c;
	sscanf(te,"(%d,%lf,%d)",&a,&b,&c);
	printf("3: %d,%.2lf,%d\n",a,b,c);
	char pt[] = "(3.4,-7.9)";
	pair_double p = pair_double_parse_s(pt);
	printf("4: %s\n",pair_double_tostring(&p,mem));
	char pa[] = "(4,-9)";
	pair_int pp = pair_int_parse_s(pa);
	printf("5: %s\n",tostring(&pp,mem,&pair_int_type));
	char tt2[200] = "34 389   23.5 -37.90 (3,-5) (34.1,-67.8)";
	char delimiters2[] = " ";
	char * tt[10];
	n = split_text(tt2,delimiters2,tt);
	printf("6: %d\n",n);
	for(int i =0;i<n;i++){
		printf("%d,%s\n",i,tt[i]);
	}
	int a1 = int_parse_s(tt[0]);
	long b1 = long_parse_s(tt[1]);
	float c1 = float_parse_s(tt[2]);
	double d1 = double_parse_s(tt[3]);
	pair_int e1 = pair_int_parse_s(tt[4]);
	pair_double f1 = pair_double_parse_s(tt[5]);
	printf("8: %d,%ld,%f,%lf\n",a1,b1,c1,d1);
	printf("9: %s\n",tostring(&e1,mem,&pair_int_type));
	printf("10: %s\n",tostring(&f1,mem,&pair_double_type));
	char aa[] = "En un lugar de la mancha de cuyo nombre no quiero acordarme";
	char * s = string_fix_substring(mem,aa,10,13);
	printf("%s\n",aa);
	printf("%s\n",s);
	string_fix d[] = {"Juan","Antonio","Jesus"};
	printf("%s\n",d[1]);
	printf("%d,%d,%d",string_fix_size(d[0]),string_fix_size(d[1]),string_fix_size(d[2]));
}

void test_types() {
	char mem[256];
	double a = 4;
	double b = 17;
	double * c = &b;
	double d = 18;
	copy_size(&a,&d,sizeof(double));
	printf("a = %lf,d = %lf\n",a,d);
	copy_in_pointer(&c,&d,sizeof(double));
	printf("d = %lf,b = %lf\n",d,b);
	int n = 0;
	char tt2[] = "34 389   23.5 -37.90 (3,-5) (34.1,-67.8)";
	char delimiters2[] = " ";
	char * tt[10];
	n = split_text(tt2, delimiters2,tt);
	printf("6: %d\n", n);
	for (int i = 0; i < n; i++) {
		printf("7: %s\n", tt[i]);
	}
	string_fix e;
	string_fix_copy(e,"Juego de tronos",&string_fix_type);
	printf("7: %s\n", tostring(e,mem,&string_fix_type));
	long long g;
	parse(&g,"456",&long_long_type);
	long long g2;
	parse(&g2,"4569999",&long_long_type);
	long long g3 = g+g2;
	tostring(&g3,mem,&long_long_type);
	printf("8: %s\n",mem);
}

void test_types_1() {
	char mem[256];
	char mem2[256];
	string_fix text = "(0,1)";
	double a;
	long b;
	pair p = { &a, &b };
	char * tokens[10];
	int n = split_text(text, " ,()", tokens);
	printf("1: %d,%s,%s\n", n, tokens[0], tokens[1]);
	type t = generic_type_2(&pair_type, &double_type, &long_type);
	parse(&p, "(34.56,56)", &t);
	printf("2: %s\n", tostring(&p, mem, &t));
	printf("3: %d,%d\n", types(&t, 0)->size, types(&t, 1)->size);
	type t2 = generic_type_1(&enumerate_type, &double_type);
	enumerate p2 = { 7, &a };
	parse(&p2, "(5667,-55.12)", &t2);
	printf("4: %s\n", tostring(&p2, mem, &t2));
	type t3 = generic_type_1(&optional_type, &double_type);
	optional p3 = { &a };
	parse(&p3, "34.56", &t3);
	printf("5: %s\n", tostring(&p3, mem, &t3));
}

void test_generic_types_0() {
	 type * t = &long_long_type;
	 printf("%s,%d",t->name,t->size);
}

void test_generic_types_1() {
	char mem[200];
	int a = 2;
	double b = 3.;
	void * a2 = copy_new(NULL,NULL,&int_type);
	void * b2 = copy_new(NULL,NULL,&double_type);
	copy(a2,&a,&int_type);
	copy(b2,&b,&double_type);
	printf("%s\n",int_type.tostring(a2,mem,&int_type));
	printf("%s\n",double_type.tostring(b2,mem,&double_type));
}

void test_generic_types_2() {
	char mem[200];
	int a = 2;
	double b = 3.;
	pair p = pair_of(&a,&b);
	enumerate e = enumerate_of(10,&p);
	type pt = generic_type_2(&pair_type,&int_type,&double_type);
	type et = generic_type_1(&enumerate_type,&pt);
	printf("%s\n",et.tostring(&e,mem,&et));
	enumerate * e2 = copy_new(&e,NULL,&et);
	printf("%s\n",et.tostring(e2,mem,&et));
	enumerate * e3 = copy_new(NULL,NULL,&et);
	copy(e3,e2,&et);
	printf("%s\n",et.tostring(e3,mem,type_copy(&et,NULL)));
}




