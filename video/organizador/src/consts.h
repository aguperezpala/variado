#ifndef CONSTS_H
#define CONSTS_H

/*! Funcion que obtenemos directamente el char * */
#define qstrtochar(x)	x.toStdString().c_str()


/*! ###		GTimeQueue		### */
/* vamos a definir la maxima y minima escala posible, la escala se va a comportar
 * de la siguiente manera, mientras mayor sea la escala => mayor sera el "zoom"
 */
#define GTQ_MAX_SCALE		100
#define GTQ_MIN_SCALE		1
/* escala sin zoom */
#define GTQ_NORMAL_SCALE	(GTQ_MAX_SCALE + GTQ_MIN_SCALE) / 2

#endif
