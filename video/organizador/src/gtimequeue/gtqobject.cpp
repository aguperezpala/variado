#include "gtqobject.h"




/* Setea la escala, si esta fuera dentro de los rangos 
* especificados no hace nada
*/
void GTQObject::setScale(unsigned long long scale)
{
	if(scale > GTQ_MAX_SCALE || scale < GTQ_MIN_SCALE) {
		/* no esta dentro de los rangos posibles */
		debugp("Escala incorrecta\n");
		return;
	}
	/* si esta dentro de los rangos... seteamos */
	this->scale = scale;
}


/* Funcion que va a setear la imagen de fondo, si ya existe
* alguna, sera eliminada (liberada la memoria) y se seteara
* la nueva imagen.
* REQUIRES:
* 	si img != NULL && no liberar img una vez seteada
* 	si img == NULL => solo borramos la anterior y seteamos
* 			  ninguna imagen de fondo.
* NOTE: si se libera la img tamos al horno
*/
void GTQObject::setBackImg(QImage * img)
{
	/* en cualquier caso tenemos que eliminar la imagen actual... */
	if (this->backImg == NULL) {
		delete this->backImg; this->backImg = NULL;
	}
	/* ahora simplemente seteamos la imagen... */
	this->backImg = img;
}



/*! Funcion que va a determinar si un punto esta dentro del
*  del objeto (en este caso es para determinar si el mouse
*  esta sobre el objeto o no).
*  REQUIRES:
*  	p.isNull() != false
*  RETURNS:
*  	true	si el punto esta sobre el objeto
*  	false	caso contrario
*/
bool GTQObject::havePoint(QPoint & p)
{
	if (p.isNull()) {
		debugp("Estamos tomando un punto vacio (Null) \n");
		return false;
	}
	return this->objRect.contains(p);
}


/* Destructor */
GTQObject::~GTQObject()
{
	/* liberamos la memoria */
	if (this->backImg == NULL)
		delete this->backImg;
	
}


/* Vamos a definir el operador de comparacion '<' para poder
* ordenarlos, segun el layer. */
bool GTQObject::operator<(GTQObject & other)
{
	return (this->layer < other.layer);
}

