/*
 * intervalo.c
 *
 *  Created on: 18 mar 2022
 *      Author: migueltoro
 */

#include "../ruta/intervalo.h"

intervalo intervalo_of(marca * p, marca * f){
	intervalo r = {p,f};
	return r;
}

double intervalo_tiempo(intervalo in){
	time_t initial_time = in.p->time;
	time_t last_time = in.f->time;
	return time_diff_hours(initial_time,last_time);
}


double intervalo_longitud(intervalo in) {
	coordenadas_3d c0 = in.p->coordenadas;
	coordenadas_3d c1 = in.f->coordenadas;
	double d = coordenadas_3d_distance(c0,c1);
	return d;
}

double intervalo_velocidad(intervalo in){
	return intervalo_longitud(in)/intervalo_tiempo(in);
}

double intervalo_desnivel(intervalo in) {
	coordenadas_3d c0 = in.p->coordenadas;
	coordenadas_3d c1 = in.f->coordenadas;
	double d = c1.altitud-c0.altitud;
	return d;
}
