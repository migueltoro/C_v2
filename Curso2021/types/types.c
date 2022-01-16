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
	char * r = tostring(in, mem,t);
	unsigned long int hash_index = hash(r);
	return hash_index;
}


type * type_copy(type * t) {
	return copy_and_mem(t,sizeof(type));
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


void type_free(type *t) {
	if (t->num_types == 1)
		free(t->types[0]);
	else if (t->num_types == 2) {
		free(t->types[0]);
		free(t->types[1]);
	}
}

int num_types(type * t){
	return t->num_types;
}

type * types(type * base, int i){
	return base->types[i];
}


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

type tmp_type;

void set_tmp_type(type t){
	tmp_type = t;
}

bool equals_st(const void * e1, const void * e2) {
	return tmp_type.equals(e1,e2,&tmp_type);
}

char * tostring_st(const void * e, char * mem) {
	return tmp_type.tostring(e,mem,&tmp_type);
}

int order_st(const void * e1, const void * e2){
	return tmp_type.order(e1,e2,&tmp_type);
}

void * parse_st(void * out, char * text){
	return tmp_type.parse(out,text,&tmp_type);
}



// int type

int int_parse_s(char * text){
	int p;
	sscanf(text,"%d",&p);
	return p;
}

int * int_parse(int * out, char * text, type * type){
	sscanf(text,"%d",out);
	return out;
}

char * int_tostring(const int * e, char * mem, type * type){
    sprintf(mem,"%d",*e);
    return mem;
}

bool int_equals(const int * e1, const int * e2, type * type){
    return *e1 == *e2;
}

int int_naturalorder(const int * e1,const int * e2, type * type){
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type int_type = {int_equals,int_tostring,int_naturalorder,int_parse,sizeof(int),0,NULL};

// char type

char * char_parse(char * out, char * text, type * type){
	sscanf(text,"%c",out);
	return out;
}

char * char_tostring(const char * e, char * mem, type * type){
    sprintf(mem,"%c",*e);
    return mem;
}

bool char_equals(const char * e1, const char * e2, type * type){
	char r1 = *e1;
	char r2 = *e2;
	return r1 == r2;
}

int char_naturalorder(const char * e1,const char * e2, type * type){
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type char_type = {char_equals, char_tostring, char_naturalorder, char_parse, sizeof(char),0,NULL};

// bool type

bool * bool_parse(bool * out, char * in, type * type) {
	sscanf(in,"%d",out);
	return out;
}

char * bool_tostring(const bool * e, char * out, type * type) {
	bool r = *e;
	sprintf(out,"%s",r?"true":"false");
    return out;
}

bool bool_equals(const bool * e1, const bool * e2, type * type) {
	bool r1 = *e1;
	bool r2 = *e2;
	return r1 == r2;
}

int bool_naturalorder(const bool * e1,const bool * e2, type * type) {
    int a = *e1;
    int b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}


type bool_type = {bool_equals, bool_tostring, bool_naturalorder, bool_parse, sizeof(bool),0,NULL};

// long type

long long_parse_s(char * text){
	long p;
	sscanf(text,"%ld",&p);
	return p;
}

long * long_parse(long * out, char * text, type * type){
	sscanf(text,"%ld",out);
	return out;
}

char * long_tostring(const long * e, char * mem, type * type){
    sprintf(mem,"%ld",*e);
    return mem;
}

bool long_equals(const long * e1,const long * e2, type * type){
    return *e1 == *e2;
}

int long_naturalorder(const long * e1,const long * e2, type * type){
    long a = *e1;
    long b = *e2;
    long r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}


type long_type = {long_equals,long_tostring,long_naturalorder,long_parse,sizeof(long),0,NULL};

// long long type

long long long_long_parse_s(char * text){
	long long p;
	sscanf(text,"%lld",&p);
	return p;
}

long long * long_long_parse(long long * out, char * text, type * type){
	sscanf(text,"%lld",out);
	return out;
}

char * long_long_tostring(const long long * e, char * mem, type * type){
    sprintf(mem,"%lld",*e);
    return mem;
}

bool long_long_equals(const long long * e1,const long long * e2, type * type){
    return *e1 == *e2;
}

int long_long_naturalorder(const long long * e1,const long long * e2, type * type){
    long a = *e1;
    long b = *e2;
    long r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type long_long_type = {long_long_equals,long_long_tostring,long_long_naturalorder,long_long_parse,sizeof(long long),0,NULL};

// float type

float float_parse_s(char * text){
	float p;
	sscanf(text,"%f",&p);
	return p;
}

float * float_parse(float * out, char * text, type * type){
	sscanf(text,"%f",out);
	return out;
}

char * float_tostring(const float * e, char * mem, type * type){
    sprintf(mem,"%.2f",*e);
    return mem;
}

bool float_equals(const float * e1, const float * e2, type * type){
    return *e1 == *e2;
}

int float_naturalorder(const float * e1, const float * e2, type * type){
    float a = *e1;
    float b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type float_type = {float_equals,float_tostring,float_naturalorder,float_parse,sizeof(float),0,NULL};

//double type

double double_parse_s(char * text){
	double p;
	sscanf(text,"%lf",&p);
	return p;
}

double * double_parse(double * out, char * text, type * type){
	sscanf(text,"%lf",out);
	return out;
}

char * double_tostring(const double * e, char * mem, type * type){
    sprintf(mem,"%0.2lf",*e);
    return mem;
}

bool double_equals(const double * e1, const double * e2, type * type){
    return *e1 == *e2;
}

int double_naturalorder(const double * e1, const double * e2, type * type){
    double a = *e1;
    double b = *e2;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type double_type = {double_equals,double_tostring,double_naturalorder,double_parse,sizeof(double),0,NULL};

// int_pair

int_pair int_pair_parse_s(char * text){
	int_pair p;
	sscanf(text,"(%d,%d)",&p.a,&p.b);
	return p;
}

int_pair * int_pair_parse(int_pair * out, char * text, type * type){
	sscanf(text,"(%d,%d)",&out->a,&out->b);
	return out;
}

char * int_pair_tostring(const int_pair * p, char * mem, type * type){
	sprintf(mem,"(%d,%d)",p->a,p->b);
	return mem;
}

bool int_pair_equals(const int_pair * p1, const int_pair * p2, type * type){
	return p1->a == p2->a && p1->b == p2->b;
}

int int_pair_naturalorder(const int_pair * p1, const int_pair * p2, type * type){
	int_pair np1 = *p1;
	int_pair np2 = *p2;
	int r  = int_naturalorder(&np1.a,&np2.a,NULL);
	if(r==0) r  = int_naturalorder(&np1.b,&np2.b,NULL);
	return r;
}



type int_pair_type = {int_pair_equals,int_pair_tostring,int_pair_naturalorder,int_pair_parse,sizeof(int_pair),0,NULL};

/////
// int_pair

long_pair long_pair_parse_s(char * text){
	long_pair p;
	sscanf(text,"(%ld,%ld)",&p.a,&p.b);
	return p;
}

long_pair * long_pair_parse(long_pair * out, char * text, type * type){
	sscanf(text,"(%ld,%ld)",&out->a,&out->b);
	return out;
}

char * long_pair_tostring(const long_pair * p, char * mem, type * type){
	sprintf(mem,"(%ld,%ld)",p->a,p->b);
	return mem;
}

bool long_pair_equals(const long_pair * p1, const long_pair * p2, type * type){
	return p1->a == p2->a && p1->b == p2->b;
}

int long_pair_naturalorder(const long_pair * p1, const long_pair * p2, type * type){
	long_pair np1 = *p1;
	long_pair np2 = *p2;
	int r  = long_naturalorder(&np1.a,&np2.a,NULL);
	if(r==0) r  = long_naturalorder(&np1.b,&np2.b,NULL);
	return r;
}


type long_pair_type = {long_pair_equals,long_pair_tostring,long_pair_naturalorder,long_pair_parse,sizeof(long_pair),0,NULL};

/////
Cuadrante punto_cuadrante(const punto * p) {
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

punto punto_parse_s(char * text){
	punto pt;
	sscanf(text,"(%lf,%lf)",&pt.x,&pt.y);
	return pt;
}

punto * punto_parse(punto * out, char * text, type * type){
	sscanf(text,"(%lf,%lf)",&out->x,&out->y);
	return out;
}

double punto_distancia_al_origen(const punto * p){
	double x2 = p->x*p->x;
	double y2 = p->y*p->y;
	return sqrt(x2+y2);
}

char * punto_tostring(const punto * p, char * mem, type * type){
	sprintf(mem,"(%.2lf,%.2lf)",p->x,p->y);
	return mem;
}

bool punto_equals(const punto * p1, const punto * p2, type * type){
	return p1->x == p2->x && p1->y == p2->y;
}

int punto_naturalorder(const punto * p1, const punto * p2, type * type){
	double d1 = punto_distancia_al_origen(p1);
	double d2 = punto_distancia_al_origen(p2);
	return double_naturalorder(&d1,&d2,NULL);
}

type punto_type = {punto_equals,punto_tostring,punto_naturalorder,punto_parse,sizeof(punto),0,NULL};


// pair type

void * pair_to_key(void * key, pair * in){
	return in->key;
}

void * pair_to_value(void * value, pair * in){
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
memory_heap pair_memory_heap = {0,0,NULL};


pair * pair_parse(pair * out, char * text, type * t){
	char * tokens[20];
	string_fix tt;
	string_fix_copy(tt,text);
	int n = split_text(tt,pair_delimiters, tokens);
	check_argument(n==2,__FILE__,__LINE__,"el numero de tokens no es 2");
	if(memory_heap_isnull(&pair_memory_heap)) pair_memory_heap = memory_heap_create();
	out->key = memory_heap_get_memory(&pair_memory_heap,t->types[0]->size);
	out->value = memory_heap_get_memory(&pair_memory_heap,t->types[1]->size);
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


void pair_free(pair * p){
	if(p->key != NULL) free(p->key);
	if(p->value != NULL) free(p->value);
}


type pair_type = {pair_equals,pair_tostring,pair_naturalorder,pair_parse,sizeof(pair),2,NULL};

// pair_enumerate

pair_enumerate pair_enumerate_of(int counter, void * value){
	pair_enumerate p = {counter,value};
	return p;
}

pair_enumerate * pair_enumerate_parse(pair_enumerate * out, char * text, type * t) {
	char * tokens[20];
	string_fix tt;
	string_fix_copy(tt, text);
	int n = split_text(tt, pair_delimiters, tokens);
	check_argument(n == 2, __FILE__, __LINE__, "el numero de tokens no es 2");
	out->value = memory_heap_get_memory(&pair_memory_heap,t->types[0]->size);
	sscanf(tokens[0],"%d",&out->counter);
	parse(out->value, tokens[1], t->types[0]);
	return out;
}

char * pair_enumerate_tostring(const pair_enumerate * e, char * mem, type * t){
	char a[Tam_String];
	tostring(e->value,a,t->types[0]);
	sprintf(mem,"(%d,%s)",e->counter,a);
	return mem;
}
bool pair_enumerate_equals(const pair_enumerate * e1, const pair_enumerate * e2, type * t){
	bool k = equals(e1->value,e2->value,t->types[0]);
	return k && e1->counter == e2->counter;
}

int pair_enumerate_naturalorder(const pair_enumerate * e1, const pair_enumerate * e2, type * t){
	int k = order(e1->value,e2->value,t->types[0]);
	if(k == 0) k = order(&e1->counter,&e2->counter,&int_type);
    return k;
}


type pair_enumerate_type = {pair_enumerate_equals,pair_enumerate_tostring,pair_enumerate_naturalorder,pair_enumerate_parse,sizeof(pair_enumerate),1,NULL};

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
	string_var_add_string(&bf,initial);
	return bf;
}

void * string_var_add_string(string_var * out, const char * s) {
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
	return string_var_add_string(out,in->data);
}

void string_var_clear(string_var * in){
	free(in->data);
	void * data = malloc(INITIAL_TAM);
	in->tam = INITIAL_TAM;
	in->size = 0;
	in->data = data;
	strcpy(in->data,"");
}

void string_var_free(string_var * in){
	string_var * st = (string_var * ) in;
	free(st->data);
}

string_var * string_var_parse(string_var * out, char * text, type * type){
	string_var_add_string(out,text);
	return out;
}

char * string_var_tostring(const string_var * e, char * mem, type * type) {
	strcpy(mem,e->data);
	return mem;
}

bool string_var_equals(const string_var * e1, const string_var * e2, type * type) {
	return strcmp(e1->data, e2->data) == 0;
}

int string_var_naturalorder(const string_var * e1, const string_var * e2, type * type) {
	return strcmp(e1->data, e2->data);
}


type string_var_type = {string_var_equals,string_var_tostring,string_var_naturalorder,string_var_parse,sizeof(string_var),0,NULL};


// pchar type
// Las variables de este tipo se declaran char v[Tam_String]
// y se referencian como char *

char * remove_eol(char * in){
	int len = strlen(in);
	if (len>0 && in[len-1] == 10) {
		in[len-1] = '\0';
	}
	return in;
}

//char *  array_char_remove_eol(char * out, char * in){
//	strcpy(out,in);
//	remove_eol_s(out);
//	return out;
//}


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


char * pchar_concat(char * out, const char * in){
	strcat(out,in);
	return out;
}

char * string_fix_copy(char * out, const char * in){
	strcpy(out,in);
	return out;
}


char * string_fix_parse(char * out, char * text, type * type){
	strcpy(out,text);
	return out;
}

char * string_fix_tostring(const char * e, char * mem, type * type){
	strcpy(mem,e);
	return mem;
}

bool string_fix_equals(const char * e1, const char * e2, type * type){
	return strcmp(e1,e2) == 0;
}

int string_fix_naturalorder(const char * e1,const char * e2, type * type){
	return strcmp(e1,e2);
}

type string_fix_type = {string_fix_equals,string_fix_tostring,string_fix_naturalorder,string_fix_parse,Tam_String,0,NULL};

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
	bool r = true;
	int i =0;
	while(in[i] != '\0') {
		r = isspace(in[i]);
		if(!r) break;
		i=i+1;
	}
	return r;
}
bool string_fix_not_all_space(char * in){
	return !string_fix_all_space(in);
}

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

type optional_type = {optional_equals,optional_tostring,optional_naturalorder,optional_parse,sizeof(optional),1,NULL};

// null type

type null_type = {NULL,NULL,NULL,NULL,0};

// utilities

void * copy(void * out, void * in, int size){
	check_not_null(in,__FILE__,__LINE__,"puntero null");
	check_not_null(out,__FILE__,__LINE__,"puntero null");
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

void * copy_and_mem(void * in, int size) {
	check_not_null(in, __FILE__, __LINE__, "puntero null");
	if (size == 0) return NULL;
	void * element = (void *) malloc(size);
	memcpy(element, in, size);
	return element;
}

void * swap(void * out, void * in, int size){
	check_not_null(in,__FILE__,__LINE__,"puntero null");
	check_not_null(out,__FILE__,__LINE__,"puntero null");
	check_argument(size>0,__FILE__,__LINE__,"size debe ser mayor que cero");
	void * tmp = malloc(size);
	memcpy(tmp,in,size);
	memcpy(in,out,size);
	memcpy(out,tmp,size);
	free(tmp);
	return out;
}

void * reduce_one_indirection(void * in){
	void ** r = (void **) in;
	void * out = * r;
	return out;
}


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
	punto p = punto_parse_s(pt);
	printf("4: %s\n",punto_tostring(&p,mem,NULL));
	char pa[] = "(4,-9)";
	int_pair pp = int_pair_parse_s(pa);
	printf("5: %s\n",tostring(&pp,mem,&int_pair_type));
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
	int_pair e1 = int_pair_parse_s(tt[4]);
	punto f1 = punto_parse_s(tt[5]);
	printf("8: %d,%ld,%f,%lf\n",a1,b1,c1,d1);
	printf("9: %s\n",tostring(&e1,mem,&int_pair_type));
	printf("10: %s\n",tostring(&f1,mem,&punto_type));
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
	copy(&a,&d,sizeof(double));
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
	string_fix_copy(e,"Juego de tronos");
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
	char text[256] = "(0,1)";
	double a;
	long b;
	pair p = { &a, &b };
	char * tokens[10];
	int n = split_text(text, " ,()", tokens);
	printf("1: %d,%s,%s\n", n, tokens[0], tokens[1]);
	type t = generic_type_2(&pair_type, &double_type, &long_type);
	parse(&p, "(34,56)", &t);
	printf("2: %s\n", tostring(&p, mem, &t));
	printf("3: %d,%d\n", types(&t, 0)->size, types(&t, 1)->size);
	type t2 = generic_type_1(&pair_enumerate_type, &double_type);
	pair_enumerate p2 = { 7, &a };
	parse(&p2, "(5667,-55.12)", &t2);
	printf("4: %s\n", tostring(&p2, mem, &t2));
	type t3 = generic_type_1(&optional_type, &double_type);
	optional p3 = { &a };
	parse(&p3, "34.56", &t3);
	printf("5: %s\n", tostring(&p3, mem, &t3));
}


