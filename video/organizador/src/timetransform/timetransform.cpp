#include "timetransform.h"

/* Funcion que va a tomar una cantidad de ms y va a devolver un QString
* de la siguiente forma horas:minutos:segundos:milisegundos
*/
QString tt_ms_to_all(unsigned long long ms)
{
	QString result = "";
	QString aux;
	int hours = 0;
	int seconds = 0;
	int mins = 0;
	
	
	/* calculamos la cantidad de horas y le restamos */
	hours = ms / TT_HOUR;
	ms -= (TT_HOUR * hours);
	
	/* calculamos la cantidad de minutos */
	mins = ms / TT_MIN;
	ms -= (TT_MIN * mins);
	
	/* cantidad de segundos */
	seconds = ms / TT_SEC;
	ms -= (TT_SEC * seconds);
	
	/* nos aseguramos */
	ASSERT(ms <= 1000);
	
	/* ahora creamos el resultado */
	aux.setNum(hours);
	result.append(aux);
	result.append(TT_SEPARATOR);
	aux.setNum(mins);
	result.append(aux);
	result.append(TT_SEPARATOR);
	aux.setNum(seconds);
	result.append(aux);
	result.append(TT_SEPARATOR);
	aux.setNum(ms);
	result.append(aux);
	
	return result;
}

/* Funcion que devuelve la cantidad de horas segun la cantidad de ms */
int tt_ms_get_hs(unsigned long long ms)
{
	return (int)(ms / TT_HOUR);
}

/* Funcion que devuelve la cantidad de minutos segun la cantidad de ms */
int tt_ms_get_mins(unsigned long long ms)
{
	return (int)(ms / TT_MIN);
}

/* Funcion que devuelve la cantidad de segundos segun la cantidad de ms */
int tt_ms_get_secs(unsigned long long ms)
{
	return (int)(ms / TT_SEC);
}




