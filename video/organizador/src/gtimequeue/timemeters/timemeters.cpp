#include "timepointer.h"

/* Constructor:
* REQUIRES:
* 	posicion inicial
* 	w = width
*/
GTQTimePointer::GTQTimePointer(unsigned long long p, int w)
{
	this->pos = p;
	this->width = w;
}

/* Esta funcion va a modificar el tamaño del objeto segun la
* escala que se pase por parametro.
* NOTE: la escala va a representar cuantos ms son representados
* 	 por 1 pixel.
*/
void GTQTimePointer::setScale(unsigned long long scale)
{
	this->scale = scale;
}

/* Dibujamos el puntero en la posicion correspondiente.
* NOTE: Por el momento va a ser una simple linea, despues
* podemos reemplazarlo por una imagen o algo mas lindo
*/
void GTQTimePointer::paint(QPainter *painter, const QRect &dest, unsigned long long msRef)
{
	int pixelPos = 0;
	QRect aux(dest);
	
	/* seteamos el color */
	painter->setPen(this->color);
	
	/* obtenemos en que posicion debemos dibujar el puntero */
	pixelPos = (int) ((this->pos - msRef) / this->scale);
	
	/* ahora lo dibujamos */
	aux.setWidth(this->width);
	aux.moveLeft(pixelPos - (this->width / 2));
	aux.moveTop(dest.top());
	painter->drawArc(aux, 30*16, 120*16);
	painter->drawText(dest, Qt::AlignCenter, QString("Time pointer"));
	
}

/* Verificamos si debemos o no pintar el puntero.*/
bool GTQTimePointer::haveToPaint(const QRect &rect,  unsigned long long initMs)
{
	/* lo tenemos que dibujar si y solo si la posicion del puntero esta 
	 * dentro de los ms que vamos a mostrar */
	if ((this->pos >= initMs) && (this->pos <= (initMs + (this->scale * 
		rect.width()))))
		return true;
	
	/* caso contrario.. */
	return false;
}

/* Destructor */
GTQTimePointer::~GTQTimePointer()
{
	/*! nothing to do for the moment */
}
