/*
 * new_math.h
 *
 *  Created on: 13 ago. 2018
 *      Author: migueltoro
 */

#ifndef MATH2_H_
#define MATH2_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "../types/types.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

/**
 * @param a Un entero
 * @return Si es par
 */
bool es_par(int a);
bool es_par_f(int *a);

extern long menor_que_long_ref;

bool menor_que_long(long * a);
bool multiplo_de_long(long * a);

/**
 * @param a Un entero
 * @return Si es impar
 */
bool es_impar(int a);
bool es_impar_f(int *a);
/**
 * @param a Un entero
 * @param b Un segundo entero
 * @return Si a es divisible por b
 */
bool es_divisible_int(int a, int b);
bool es_divisible_long(long a, long b);

extern int es_divisible_int_base;
bool es_divisible_int_f(int * in);

extern long es_divisible_long_base;
bool es_divisible_long_f(int * in);

/**
 * @param a Un entero
 * @return Si a es primo
 */
bool es_primo(long a);
bool es_primo_f(long * a);
/**
 * @param a Un entero
 * @return Siguiente primo
 */
long siguiente_primo(long a);
long * siguiente_primo_f(long * out, long * in);

extern long inc_long_ref;
extern long inc_int_ref;

long * inc_long_f(long * out, long * in);
int * inc_int_f(int * out, int * in);

bool long_sum(long * out, long * in);
bool long_max(long * out, long * in);

extern long es_multiplo_base;
bool es_multiplo(long * in);

extern long remainder_base;

long * remainder_f(long * out, long * in);

bool true_p(void * in);
bool false_p(void * in);

/**
 * Inicializa el generador de numeros aleatorios
 */
void new_rand();

/**
 * @pre b &gt; a
 * @param a L�mite inferior
 * @param b L�mite Superior
 * @return Un entero aleatorio r tal que a &le; = r &lt; b
 */
int entero_aleatorio(int a, int b);
int * entero_aleatorio_f(int * out, pair_int * in);

extern long entero_aleatorio_long_a;
extern long entero_aleatorio_long_b;

long * entero_aleatorio_long_f(long * out, void * in);
/**
 * @pre b &gt; a
 * @param a L�mite inferior
 * @param b L�mte Superior
 * @return Un double aleatorio que  est� en el intervalo  a &le; = r &lt; b
 */
double double_aleatorio(double a, double b);

extern double double_aleatorio_a;
extern double double_aleatorio_b;

double * double_aleatorio_f(double * out, double * in);
/**
 * @param a Un entero
 * @param b Un segundo entero
 * @return Calcula el m�ximo com�n divisor de los valores ablsolutos de los par�metros
 */
long mcd(long a, long b);
long * mcd_f(long * out, pair_long * in);

long square_long(long a);
long * square_long_f(long * out, long * in);
int * square_int_f(int * out, int * in);
int * square_double_f(double * out, double * in);

void test_math2();

#endif /* MATH2_H_ */
