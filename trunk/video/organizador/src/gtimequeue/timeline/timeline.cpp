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
	int prevSmallMarks = 0;
	int numSmallMarks = 0;
	int startSMPixel = 0;
	int i = 0, j = 0, centerY = 0;
	int top = dest.top() + 2;
	int smallStepSize = 0;
	QRect textRect(dest);
	unsigned long long actualMs = 0;
	
	/* vamos a calcular cuantas marcas grandes tenemos que hacer y cuantas
	 * marcas pequeñas tenemos que hacer. */
	numSmallMarks = dest.width() * this->scale / this->deltaSmallMark;
	
	/* vemos cuantas marcas anteriores segun el punto de referencia se 
	 * hicieron */
	prevSmallMarks = msRef / this->deltaSmallMark;
	
	/* vemos si tenemos que dibujar o no una larga, las largas solo van a
	 * ser dibujadas si j == 0 */
	j = (prevSmallMarks + 1) % this->deltaLongMark;
	
	/* calculamos en que pixel debemos comenzar a dibujar */
	/*! hay varias operaciones hechas y resumidas aca */
	startSMPixel = (int) (this->deltaSmallMark * (prevSmallMarks + 1) - 
			msRef) / this->scale;
	
	/* calculamos cada cuantos pixeles tenemos que dibujar las marcas */
	smallStepSize = this->deltaSmallMark / this->scale;
	
	/* seteamos las configuraciones */
	pen.setWidth(this->penWidth);
	pen.setColor(this->color);
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	
	/* dibujamos */
	painter->drawLine(dest.left(), top,
			   dest.right(), top);
	
	centerY = top + dest.height() / 2;
	/* dibujamos las marcas */
	longMarkSize += top;
	smallMarkSize += top;
	for (i = 0; i < numSmallMarks; i++) {
		if (j == 0) {
			/* dibujamos larga */
			painter->drawLine(startSMPixel , top,
					   startSMPixel, longMarkSize);
			/*! vamos a setear el tiempo tambien */
			actualMs = msRef + startSMPixel * this->scale;
			textRect.moveCenter(QPoint(startSMPixel, centerY));
			painter->drawText(textRect, Qt::AlignHCenter | 
					Qt::AlignBottom, tt_ms_to_hms(actualMs));
			
		} else {
			/* dibujamos corta */
		painter->drawLine(startSMPixel , top,
				   startSMPixel, smallMarkSize);
		}
		j = (j + 1) % this->deltaLongMark;
		startSMPixel += smallStepSize;
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
