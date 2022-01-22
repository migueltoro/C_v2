/*
 * coordenadas_2d.c
 *
 *  Created on: 12 jul. 2019
 *      Author: migueltoro
 */

#include "../data/coordenadas.h"

coordenadas_2d coordenadas_2d_of(double latitud, double longitud){
	coordenadas_2d c = {latitud, longitud};
	return c;
}

coordenadas_2d coordenadas_2d_to_radians(coordenadas_2d c){
	double lat = c.latitud * M_PI / 180.0;
	double lon = c.longitud * M_PI / 180.0;
	coordenadas_2d r = {lat, lon};
	return r;
}

double coordenadas_2d_distance(coordenadas_2d c1, coordenadas_2d c2){
	double radio_tierra = 6373.0;
	coordenadas_2d c1R = coordenadas_2d_to_radians(c1);
	coordenadas_2d c2R = coordenadas_2d_to_radians(c2);
	double incLat  = c2R.latitud-c1R.latitud;
	double incLong = c2R.longitud-c1R.longitud;
	double a = pow(sin(incLat/2),2)+cos(c1R.latitud)*cos(c2R.latitud)*pow(sin(incLong/2),2);
	double c = 2 * atan2(sqrt(a),sqrt(1 - a));
	return radio_tierra*c;
}

bool coordenadas_2d_cercanas(coordenadas_2d c1, coordenadas_2d c2, double d){
	return coordenadas_2d_distance(c1,c2) <=d;
}

char * coordenadas_2d_tostring(coordenadas_2d * c, const char * mem) {
	sprintf(mem,"(%lf,%lf)",c->latitud,c->longitud);
	return mem;
}

double * double_sum(double * out, double * in){
	*out = *out + *in;
	return out;
}

double * lat(double * out, const coordenadas_2d * c){
	*out = c->latitud;
	return out;
}

double * lng(double * out, const coordenadas_2d * c){
	*out = c->longitud;
	return out;
}

coordenadas_2d  coordenadas_2d_center(list lc){
	iterator it = list_iterable(&lc);
	iterator ilat = iterable_map(&it,&double_type,lat);
	double average_lat = iterable_average(&ilat);
	it = list_iterable(&lc);
	iterator ilng = iterable_map(&it,&double_type,lng);
	double average_lng = iterable_average(&ilng);
	coordenadas_2d c = coordenadas_2d_of(average_lat,average_lng);
//	iterable_free(&it);
//	iterable_free(&ilat);
//	iterable_free(&ilng);
	return c;
}

coordenadas_3d coordenadas_3d_of(double latitude, double longitude, double altitud){
	coordenadas_3d c = {latitude, longitude, altitud};
	return c;
}

double coordenadas_3d_distance(coordenadas_3d c1, coordenadas_3d c2){
	coordenadas_2d c12D = coordenadas_2d_of(c1.latitud, c1.longitud);
	coordenadas_2d c22D = coordenadas_2d_of(c2.latitud, c2.longitud);
	return sqrt(pow(coordenadas_2d_distance(c12D,c22D),2)+pow(c1.altitud-c1.altitud,2));
}
char * coordenadas_3d_tostring(coordenadas_3d * c, const char * mem){
	sprintf(mem,"(%lf,%lf,%lf)",c->latitud,c->longitud,c->altitud);
	return mem;
}

