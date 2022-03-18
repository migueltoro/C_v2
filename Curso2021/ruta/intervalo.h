/*
 * intervalo.h
 *
 *  Created on: 18 mar 2022
 *      Author: migueltoro
 */

#ifndef INTERVALO_H_
#define INTERVALO_H_

#include "../coordenadas/coordenadas.h"
#include "../ruta/marca.h"
#include "../types/types.h"
#include "../types/dates.h"
#include "../types/list.h"
#include "../types/iterables.h"
#include "../types/accumulators.h"

typedef struct {
	marca * p;
	marca * f;
}intervalo;

intervalo intervalo_of(marca * p, marca * f);
double intervalo_tiempo(intervalo in);
double intervalo_longitud(intervalo in);
double intervalo_velocidad(intervalo in);
double intervalo_desnivel(intervalo in);

#endif /* INTERVALO_H_ */
