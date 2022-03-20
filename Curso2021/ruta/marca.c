/*
 * marca.c
 *
 *  Created on: 12 jul. 2019
 *      Author: migueltoro
 */

#include "../ruta/marca.h"

marca marca_of(time_t time, coordenadas_3d coordenadas){
	marca m  = {time,coordenadas};
	return m;
}

marca * marca_parse(marca * out, const char * in) {
	iterator it = iterable_split_text(in,",");
	list ls = iterable_to_list(&it);
	char mens[30];
	sprintf(mens,"numero de campos %d",list_size(&ls));
	check_argument(list_size(&ls)==4,__FILE__,__LINE__,mens);
	marca m;
	m.time = hour_parse(list_get(&ls,0));
	coordenadas_3d coordenadas = coordenadas_3d_of(
			double_parse_s(list_get(&ls,1)),
			double_parse_s(list_get(&ls,2)),
			double_parse_s(list_get(&ls,3)) / 1000);
	m.coordenadas = coordenadas;
	*out = m;
	iterable_free(&it);
	list_free(&ls);
	return out;
}

char * marca_tostring(marca * m, char * mem){
	char mr1[Tam_String];
	char mr2[Tam_String];
	sprintf(mem,"(%s,%s)",hours_tostring(&m->time,mr1),coordenadas_3d_tostring(&m->coordenadas,mr2));
	return mem;
}

bool marca_equals(const marca * e1, const marca * e2){
	return time_equals(e1->time,e2->time);
}

int marca_naturalorder(const marca * e1, const marca * e2){
	return time_naturalorder(e1->time,e2->time);
}

type marca_type = {"marca",marca_equals,marca_tostring,marca_naturalorder,marca_parse,free_0,copy_new_0,copy_0,sizeof(marca),0,NULL};

void test_marca() {
	char mem[250];
	char in[] =	"00:02:36, 36.75061631016433,-5.148278838023543,715.4000244140625";
	marca m;
	parse(&m,in,&marca_type);
	printf("9: %s\n", marca_tostring(&m, mem));
}
