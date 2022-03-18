/*
 * ruta.c
 *
 *  Created on: 14 jul. 2019
 *      Author: migueltoro
 */

#include "../ruta/ruta.h"


ruta * ruta_of_file(ruta * r, char * file){
	iterator i1 = iterable_file_string_fix(file);
	iterator i2 = iterable_map(&i1,&marca_type,marca_parse);
	list ls = iterable_to_list(&i2);
	r->marcas = ls;
//	iterable_free(&i1);
//	iterable_free(&i2);
	return r;
}

coordenadas_3d * marca_to_c3(coordenadas_3d * out, marca * in){
	*out = in->coordenadas;
	return out;
}

double * _f_dis(double * out, pair * in){
	coordenadas_3d c1 = ((marca *)in->key)->coordenadas;
	coordenadas_3d c2 = ((marca *)in->value)->coordenadas;
	*out = coordenadas_3d_distance(c1,c2);
	return out;
}

intervalo ruta_intervalo(ruta * r, int i){
	return intervalo_of(list_get(&r->marcas,i),list_get(&r->marcas,i+1));
}

double ruta_longitud(ruta * r){
	double s = 0;
	for(int i =0; i<r->marcas.size-1;i++){
		s = s + intervalo_longitud(ruta_intervalo(r,i));
	}
	return s;
}

void ruta_printf(ruta * r){
	char mem[Tam_String];
	printf("\n");
	for(int i = 0; i < r->marcas.size; i++){
		printf("%s\n",list_get_string(&r->marcas,i,mem));
	}
}

double ruta_tiempo(ruta *r) {
	double s = 0;
	for (int i = 0; i < r->marcas.size - 1; i++) {
		s = s + intervalo_tiempo(ruta_intervalo(r, i));
	}
	return s;
}

double ruta_velocidad_media(ruta * r) {
	return ruta_longitud(r)/ruta_tiempo(r);
}

coordenadas_2d * marca_to_c2(coordenadas_2d * out, marca * in){
	out->latitud = in->coordenadas.latitud;
	out->longitud = in->coordenadas.longitud;
	return out;
}

void test_ruta(){
	char mem[1000];
	ruta r;
	ruta_of_file(&r,"ficheros/ruta.csv");
//	ruta_printf(&r);
	printf("\n________________\n");
	type nt = null_type;
	nt.size = sizeof(coordenadas_2d);
	list lc = list_map(&r.marcas,&nt,marca_to_c2);
	coordenadas_2d c2 = coordenadas_2d_center(lc);
	printf("\nCentro de coordenadas en = %s\n",coordenadas_2d_tostring(&c2,mem));
	double rs1 = ruta_longitud(&r);
	printf("\nLongitud = %lf kilometros \n",rs1);
	double rs2 = ruta_tiempo(&r);
	printf("\nTiempo = %lf horas \n",rs2);
	double rs3 = intervalo_tiempo(ruta_intervalo(&r,14));
	printf("\nTiempo %lf km/h en intervalo 3\n",rs3);
//	double rs4 = ruta_velocidad_media(&r);
	double rs4 = rs1/rs2;
	printf("\nVelocidad media = %lf km/h \n",rs4);
}
