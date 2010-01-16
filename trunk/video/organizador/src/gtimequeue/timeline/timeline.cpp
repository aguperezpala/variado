#include "timeline.h"

/* Constructor, no vamos a pedir nada  */
GTQTimeLine::GTQTimeLine(void):deltaSmallMark(0),deltaLongMark(0)
{
	/* TODO: setear el estilo y esas cosas que podemos llegar a usar */
	this->setPenWidth(1);
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
	QPen pen(painter->pen());
	int longMarkSize = dest.height() * GTQTL_LONGMARK_SIZE / 100;
	int smallMarkSize = dest.height() * GTQTL_SMALLMARK_SIZE / 100;
	int numLongMarks = 0;
	int numSmallMarks = 0;
	int startLMPixel = 0, startSMPixel = 0;
	int i = 0;
	int top = dest.top() + 2;
	int smallStepSize = 0, longStepSize = 0;
	
	/* vamos a calcular cuantas marcas grandes tenemos que hacer y cuantas
	 * marcas pequeñas tenemos que hacer. */
	numLongMarks = dest.width() * this->scale / this->deltaLongMark;
	numSmallMarks = dest.width() * this->scale / this->deltaSmallMark;
	
	/* calculamos en que pixel debemos comenzar a dibujar */
	startLMPixel = (int) (msRef % this->deltaLongMark) / this->scale;
	startSMPixel = (int) (msRef % this->deltaSmallMark) / this->scale;
	
	/* calculamos cada cuantos pixeles tenemos que dibujar las marcas */
	smallStepSize = this->deltaSmallMark / this->scale;
	longStepSize = this->deltaLongMark / this->scale;
	
	/* seteamos las configuraciones */
	pen.setWidth(this->penWidth);
	pen.setColor(this->color);
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	
	/* dibujamos */
	painter->drawLine(dest.left(), top,
			   dest.right(), top);
	
	/* dibujamos las marcas */
	longMarkSize += top;
	
	for (i = 0; i < numLongMarks; i++) {
		startLMPixel += longStepSize;
		painter->drawLine(startLMPixel , top,
				   startLMPixel, longMarkSize);
	}
	smallMarkSize += top;
	for (i = 0; i < numSmallMarks; i++) {
		startSMPixel += smallStepSize;
		painter->drawLine(startSMPixel , top,
				   startSMPixel, smallMarkSize);
	}
	
	/*painter->drawText(dest, Qt::AlignCenter, QString("Time line"));*/
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
