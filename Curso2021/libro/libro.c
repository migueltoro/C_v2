/*
 * palabras.c
 *
 *  Created on: 22 jul. 2019
 *      Author: migueltoro
 */

#include "libro.h"

int numero_de_lineas(char * file) {
	iterator r = iterable_file_string_fix_tam(file,100);
	int n = iterable_size(&r);
	iterable_free(&r);
	return n;
}

int numero_de_lineas_vacias(char * file) {
	iterator r = iterable_file_string_fix_tam(file,100);
	iterator r2 = iterable_filter(&r,string_fix_all_space);
	int n = iterable_size(&r2);
	iterable_free(&r);
	return n;
}

int numero_de_palabras(char * file) {
	iterator r = iterable_words_in_file(file,100,20," ,;.()");
	int n = iterable_size(&r);
	iterable_free(&r);
	return n;
}

set palabras_huecas(char * file){
	iterator r1 = iterable_file_string_fix_tam(file,30);
	set s = iterable_to_set(&r1);
	return s;
}

set sg;
bool no_hueca(char * in){
	return set_contains(&sg,in);
}

int numero_de_palabras_no_huecas(char * file) {
	sg = palabras_huecas("ficheros/palabras_huecas.txt");
	iterator r = iterable_words_in_file(file,100,20," ,;.()");
	iterator r2 = iterable_filter(iterable_copy_new(&r),no_hueca);
	return iterable_size(&r);
}

string_fix word_find;
bool es_palabra(enumerate * in){
	return strcmp(in->value,word_find) == 0;
}

bool is_prefix(enumerate * in) {
    return strncmp(word_find, in->value, strlen(word_find)) == 0;
}

int primera_linea_con_palabra(char * file, char * word){
	strcpy(word_find,word);
	iterator r = iterable_words_and_line_in_file(file,1,100,20," ,;.()");
	enumerate * p = (enumerate *) iterable_first(&r,es_palabra);
	if(p != NULL ) return p->counter;
	else return -1;
}

int ultima_linea_con_palabra(char * file, char * word){
	strcpy(word_find,word);
	iterator r = iterable_words_and_line_in_file(file,1,100,20," ,;.()");
	enumerate * p = (enumerate *) iterable_last(&r,es_palabra);
	if(p != NULL ) return p->counter;
	else return -1;
}

int lin_num;
bool es_numero(enumerate * in){
	return lin_num == in->counter;
}

char * linea_numero(char * file, int n){
	lin_num = n;
	iterator r = iterable_file_string_fix_tam(file,100);
	iterator r2 = iterable_enumerate(&r,1);
	enumerate * p = (enumerate *) iterable_first(&r2,es_numero);
	if(p != NULL ) return p->value;
	else return NULL;
}

int * to_len(int * out, int * in){
	*out = strlen(in);
	return out;
}

double longitud_media(char * file){
	iterator r = iterable_file_string_fix_tam(file,100);
	iterator r2 = iterable_map(&r,&int_type,to_len);
	double r3 = iterable_average(&r2);
	return r3;
}

int cmp_len(char * e1, char * e2){
	 int n1 = strlen(e1);
	 int n2 = strlen(e2);
	 return int_type.order(&n1,&n2,&int_type);
}

char * linea_mas_corta(char * file){
	iterator r = iterable_file_string_fix_tam(file,100);
	iterator r2 = iterable_filter(&r,string_fix_not_all_space);
	char * ln = (char *) iterable_min(&r2, cmp_len);
	return ln;
}

char * palabras_f_key(char * out, enumerate * in){
	out = in->value;
	return out;
}

char * palabras_f_value(int * out, enumerate * in){
	*out = in->counter;
	return out;
}

set_multimap lineas_de_palabras(char * file){
	iterator r = iterable_words_and_line_in_file(file,1,100,20," ,;.()");
	set_multimap sm = iterable_grouping_set_map(&r,r.type->types[0],&int_type,
			palabras_f_key, palabras_f_value);
	return sm;
}

void test_palabras_1(){
	int n  = numero_de_lineas_vacias("ficheros/quijote.txt");
	printf("%d\n",n);
}

void test_palabras_2(){
	int n  = numero_de_palabras("ficheros/quijote.txt");
	printf("%d\n",n);
}

void test_palabras_3(){
	iterator r = iterable_words_and_line_in_file("ficheros/short_quijote.txt",1,100,20," ,;.()");
	iterable_to_console(&r);
}

void test_palabras_4(){
	printf("%s\n",linea_mas_corta("ficheros/quijote.txt"));
	printf("%d\n",primera_linea_con_palabra("ficheros/quijote.txt","Gaula"));
	printf("%d\n",ultima_linea_con_palabra("ficheros/quijote.txt","Gaula"));
	printf("%s\n",linea_numero("ficheros/quijote.txt",3203));
}

void test_palabras_5(){
	printf("%.2f\n",longitud_media("ficheros/quijote.txt"));
}

void test_palabras_6(){
	set_multimap sm = lineas_de_palabras("ficheros/quijote.txt");
	iterator r = set_multimap_iterable(&sm);
	iterable_to_console_sep(&r,"\n","","");
}




