#include "timemeters.h"

/* Constructor:
* REQUIRES:
* 	posicion inicial
* 	w = width
*/
GTQTimeMeter::GTQTimeMeter(void)
{
}


/* para setear el comienzo del medidor */
void GTQTimeMeter::setStart(unsigned long long start)
{
	this->start = start;
	if (this->end < start)
		this->end = this->start;
}

/* para setear el fin del medidor.
* REQUIRES:
* 	end >= start
*/
void GTQTimeMeter::setEnd(unsigned long long end)
{
	if (end < this->start) {
		ASSERT(false);
		debugp("Estan metiendo un valor incorrecto\n");
		return;
	}
	this->end = end;
}


/* Vamos a dibujar el medidor como si fuese un rectangulo
* relleno que comienza/finaliza en |--====--|. En el interior
* vamos a setear cuanto dura en ms o la medida predeterminada
* y en los extremos vamos a poner el comienzo y el final
*/
void GTQTimeMeter::paint(QPainter *painter, const QRect &dest, unsigned long long msRef)
{
	QRect rect;
	QPen pen(painter->pen());
	int width = 0;
	int startX = 0, startY = 0;
	
	
	/* configuramos el pen */
	pen.setWidth(this->penWidth);
	pen.setColor(this->color);
	pen.setStyle(Qt::DotLine);
	painter->setPen(pen);
	
	/* calculamos el tamaño de la zona donde vamos a dibujar */
	width = (this->end - this->start) / this->scale;
	
	/* configuramos y posicionamos el rectangulo */
	rect.setHeight(dest.height() / 3);
	rect.setWidth(width * 80 / 100); /* va a ocupar el 80 % el rectangulo */
	startX = (int) (((long long) this->start - (long long) msRef)/
			((long long)this->scale));
	
	rect.moveCenter(QPoint(startX + width/2 ,dest.top() + dest.height()/2));
	
	/* lo pintamos */
	painter->fillRect(rect, this->color);
	
	/* ahora dibujamos las lineas punteadas */
	startY = rect.center().y();
	/* la horizontal */
	painter->drawLine(startX, startY, startX + width, startY);
	/* las verticales */
	painter->drawLine(startX, startY, startX, dest.bottom());
	painter->drawLine(startX + width, startY, startX + width, dest.bottom());
	
	/* ahora pintamos los numeros usando un nuevo rectangulo */
	rect.setWidth(width);
	rect.setHeight(dest.height());
	rect.moveCenter(QPoint(startX + width/2, dest.top() + dest.height() / 2));
	
	pen.setColor(Qt::black);
	painter->setPen(pen);
	/* dibujamos la duracion del segmento */
	painter->drawText(rect, Qt::AlignHCenter | Qt::AlignBottom,
			   tt_ms_to_all(this->end - this->start));
	/* el comienzo */
	painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop,
			   tt_ms_to_all(this->start));
	/* el final */
	painter->drawText(rect, Qt::AlignRight | Qt::AlignTop,
			   tt_ms_to_all(this->end));
	
}

/* Verificamos si debemos o no pintar el puntero.*/
bool GTQTimeMeter::haveToPaint(const QRect &rect,  unsigned long long initMs)
{
	unsigned long long endMs = rect.width() * this->scale;
	
	/* vemos si o el comienzo o el final del medidor esta adentro del
	 * rango visible (initMs, endMs) */
	if (((this->start >= initMs) && (this->start <= endMs)) ||
		((this->end >= initMs) && (this->end <= endMs)))
		return true;
	
	return false;
}

/* Destructor */
GTQTimeMeter::~GTQTimeMeter()
{
	/*! nothing to do for the moment */
}
