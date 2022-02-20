/*
 * ejemplos2.c
 *
 *  Created on: 14 oct. 2019
 *      Author: migueltoro
 */


#include "../ejemplos_files/ejemplos_files.h"

//suma los primos del fichero

long sum_primos_file(char * file){
	iterator it = iterable_file_string_fix(file);
	long sum = 0;
	while(iterable_has_next(&it)){
		char * line = (char *) iterable_next(&it);
		long e;
		parse(&e,line,&long_type);
		if(es_primo(e)){
			sum = sum +e;
		}
	}
	iterable_free(&it);
	return sum;
}

//cuenta los primos del fichero

int count_primos_file(char * file){
	iterator it = iterable_file_string_fix(file);
	int sum = 0;
	while(iterable_has_next(&it)){
		char * line = (char *) iterable_next(&it);
		long e;
		parse(&e,line,&long_type);
		if(es_primo(e)){
			sum = sum +1;
		}
	}
	return sum;
}

//cuenta los primos del fichero con otra estructura

int count_primos_file_2(char * file){
	iterator it = iterable_file_string_fix(file);
	int sum = 0;
	while(iterable_has_next(&it)){
		char * line = (char *) iterable_next(&it);
		iterator it2 = iterable_split_text_tam(line," ,",string_fix_tam);
		while(iterable_has_next(&it2)) {
			char * num = (char *) iterable_next(&it2);
			long e;
			parse(&e,num,&long_type);
			if(es_primo(e)){
				sum = sum +1;
			}
		}
	}
	return sum;
}

//obtiene una lista del fichero

list file_to_list(char * file){
       iterator it = iterable_file_string_fix(file);
       list ls = list_empty(&long_type);
       long e;
       while(iterable_has_next(&it)){
             char * line = (char *) iterable_next(&it);
             iterator it2 = iterable_split_text_tam(line," ,",string_fix_tam);
             while(iterable_has_next(&it2)) {
                    char * num = (char *) iterable_next(&it2);
                    parse(&e,num,&long_type);
                    if (e%2 == 0) {
                        e = e*e;
                        list_add(&ls, &e);
                    }
             }
       }
       return ls;
}

//obtiene una lista del fichero

list file_to_list_2(char * file) {
	iterator f = iterable_file_string_fix(file);
	list ls = list_empty(&long_type);
	long e;
	while (iterable_has_next(&f)) {
		char * linea = iterable_next(&f);
		char * tt[20];
		int n = split_text(linea, " ,", tt);
		for (int i = 0; i < n; i++) {
			parse(&e,tt[i],&long_type);
			if (e%2 == 0) {
				e = e * e;
				list_add(&ls, &e);
			}
		}
	}
	return ls;
}

//obtiene una lista de listas del fichero

list file_to_list_of_list(char * file){
	list res = list_empty(&list_type);
	iterator it1 = iterable_file_string_fix(file);
	int x;
	while(iterable_has_next(&it1)) {
	      char* linea = (char*)iterable_next(&it1);
	      list ls = list_empty(&long_type);
	      iterator it2 = iterable_split_text_tam(linea, " ,",string_fix_tam);
	      while(iterable_has_next(&it2)) {
	    	  	char * tx = iterable_next(&it2);
	            parse(&x,tx,&long_type);
                list_add(&ls, &x);
	      }
	      list_add(&res, &ls);
	}
	return res;
}

//comprueba si la es un palidrome

bool is_palindrome(list * ls){
	int i = 0;
	int n = list_size(ls);
	bool r = true;
	while(n-2*i-1>0 && r){
		r = *(int *)list_get(ls,i) == *(int *)list_get(ls,n-i-1);
		i = i+1;
	}
	return r;
}

void test_ejemplos_files(){
	char mem[2500];
	int r = sum_primos_file("ficheros/numeros.txt");
	printf("r = %d\n",r);
	int np = count_primos_file("ficheros/numeros.txt");
	printf("np = %d\n",np);
	np = count_primos_file_2("ficheros/numeros_2.txt");
	printf("np2 = %d\n",np);
	int dt[] = {1,2,0,4,5,4,3,2,1};
	list ls = list_of(dt,9,&int_type);
	bool s = is_palindrome(&ls);
	printf("s = %s\n",s?"true":"false");
	list ls2 = file_to_list("ficheros/numeros_2.txt");
	char * s2 = list_tostring(&ls2,mem);
	printf("%s\n", s2);
	ls2 = file_to_list_2("ficheros/numeros_2.txt");
	s2 = list_tostring(&ls2,mem);
	list ls3 = file_to_list_of_list("ficheros/datos_entrada.txt");
	s2 = list_tostring(&ls3, mem);
	printf("%s ",s2);
}
