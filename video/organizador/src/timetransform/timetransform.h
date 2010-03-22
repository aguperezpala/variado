/*! modulo que va a facilitar la transformacion de tiempos */
#ifndef TIMETRANSFORM_H
#define TIMETRANSFORM_H

#include <QString>

#include "consts.h"
#include "debug.h"

/*! Algunos define utiles */
#define TT_HOUR		3600000		/* 1000 * 60 * 60 */
#define TT_MIN		60000		/* 60 * 1000 */
#define TT_SEC		1000

#define TT_SEPARATOR	":"		/* caracter separador hh:mm:ss:ms */

/* Funcion que va a tomar una cantidad de ms y va a devolver un QString
 * de la siguiente forma horas:minutos:segundos:milisegundos
 */
QString tt_ms_to_all(unsigned long long ms);

/* Funcion que va a devolver horas:minutos:segundos desde una cantidad de ms */
QString tt_ms_to_hms(unsigned long long ms);

/* Funcion que devuelve la cantidad de horas segun la cantidad de ms */
int tt_ms_get_hs(unsigned long long ms);

/* Funcion que devuelve la cantidad de minutos segun la cantidad de ms */
int tt_ms_get_mins(unsigned long long ms);

/* Funcion que devuelve la cantidad de segundos segun la cantidad de ms */
int tt_ms_get_secs(unsigned long long ms);




#endif
