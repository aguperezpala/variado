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
}


/* Vamos a definir el operador de comparacion '<' para poder
* ordenarlos, segun el startMs. */
bool GTQObject::operator<(GTQObject & other)
{
	return (this->startMs < other.getStartMs());
}

