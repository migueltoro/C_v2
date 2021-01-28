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

typedef struct stp {
	union {
		struct {
			bool (*equals_g)(const void * e1, const void * e2, struct stp * t);
			char * (*tostring_g)(const void * e, char * mem, struct stp * t);
			int (*order_g)(const void * e1, const void * e2, struct stp * t);
			void * (*parse_g)(void * out, char * text, struct stp * t);
		} gt;
		struct{
			bool (*equals)(const void * e1, const void * e2);
			char * (*tostring)(const void * e, char * mem);
			int (*order)(const void * e1, const void * e2);
			void * (*parse)(void * out, char * text);
		} tp;
	};
	int size;
	int num_types;
	struct stp * types[2];
} type;


unsigned long int hash(const char * key);
unsigned long int hash_code(void * in, type * t);

//type * type_copy(type * t);
bool type_equals(const type* e1, const type* e2);
type * generic_type_1(type * base, type * key_type);
type * generic_type_2(type *base, type * key_type, type * value_type);
int num_types(type * t);
type * types(type * base, int i);
void type_free(type * t);




bool equals(const void * e1, const void * e2, type * type);
char * tostring(const void * e, char * mem, type * type);
int order(const void * e1, const void * e2, type * type);
void * parse(void * out, char * text, type * type);

extern type * tmp_type;

bool equals_type(const void * e1, const void * e2);
char * tostring_type(const void * e, char * mem);
int order_type(const void * e1, const void * e2);
void * parse_type(void * out, char * text);

// int type

int int_parse_s(char * text);
int * int_parse(int * out, char * text);
char * int_tostring(const int * e, char * mem);
bool int_equals(const int * e1, const int * e2);
int int_naturalorder(const int * e1, const int * e2);

#define to_int(a) *(int *) a

extern type int_type;

char* char_parse(char * out, char * text);
char* char_tostring(const char * e, char * mem);
bool char_equals(const char * e1, const char * e2);
int char_naturalorder(const char * e1, const char * e2);

#define to_char(a) *(char *) a

extern type char_type;

#ifndef MSG_BOOL
#define MSG_BOOL(x) (x==0?"false":"true")
#endif

bool* bool_parse(bool * out, char * in);
char* bool_tostring(const bool * e, char * out);
bool bool_equals(const bool * e1, const bool * e2);
int bool_naturalorder(const bool * e1,const bool * e2);

#define to_bool(a) *(bool *) a

extern type bool_type;

// long type

long long_parse_s(char * text);
long * long_parse(long * out, char * text);
char * long_tostring(const long * e, char * mem);
bool long_equals(const long * e1, const long * e2);
int long_naturalorder(const long * e1, const long * e2);

#define to_long(a) *(long *) a

extern type long_type;

// long long type

long long long_long_parse_s(char * text);
long long * long_long_parse(long long * out, char * text);
char * long_long_tostring(const long long * e, char * mem);
bool long_long_equals(const long long * e1, const long long * e2);
int long_long_naturalorder(const long long * e1, const long long * e2);

extern type long_long_type;

// float type

float float_parse_s(char * text);
float * float_parse(float * out, char * text);
char * float_tostring(const float * e, char * mem);
bool float_equals(const float * e1, const float * e2);
int float_naturalorder(const float * e1,const float * e2);

#define to_float(a) *(float *) a

extern type float_type;

// double type

double double_parse_s(char * text);
double * double_parse(double * out, char * text);
char * double_tostring(const double * e, char * mem);
bool double_equals(const double * e1, const double * e2);
int double_naturalorder(const double * e1, const double * e2);

#define to_double(a) *(double *) a

extern type double_type;

// int_pair

typedef struct{
	int a;
	int b;
}int_pair;

int_pair int_pair_parse_s(char * text);
int_pair * int_pair_parse(int_pair * out, char * text);
char * int_pair_tostring(const int_pair * p, char * mem);
bool int_pair_equals(const int_pair * t1, const int_pair * t2);
int int_pair_naturalorder(const int_pair * t1,const int_pair * t2);

#define to_int_pair(a) *(int_pair *) a

extern type int_pair_type;

// long_pair

typedef struct{
	long a;
	long b;
}long_pair;

long_pair long_pair_parse_s(char * text);
long_pair * long_pair_parse(long_pair * out, char * text);
char * long_pair_tostring(const long_pair * p, char * mem);
bool long_pair_equals(const long_pair * t1, const long_pair * t2);
int long_pair_naturalorder(const long_pair * t1,const long_pair * t2);

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
punto * punto_parse(punto * out, char * text);
char * punto_tostring(const punto * p, char * mem);
bool punto_equals(const punto * p1, const punto * p2);
int punto_naturalorder(const punto * t1,const  punto * t2);

#define to_punto(a) *(punto *) a

extern type punto_type;

// pair type

#define to_pair_s(a) *(pair_s *) a

typedef struct {
	int n;
	void * key;
}pair_enumerate;

pair_enumerate * pair_enumerate_parse(pair_enumerate * out, char * text, type * t);
char * pair_enumerate_tostring(const pair_enumerate * e, char * mem, type * t);
bool pair_enumerate_equals(const pair_enumerate * e1, const pair_enumerate * e2, type * t);
int pair_enumerate_naturalorder(const pair_enumerate * e1, const pair_enumerate * e2, type * t);


pair_enumerate pair_enumerate_of(void * key, int n);
pair_enumerate pair_enumerate_empty(type *t);

#define to_pair_enumerate(a) *(pair_enumerate *) a

extern type pair_enumerate_type;

typedef struct {
	void * key;
	void * value;
} pair;

extern memory_heap pair_memory_heap;
extern char pair_delimiters[20];
pair * pair_parse(pair * out, char * text, type * t);
pair * pair_mem_parse(pair * out, char * text, type * t);
char * pair_tostring(const pair * e, char * mem, type * t);
bool pair_equals(const pair * e1, const pair * e2, type * t);
int pair_naturalorder(const pair * e1, const pair * e2, type * t);

void * pair_to_key(void * key, pair * in);
void * pair_to_value(void * value, pair * in);

pair pair_of(void * key, void * value);
pair * pair_of_value(pair * p, void * value);

pair pair_empty(type * key_type, type * value_type);
void pair_free(pair * p);


#define to_pair(a) *(pair *) a

extern type pair_type;
extern type tmp_pair_type;
extern type pair_mem_type;

// string type

//typedef char string[Tam_String];

typedef struct {
	int tam;
	int size;
	char * data;
} string;

string * string_parse(string * out, char * text);
char * string_tostring(const string * e, char * mem);
bool string_equals(const string * e1, const string * e2);
int string_naturalorder(const string * e1,const  string * e2);


extern type string_type;

string string_empty();
bool string_is_empty(string * in);
bool string_not_is_empty(string * in);
string string_of_pchar(const char * initial);
int string_size(string * in);
char * string_tochar(string * in);

void * string_add_pchar(string * out, const char * in_char);
void * string_add_string(string * out, const string * in);

void string_clear(string * in);

void string_free(string * in);

// pchar type

#define Tam_String 256

typedef char pchar[Tam_String];

char * remove_eol_s(char * string);
int pchar_split_text(const char * in, const char * delimiters, char ** tokens);
char pchar_get(const char * in, int i);
char pchar_set(char * in_out, int i, char c);
int pchar_size(const char * string);
char * pchar_substring(char * out, char * in, int from, int to);
//char *  pchar_remove_eol(char * out, char * in);

char* strtok_r2(char *str, const char *delim, char **nextp);

char * pchar_concat(char * out, const char * in);
char * pchar_copy(char * out, const char * in);
char *  pchar_remove_eol(char * out, char * in);

char * pchar_parse(char * out, char * text);
char * pchar_tostring(const char * e, char * mem);
bool pchar_equals(const char * e1, const char * e2);
int pchar_naturalorder(const char * e1,const char * e2);

bool pchar_is_empty(char * in);
bool pchar_not_is_empty(char * in);
bool pchar_all_space(char * in);
bool pchar_not_all_space(char * in);

//char *  array_char_remove_eol(char * out, char * in);


type * array_char_type_num(int num_chars);
extern type array_char_type;
extern type pchar_type;

// optional type

typedef struct {
	void * value;
//	type * type;
} optional;

optional * optional_parse(optional * out, char * text, type * t);
char * optional_tostring(const optional * e, char * mem, type * t);
bool optional_equals(const optional * e1, const optional * e2, type * t);
int optional_naturalorder(const optional * e1, const optional * e2, type * t);


optional optional_of(void * in);
bool optional_is_present(optional * in);
void * optinal_value(optional * in);

#define to_optional(a) *(optional *) a

extern type optional_type;

//

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
