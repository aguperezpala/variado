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

