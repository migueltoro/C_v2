/*
 * types.h
 *
 *  Created on: 4 nov. 2018
 *      Author: migueltoro
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <float.h>
#include <ctype.h>

#include "../types/preconditions.h"
#include "../types/memory_heap.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


typedef struct gtp {
	bool (*equals)(const void * e1, const void * e2, struct gtp * t);
	char * (*tostring)(const void * e, char * mem, struct gtp * t);
	int (*order)(const void * e1, const void * e2, struct gtp * t);
	void * (*parse)(void * out, char * text, struct gtp * t);
	int size;
	int num_types;
	struct gtp * types[2];
}type;


unsigned long int hash(const char * key);
unsigned long int hash_code(void * in, type * t);



bool type_equals(const type * e1, const type* e2);
type generic_type_1(type * base, type * key_type);
type generic_type_2(type *base, type * key_type, type * value_type);
int num_types(type * t);
type * types(type * base, int i);
void type_free(type * t);


bool equals(const void * e1, const void * e2, type * type);
char * tostring(const void * e, char * mem, type * type);
int order(const void * e1, const void * e2, type * type);
void * parse(void * out, char * text, type * type);

extern type tmp_type;

void set_tmp_type(type t);
bool equals_st(const void * e1, const void * e2);
char * tostring_st(const void * e, char * mem);
int order_st(const void * e1, const void * e2);
void * parse_st(void * out, char * text);

bool equals_type(const void * e1, const void * e2);
char * tostring_type(const void * e, char * mem);
int order_type(const void * e1, const void * e2);
void * parse_type(void * out, char * text);

// int s_type

int int_parse_s(char * text);
int * int_parse(int * out, char * text, type * type);
char * int_tostring(const int * e, char * mem, type * type);
bool int_equals(const int * e1, const int * e2, type * type);
int int_naturalorder(const int * e1, const int * e2, type * type);

#define to_int(a) *(int *) a

extern type int_type;

char* char_parse(char * out, char * text, type * type);
char* char_tostring(const char * e, char * mem, type * type);
bool char_equals(const char * e1, const char * e2, type * type);
int char_naturalorder(const char * e1, const char * e2, type * type);

#define to_char(a) *(char *) a

extern type char_type;

#ifndef MSG_BOOL
#define MSG_BOOL(x) (x==0?"false":"true")
#endif

bool* bool_parse(bool * out, char * in, type * type);
char* bool_tostring(const bool * e, char * out, type * type);
bool bool_equals(const bool * e1, const bool * e2, type * type);
int bool_naturalorder(const bool * e1,const bool * e2, type * type);

#define to_bool(a) *(bool *) a

extern type bool_type;

// long s_type

long long_parse_s(char * text);
long * long_parse(long * out, char * text, type * type);
char * long_tostring(const long * e, char * mem, type * type);
bool long_equals(const long * e1, const long * e2, type * type);
int long_naturalorder(const long * e1, const long * e2, type * type);

#define to_long(a) *(long *) a

extern type long_type;

// long long s_type

long long long_long_parse_s(char * text);
long long * long_long_parse(long long * out, char * text, type * type);
char * long_long_tostring(const long long * e, char * mem, type * type);
bool long_long_equals(const long long * e1, const long long * e2, type * type);
int long_long_naturalorder(const long long * e1, const long long * e2, type * type);

extern type long_long_type;

// float s_type

float float_parse_s(char * text);
float * float_parse(float * out, char * text, type * type);
char * float_tostring(const float * e, char * mem, type * type);
bool float_equals(const float * e1, const float * e2, type * type);
int float_naturalorder(const float * e1,const float * e2, type * type);

#define to_float(a) *(float *) a

extern type float_type;

// double s_type

double double_parse_s(char * text);
double * double_parse(double * out, char * text, type * type);
char * double_tostring(const double * e, char * mem, type * type);
bool double_equals(const double * e1, const double * e2, type * type);
int double_naturalorder(const double * e1, const double * e2, type * type);

#define to_double(a) *(double *) a

extern type double_type;

// int_pair

typedef struct{
	int a;
	int b;
}int_pair;

int_pair int_pair_parse_s(char * text);
int_pair * int_pair_parse(int_pair * out, char * text, type * type);
char * int_pair_tostring(const int_pair * p, char * mem, type * type);
bool int_pair_equals(const int_pair * t1, const int_pair * t2, type * type);
int int_pair_naturalorder(const int_pair * t1,const int_pair * t2, type * type);

#define to_int_pair(a) *(int_pair *) a

extern type int_pair_type;

// long_pair

typedef struct{
	long a;
	long b;
}long_pair;

long_pair long_pair_parse_s(char * text);
long_pair * long_pair_parse(long_pair * out, char * text, type * type);
char * long_pair_tostring(const long_pair * p, char * mem, type * type);
bool long_pair_equals(const long_pair * t1, const long_pair * t2, type * type);
int long_pair_naturalorder(const long_pair * t1,const long_pair * t2, type * type);

#define to_long_pair(a) *(long_pair *) a

extern type long_pair_type;

// punto

typedef struct {
	double x;
	double y;
} punto;


typedef enum {PRIMERO,SEGUNDO,TERCERO,CUARTO} Cuadrante;


double punto_distancia_al_origen(const punto * p);
int punto_ord_distancia_al_origen(const punto * p1, const punto * p2);


Cuadrante punto_cuadrante(const punto * p);

punto punto_parse_s(char * text);
punto * punto_parse(punto * out, char * text, type * type);
char * punto_tostring(const punto * p, char * mem, type * type);
bool punto_equals(const punto * p1, const punto * p2, type * type);
int punto_naturalorder(const punto * t1,const  punto * t2, type * type);

#define to_punto(a) *(punto *) a

extern type punto_type;

// pair type

#define to_pair_s(a) *(pair_s *) a

typedef struct {
	int counter;
	void * value;
}pair_enumerate;

pair_enumerate pair_enumerate_of(int n, void * key);

pair_enumerate * pair_enumerate_parse(pair_enumerate * out, char * text, type * t);
char * pair_enumerate_tostring(const pair_enumerate * e, char * mem, type * t);
bool pair_enumerate_equals(const pair_enumerate * e1, const pair_enumerate * e2, type * t);
int pair_enumerate_naturalorder(const pair_enumerate * e1, const pair_enumerate * e2, type * t);

#define to_pair_enumerate(a) *(pair_enumerate *) a

extern type pair_enumerate_type;

typedef struct {
	void * key;
	void * value;
} pair;

pair pair_of(void * key, void * value);

extern memory_heap pair_memory_heap;
extern char pair_delimiters[20];
pair * pair_parse(pair * out, char * text, type * t);
pair * pair_mem_parse(pair * out, char * text, type * t);
char * pair_tostring(const pair * e, char * mem, type * t);
bool pair_equals(const pair * e1, const pair * e2, type * t);
int pair_naturalorder(const pair * e1, const pair * e2, type * t);

void * pair_to_key(void * key, pair * in);
void * pair_to_value(void * value, pair * in);


pair * pair_of_value(pair * p, void * value);

void pair_free(pair * p);


#define to_pair(a) *(pair *) a

extern type pair_type;

// string_var type

typedef struct {
	int tam;
	int size;
	char * data;
} string_var;

string_var * string_var_parse(string_var * out, char * text, type * type);
char * string_var_tostring(const string_var * e, char * mem, type * type);
bool string_var_equals(const string_var * e1, const string_var * e2, type * type);
int string_var_naturalorder(const string_var * e1,const  string_var * e2, type * type);


extern type string_var_type;

string_var string_var_empty();
bool string_var_is_empty(string_var * in);
bool string_var_not_is_empty(string_var * in);
string_var string_var_of_fix(const char * initial);
int string_var_size(string_var * in);
char * string_var_data(string_var * in);

void * string_var_add_string(string_var * out, const char * in_char);
void * string_var_add_string_var(string_var * out, const string_var * in);

void string_var_clear(string_var * in);

void string_var_free(string_var * in);

// string_fix type

#define Tam_String 256

typedef char string_fix[Tam_String];

char * remove_eol(char * string);
int split_text(const char * in, const char * delimiters, char ** tokens);
char char_get(const char * in, int i);
char char_set(char * in_out, int i, char c);
int string_fix_size(const char * string);
char * string_fix_substring(char * out, char * in, int from, int to);
//char *  pchar_remove_eol(char * out, char * in);

char* strtok_r2(char *str, const char *delim, char **nextp);

char * string_fix_concat(char * out, const char * in);
char * string_fix_copy(char * out, const char * in);
char *  string_fix_remove_eol(char * out, char * in);

char * string_fix_parse(char * out, char * text, type * type);
char * string_fix_tostring(const char * e, char * mem, type * type);
bool string_fix_equals(const char * e1, const char * e2, type * type);
int string_fix_naturalorder(const char * e1,const char * e2, type * type);

bool string_fix_is_empty(char * in);
bool string_fix_not_is_empty(char * in);
bool string_fix_all_space(char * in);
bool string_fix_not_all_space(char * in);

//char *  array_char_remove_eol(char * out, char * in);

type string_fix_type_of_tam(int numchars);
extern type string_fix_type;

// optional s_type

typedef struct {
	void * value;
} optional;

optional * optional_parse(optional * out, char * text, type * t);
char * optional_tostring(const optional * e, char * mem, type * t);
bool optional_equals(const optional * e1, const optional * e2, type * t);
int optional_naturalorder(const optional * e1, const optional * e2, type * t);


optional optional_of(void * in);
bool optional_is_present(optional * in);
void * optional_value(optional * in);

#define to_optional(a) *(optional *) a

extern type optional_type;

extern type null_type;


// utilities

void * copy(void * out, void * in, int size);
void * copy_and_mem(void * in, int size);
void * copy_in_pointer(void ** out, void * in, int size);
void * swap(void * out, void * in, int size);

void test_string();
void test_types();
void test_types_1();

//definitions



#endif /* TYPES_H_ */