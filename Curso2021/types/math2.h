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

extern long _ref_long;

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
bool es_divisible_int_f(int_pair * in);
bool es_divisible_long_f(long_pair * in);



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

long * inc_1_long(long * out, long * in);
int * inc_1_int(int * out, int * in);
/**
 * Inicializa el generador de numeros aleatorios
 */
void new_rand();

/**
 * @pre b &gt; a
 * @param a Límite inferior
 * @param b Límite Superior
 * @return Un entero aleatorio r tal que a &le; = r &lt; b
 */
int entero_aleatorio(int a, int b);
int * entero_aleatorio_f(int * out, int_pair * in);
/**
 * @pre b &gt; a
 * @param a Límite inferior
 * @param b Límte Superior
 * @return Un double aleatorio que  está en el intervalo  a &le; = r &lt; b
 */
double double_aleatorio(double a, double b);
double * double_aleatorio_f(double * out, punto * in);
/**
 * @param a Un entero
 * @param b Un segundo entero
 * @return Calcula el máximo común divisor de los valores ablsolutos de los parámetros
 */
long mcd(long a, long b);
long * mcd_f(long * out, long_pair * in);

long square_long(long a);
long * square_long_f(long * out, long * in);
int * square_int_f(int * out, int * in);
int * square_double_f(double * out, double * in);

void test_math2();

#endif /* MATH2_H_ */
