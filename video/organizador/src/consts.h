#ifndef CONSTS_H
#define CONSTS_H

/*! Funcion que obtenemos directamente el char * */
#define qstrtochar(x)	x.toStdString().c_str()


/*! ###		GTimeQueue		### */
/* vamos a definir la maxima y minima escala posible, la escala se va a comportar
 * de la siguiente manera, mientras mayor sea la escala => mayor sera el "zoom"
 */
/* TODO: modificar la cantidad de ms que van a poder ser representados por pixel
*/
#define GTQ_MAX_SCALE		86400000
#define GTQ_MIN_SCALE		1
/* escala sin zoom, la escala normal la determinamos de la siguiente forma:
 * Queremos que en una pantalla "promedio" (600 pix ancho) se vean 24 hs 
 * (1 pixel por minuto) ==> (24*60*60*1000)/600 */
#define GTQ_NORMAL_SCALE	144000
/* tamaño maximo de MILISEGUNDOS que vamos a soportar para la linea de tiempo 
 * que van a ser 24 hs = 24*60*60*1000 */
#define GTQ_MAX_MS_SUPPORTED	86400000

#endif
