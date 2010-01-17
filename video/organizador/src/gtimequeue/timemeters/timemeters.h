#ifndef GTQTIMEMETER_H
#define GTQTIMEMETER_H

/* librerias generales */
#include <QImage>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QRect>
#include <QTime>

/* librerias propias */
#include "gtqobject.h"
#include "timetransform.h"
#include "consts.h"
#include "debug.h"




class GTQTimeMeter: public GTQObject
{
	public:
		/* Constructor: */
		GTQTimeMeter(void);
		
		/* Esta funcion va a modificar el tamaño del objeto segun la
		* escala que se pase por parametro.
		* NOTE: la escala va a representar cuantos ms son representados
		* 	 por 1 pixel.
		*/
		void setScale(unsigned long long scale){this->scale = scale;};
		
		/* Vamos a dibujar el medidor como si fuese un rectangulo
		 * relleno que comienza/finaliza en |--====--|. En el interior
		 * vamos a setear cuanto dura en ms o la medida predeterminada
		 * y en los extremos vamos a poner el comienzo y el final
		 */
		void paint(QPainter *, const QRect &dest, unsigned long long msRef);
		
		/* Verificamos si debemos o no pintar el puntero.*/
		bool haveToPaint(const QRect &rect,  unsigned long long initMs);
		
		/* para setear el comienzo del medidor */
		void setStart(unsigned long long start);
		
		/* para setear el fin del medidor.
		 * REQUIRES:
		 * 	end >= start
		 */
		void setEnd(unsigned long long end);
		
		/* Destructor */
		~GTQTimeMeter();
	
	
		
	private:
		/* Atributos */
		
		/* donde comienza el "medidor" */
		unsigned long long start;
		/* donde termina */
		unsigned long long end;
};

#endif