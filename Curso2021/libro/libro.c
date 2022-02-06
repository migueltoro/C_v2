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
	iterator r2 = iterable_filter(iterable_copy(&r),no_hueca);
	return iterable_size(&r);
}

string_fix word_g;
bool es_palabra(enumerate * in){
	return strcmp(in->value,word_g) == 0;
}

int primera_linea_con_palabra(char * file, char * word){
	strcpy(word_g,word);
	iterator r = iterable_words_and_line_in_file(file,100,20," ,;.()");
	enumerate * p = (enumerate *) iterable_first(&r,es_palabra);
	if(p != NULL ) return p->counter;
	else return -1;
}

bool is_prefix(enumerate * in) {
    return strncmp(word_g, in->value, strlen(word_g)) == 0;
}

int num_linea(char * file, char * word){
	strcpy(word_g,word);
	iterator r = iterable_file_string_fix_tam(file,100);
	enumerate * p = (enumerate *) iterable_first(&r,is_prefix);
	if(p != NULL ) return p->counter;
	else return -1;
}

int ultima_linea_con_palabra(char * file, char * word){
	strcpy(word_g,word);
	iterator r = iterable_words_and_line_in_file(file,100,20," ,;.()");
	enumerate * p = (enumerate *) iterable_last(&r,es_palabra);
	if(p != NULL ) return p->counter;
	else return -1;
}

int n_g;
bool es_numero(enumerate * in){
	return n_g == in->counter;
}

char * line_n(char * file, int n){
	n_g = n;
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

int nn_g;
bool con_n(enumerate * in) {
	int ln = strlen(in->value);
    bool r = ln> 0 &&  ln <= nn_g;
//    if(r) printf("%d == %d == %s\n",nn_g,ln,in->value);
    return r;
}
int nn_p;
bool primeras_lineas(enumerate * in) {
    bool r = in->counter < nn_g;
//    if(r) printf("%d == %d == %s\n",nn_g,ln,in->value);
    return r;
}

void lineas_con_n_caracteres(char * file, int n){
	char mem[1000];
	nn_g = n;
	nn_p = n;
	iterator r = iterable_file_string_fix_tam(file,100);
	iterator r2 = iterable_enumerate(&r,1);
//	iterator r3 = iterable_filter(&r2,con_n);
	iterator r4 = iterable_filter(&r2,primeras_lineas);
	iterable_to_console_sep(&r4,"\n","","");
}

void test_palabras_3(){
	printf("%s\n",linea_mas_corta("ficheros/quijote.txt"));
//	printf("%d\n",num_linea("ficheros/quijote.txt","de:"));
	lineas_con_n_caracteres("ficheros/quijote.txt",350);
}


