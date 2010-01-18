#include "trigger.h"

/* Constructor: 
* Cuando lo creamos vamos a determinar que tipo de trigger es.
*/
GTQTrigger::GTQTrigger(gtqt_type_t kind)
{
	this->kind = kind;
	this->penWidth = 1;
}

/* Dibujamos el trigger segun que tipo de trigger sea.
* NOTE: Por el momento va a ser una simple linea, despues
* podemos reemplazarlo por una imagen o algo mas lindo
*/
void GTQTrigger::paint(QPainter *painter, const QRect &dest, unsigned long long msRef)
{
	QPen pen(painter->pen());
	int lineHeight = dest.top() + dest.height()/2;
	int startPixel = 0, endPixel = 0;
	QString label = "PRUEBA";
	QRect auxRect;
	
	/* configuramos el pen */
	pen.setWidth(this->penWidth);
	pen.setColor(this->color);
	pen.setStyle(Qt::DotLine);
	painter->setPen(pen);
	
	/*! va a ser una simple linea que va de un punto al otro (start -> end)
	 * y se va a escribir que por encima el tipo de trigger que es
	 */
	/* calculamos a donde empezamos a dibujar en pixeles */
	startPixel = ((long long) this->start - (long long) msRef) / 
			(long long) this->scale;
	endPixel = ((long long) this->end - (long long) msRef) / 
			(long long) this->scale;
	
	/* dibujamos las lineas verticales */
	painter->drawLine(startPixel, dest.top(), startPixel, lineHeight);
	painter->drawLine(endPixel, dest.top(), endPixel, lineHeight);
	pen.setStyle(Qt::SolidLine);
	pen.setColor(this->color);
	painter->setPen(pen);
	painter->drawLine(startPixel, lineHeight, endPixel, lineHeight);
	
	/* ahora dibujamos el nombre del tipo de trigger */
	switch (this->kind) {
		case GTQT_JUMP:
			label = "Salto";
			break;
		case GTQT_STOP:
			label = "Pausa";
			break;
		default:
			label = "...";
			break;
	}
	/* vamos a utilizar un rectangulo para que quede lindo el texto */
	auxRect.setWidth(endPixel - startPixel);
	auxRect.setHeight(lineHeight);
	auxRect.moveTopLeft(QPoint(startPixel, lineHeight + 2));
	/* escribimos */
	painter->drawText(auxRect, Qt::AlignHCenter | Qt::AlignTop, label);
	
	
}

/* Verificamos si debemos o no pintar el trigger.*/
bool GTQTrigger::haveToPaint(const QRect &rect,  unsigned long long initMs)
{
	unsigned long long endMs = initMs + rect.width() * this->scale;
	
	/* vemos si lo tenemos que dibujar... */
	if (((this->start >= initMs) && (this->start <= endMs)) || 
		((this->end >= initMs) && (this->end <= endMs)) ||
		((this->start <= initMs) && (this->end >= endMs)))
		return true;
	
	/* caso contrario.. */
	return false;
}

/* Destructor */
GTQTrigger::~GTQTrigger()
{
	/*! nothing to do for the moment */
}
