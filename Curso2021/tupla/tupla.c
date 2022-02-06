/*
 * tupla.c
 *
 *  Created on: 30 nov. 2020
 *      Author: migueltoro
 */

#include "tupla.h"

smax smax_of(int li,int ls,int suma){
	smax s = {li,ls,suma};
	return s;
}

int * smax_parse(smax * out, char * text, type * t){
	sscanf(text,"(%d,%d,%d)",&(out->li),&(out->ls),&(out->suma));
	return out;
}

char * smax_tostring(const smax * e, char * mem, type * t){
    sprintf(mem,"(%d,%d,%d)",e->li,e->ls,e->suma);
    return mem;
}

bool smax_equals(const smax * e1, const smax * e2, type * t){
    return e1->li == e2->li && e1->ls == e2->ls && e1->suma == e2->suma;
}

int smax_naturalorder(const smax * e1,const smax * e2, type * t){
    int a = e1->suma;
    int b = e2->suma;
    int r;
    if(a==b) r = 0;
    else if(a<b) r = -1;
    else r = +1;
    return r;
}

type smax_type = {"smax",smax_equals,smax_tostring,smax_naturalorder,smax_parse,free_0,copy_0,sizeof(smax),0,NULL};

void test_tupla(){
	char mem[256];
	smax s = smax_of(2,3,45);
	smax s2;
	parse(&s2,"(3,4,45)",&smax_type);
	int r = order(&s,&s2,&smax_type);
	printf("%d\n",r);
	tostring(&s,mem,&smax_type);
	printf("%s\n",mem);
	tostring(&s2,mem,&smax_type);
	printf("%s\n",mem);
}
