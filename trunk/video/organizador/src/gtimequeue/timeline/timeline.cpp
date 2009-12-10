#include "timeline.h"

/* Constructor, no vamos a pedir nada  */
GTQTimeLine::GTQTimeLine(void)
{
	/* TODO: setear el estilo y esas cosas que podemos llegar a usar */
}


/*! Vamos a dibujar la linea de tiempo teniendo en cuenta el
*   el punto de referencia (pos), la escala (que nos va a
*   determinar el zoom => segundos, minutos, horas) y ademas
*   el rect donde debemos dibujar la linea.
*   TODO: chequear el tema del estilo.
* REQUIRES: 
* 	painter != NULL
*/
void GTQTimeLine::paint(QPainter * painter)
{
	/* TODO: Tenemos que reimplementar eso modularizandola probablemente
	 * en una funcion auxiliar para tener en cuenta distintos estilos.
	 */
	
	/*! por el momento dibujamos una linea recta. */
	painter->drawLine(this->rect.left(), this->rect.height()/4,
			   this->rect.right(), this->rect.height()/4);
}

/*! Siempre vamos a devolver true, siempre queremos mostrar
*   la linea de tiempo...
* RETURNS:
* 	true	si tiene que ser pintado
*/
bool GTQTimeLine::haveToPaint(QRect &rect)
{
	/* vamos a guardar este rect para usarlo luego, y devolvemos siempre
	 * true */
	this->rect = rect;
	
	return true;
}


/* Destructor */
GTQTimeLine::~GTQTimeLine()
{
	/*! nothing to do for the moment */
}
