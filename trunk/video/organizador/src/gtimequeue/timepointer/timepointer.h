#ifndef GTQTIMEPOINTER_H
#define GTQTIMEPOINTER_H

/* librerias generales */
#include <QImage>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QRect>

/* librerias propias */
#include "gtqobject.h"
#include "timetransform.h"
#include "consts.h"
#include "debug.h"


class GTQTimePointer: public GTQObject
{
	public:
		/* Constructor:
		 * REQUIRES:
		 * 	posicion inicial
		 * 	width del puntero
		 */
		GTQTimePointer(unsigned long long p, int width);
		
		/* Esta funcion va a modificar el tamaño del objeto segun la
		* escala que se pase por parametro.
		* NOTE: la escala va a representar cuantos ms son representados
		* 	 por 1 pixel.
		*/
		void setScale(unsigned long long scale);
		
		/* Dibujamos el puntero en la posicion correspondiente.
		 * NOTE: Por el momento va a ser una simple linea, despues
		 * podemos reemplazarlo por una imagen o algo mas lindo
		 */
		void paint(QPainter *, const QRect &dest, unsigned long long msRef);
		
		/* Verificamos si debemos o no pintar el puntero.*/
		bool haveToPaint(const QRect &rect,  unsigned long long initMs);
		
		/* funcion que setea la posicion del puntero */
		void setPos(unsigned long long ms){this->pos = ms;};
		
		/* Funcion que setea el grosor del puntero, en pixeles */
		void setWidth(int w){this->width = w;};
		
		/* Destructor */
		~GTQTimePointer();
	
	
		
	private:
		/* Atributos */
		
		/* este determina en que posicion esta el puntero */
		unsigned long long pos;
		/* definimos el tamaño que debe tener el puntero */
		int width; /* recordemos que el heigth esta dado por el rect */

};

#endif