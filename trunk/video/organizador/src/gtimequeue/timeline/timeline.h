#ifndef GTQTIMELINE_H
#define GTQTIMELINE_H

/* librerias generales */
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QColor>

/* librerias propias */
#include "gtqobject.h"
#include "consts.h"
#include "debug.h"


class GTQTimeLine : public GTQObject
{
	public:
		/* Constructor, no vamos a pedir nada  */
		GTQTimeLine(void);
		
		/* Esta funcion va a modificar el tamaño del objeto segun la
		* escala que se pase por parametro.
		* NOTE: la escala va a representar cuantos ms son representados
		* 	 por 1 pixel.
		*/
		void setScale(unsigned long long scale);
		
		/*! Vamos a dibujar la linea de tiempo teniendo en cuenta el
		*   el punto de referencia (pos), la escala (que nos va a
		*   determinar el zoom => segundos, minutos, horas) y ademas
		*   el rect donde debemos dibujar la linea.
		*   TODO: chequear el tema del estilo.
		* REQUIRES: 
		* 	painter != NULL
		*/
		void paint(QPainter *, const QRect &dest, unsigned long long msRef);
		
		/*! Siempre vamos a devolver true, siempre queremos mostrar
		*   la linea de tiempo...
		* RETURNS:
		* 	true	si tiene que ser pintado
		*/
		bool haveToPaint(const QRect &rect,  unsigned long long initMs);
		
		/* Destructor */
		~GTQTimeLine();
	
	
		
	private:
		/* Atributos */
		
};

#endif