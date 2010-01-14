#include "timeline.h"

/* Constructor, no vamos a pedir nada  */
GTQTimeLine::GTQTimeLine(void)
{
	/* TODO: setear el estilo y esas cosas que podemos llegar a usar */
}

/* Esta funcion va a modificar el tamaño del objeto segun la
* escala que se pase por parametro.
* NOTE: la escala va a representar cuantos ms son representados
* 	 por 1 pixel.
*/
void GTQTimeLine::setScale(unsigned long long scale)
{
	this->scale = scale;
}

/*! Vamos a dibujar la linea de tiempo teniendo en cuenta el
*   el punto de referencia (pos), la escala (que nos va a
*   determinar el zoom => segundos, minutos, horas) y ademas
*   el rect donde debemos dibujar la linea.
*   TODO: chequear el tema del estilo.
* REQUIRES: 
* 	painter != NULL
*/
void GTQTimeLine::paint(QPainter *painter, const QRect &dest, 
			 unsigned long long msRef)
{
	/* TODO: Tenemos que reimplementar eso modularizandola probablemente
	 * en una funcion auxiliar para tener en cuenta distintos estilos.
	 */
	
	/*! por el momento dibujamos una linea recta. */
	painter->setPen(this->color);
	painter->drawLine(dest.left(), dest.bottom(),
			   dest.right(), dest.bottom());
}

/*! Siempre vamos a devolver true, siempre queremos mostrar
*   la linea de tiempo...
* RETURNS:
* 	true	si tiene que ser pintado
*/
bool GTQTimeLine::haveToPaint(const QRect &rect, unsigned long long initMs)
{
	
	return true;
}


/* Destructor */
GTQTimeLine::~GTQTimeLine()
{
	/*! nothing to do for the moment */
}
