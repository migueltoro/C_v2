/*
 * new_math.c
 *
 *  Created on: 13 ago. 2018
 *      Author: migueltoro
 */


#include "../types/math2.h"
#include "../types/preconditions.h"
;
/**
 * @param a Un entero
 * @return Si es par
 */
bool es_par(int a) {
	return a % 2 == 0;
}

bool es_par_f(int * a) {
	return es_par(*a);
}

long menor_que_long_ref;

bool menor_que_long(long * a){
	return *a < menor_que_long_ref;
}

bool multiplo_de_long(long * in){
	return (*in)%menor_que_long_ref == 0;
}

/**
 * @param a Un entero
 * @return Si es impar
 */
bool es_impar(int a) {
	return !es_par(a);
}

bool es_impar_f(int * a) {
	return es_impar(*a);
}

/**
 * @param a Un entero
 * @param b Un segundo entero
 * @return Si a es divisible por b
 */

bool es_divisible_int(int a, int b) {
	return (a % b) == 0;
}

int es_divisible_int_base;
bool es_divisible_int_f(int * in) {
	return *in % es_divisible_int_base;
}

bool es_divisible_long(long a, long b) {
	return (a % b) == 0;
}

long es_divisible_long_base;
bool es_divisible_long_f(int * in) {
	return *in % es_divisible_long_base;
}
/**
 * @param a Un entero
 * @return Si a es primo
 */


bool es_primo(long n) {  //problema 18
	check_argument(n>=2,__FILE__,__LINE__,"El parámetro debe ser mayor o igual que 2 y es %ld",n);
	long sq = (long) sqrt((double) n);
	int e = 2;
	bool a = false;
	while (e <= sq && !a) {
		a = (n%e==0);
		e = e + 1;
	}
	return !a;
}

bool es_primo_f(long * n) {
	return es_primo(* n);
}

/**
 * @param a Un entero
 * @return Siguiente primo
 */

long siguiente_primo(long a) {
	long r = 2;
	if(a>=2) {
		a = a%2==0?a+1:a+2;
		for (r = a; true; r = r + 2) {
		if (es_primo(r))
			break;
		}
	}
	return r;
}

long * siguiente_primo_f(long * out, long * in){
	*out = siguiente_primo(*in);
	return out;
}

void new_rand(){
	time_t t;
	srand((unsigned) time(&t));
}

long inc_long_ref = 1;
long inc_int_ref = 1;

long * inc_long_f(long * out, long * in){
	*out = *(in)+inc_long_ref;
	return out;
}

int * inc_int_f(int * out, int * in){
	*out = *(in)+inc_int_ref;
	return out;
}

bool long_sum(long * out, long * in){
	*out = *out + *in;
	return false;
}

bool long_max(long * out, long * in){
	*out = MAX(*out,*in);
	return false;
}

long es_multiplo_base;

bool es_multiplo(long * in){
	return (*in)%es_multiplo_base == 0;
}

long remainder_base;

long * remainder_f(long * out, long * in){
	*out = (*in)%remainder_base;
	return out;
}

bool true_p(void * in){
	return true;
}

bool false_p(void * in){
	return false;
}
/**
 * @pre b &gt; a
 * @param a Límite inferior
 * @param b Límite Superior
 * @return Un entero aleatorio r tal que a &le; = r &lt; b
 */


int entero_aleatorio(int a, int b) {
	long valor;
	if (b - a == 1) {
		valor = a;
	} else {
		valor = a + rand() % (b - a);
	}
	return valor;
}

long entero_aleatorio_long_a = 0;
long entero_aleatorio_long_b = 20;

long * entero_aleatorio_long_f(long * out, void * in){
	*out = entero_aleatorio(entero_aleatorio_long_a,entero_aleatorio_long_b);
	return out;
}

/**
 * @pre b &gt; a
 * @param a Límite inferior
 * @param b Límte Superior
 * @return Un double aleatorio que  está en el intervalo  a &le; = r &lt; b
 */
double double_aleatorio(double a, double b) {
	double r = ((double) rand() / (RAND_MAX));
	r = a + (b - a) * r;
	return r;
}


double double_aleatorio_a = 0;
double double_aleatorio_b = 20;

double * double_aleatorio_f(double * out, double * in){
	*out = double_aleatorio(double_aleatorio_a,double_aleatorio_b);
	return out;
}

/**
 * @param a Un entero
 * @param b Un segundo entero
 * @return Calcula el máximo común divisor de los valores ablsolutos de los parámetros
 */
long mcd(long a, long b) {
	int u = abs(a);
	int v = abs(b);
	int r;
	while (v != 0) {
		r = u % v;
		u = v;
		v = r;
	}
	return u;
}

long * mcd_f(long * out, long_pair * in){
	*out = mcd(in->a,in->b);
	return out;
}

long square_long(long a){
	return a*a;
}

long * square_long_f(long * out, long * in){
	*out = (*in)*(*in);
	return out;
}

int * square_int_f(int * out, int * in){
	* out = (*in)*(*in);
	return out;
}

int * square_double_f(double * out, double * in){
	*out = (*in)*(*in);
	return out;
}

void test_math2(){
	long a = siguiente_primo(2);
	printf("%ld\n",a);
	bool r = es_primo(2);
	printf("%s",MSG_BOOL(r));
}
