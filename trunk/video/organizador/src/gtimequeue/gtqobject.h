/*! Esta clase representa la abstraccion de todos los objetos que pueden ser
 *  dibujados en la linea de tiempo, vamos a tener una duracion, un punto
 *  de comienzo (startMs) y una escala.
 *  NOTE: Cuando cambiamos la escala de los objetos no vamos a modificar el
 *  	  alto de los mismos.
 */
#ifndef GTQOBJECT_H
#define GTQOBJECT_H

/* librerias generales */
#include <QImage>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QRect>
#include <QColor>

/* librerias propias */
#include "consts.h"
#include "debug.h"


class GTQObject
{
	public:
		/* Constructor, no vamos a pedir nada  */
		GTQObject(void){};
		
		/* Esta funcion va a modificar el tamaño del objeto segun la
		 * escala que se pase por parametro.
		 * NOTE: la escala va a representar cuantos ms son representados
		 * 	 por 1 pixel.
		 */
		virtual void setScale(unsigned long long scale);
		
		
		/* ### ### ### Manipulacion del comienzo del obj ### ### ### */
		void setStartMs(unsigned long long s) {this->startMs = s;};
		unsigned long long getStartMs(void) {return this->startMs;};
		
		/* ### ### ### Manipulacion de la duracion ### ### ### */
		void setDurationMs(unsigned long long d) {this->durationMs = d;};
		unsigned long long getDurationMs(void) {return this->durationMs;};
			
		/* ### ### ### Manipulacion del color ### ### ### */
		QColor & getColor(void) {return this->color;};
		
		/* setea el color */
		void setColor(QColor & nColor) {this->color = nColor;};
		
		
		/*! Esta es la funcion principal, esta va a ser llamada para 
		 * graficar el objeto sobre la pantalla, esta funcion debe
		 * ser redefinida en las herencias, para que cada una grafique
		 * de forma correcta el objeto.
		 * NOTE: en ref.y() tenemos la altura a ser dibujado el objeto
		 * 	 mientras que en ref.x() tenemos en donde comienza la
		 * 	 linea de tiempo (lo mas a la izquierda del rectangulo*
		 * 	 donde vamos a dibujar).
		 * REQUIRES: 
		 * 	painter != NULL
		 */
		virtual void paint(QPainter *, QPoint &ref){};
		
		/*! Esta funcion determina si el objeto esta dentro del rectangulo
		 * a ser impreso, osea, si tiene que ser pintado o no. Cada
		 * herencia de este objeto debe redefinir esta funcion.
		 * Vamos a pasar el initMs que hace referencia al primer ms en 
		 * la parte mas a la izquierda del rectangulo.
		 * RETURNS:
		 * 	true	si tiene que ser pintado
		 * 	false	caso contrario
		 * NOTE: En general debemos hacer this->rect = nRect.
		 */
		virtual bool haveToPaint(QRect &, unsigned long long initMs)
		{return true;};
		 
		/*! Funcion que va a determinar si un punto esta dentro del
		 *  del objeto (en este caso es para determinar si el mouse
		 *  esta sobre el objeto o no).
		 *  NOTE: el punto va a ser las coordenadas dentro de la linea
		 *  	  de tiempo, osea, x y de la ventana.
		 *  	  Requiere ademas que el objeto este siendo mostrado por
		 *  	  pantalla, osea que vamos a llamar a esta funcio si y
		 *  	  solo si haveToPaint == true.
		 *  REQUIRES:
		 *  	p.isNull() == false
		 *  	havToPaint() == true
		 *  RETURNS:
		 *  	true	si el punto esta sobre el objeto
		 *  	false	caso contrario
		 */
		virtual bool havePoint(QPoint & p) {return true;};
		
		/* Vamos a definir el operador de comparacion '<' para poder
		 * ordenarlos, segun el startMs. */
		bool operator<(GTQObject & other);
		
		
		/* Destructor */
		~GTQObject();
	
	
		
	protected:
		/* Atributos */
		
		/* milisegundo en el que empiza el objeto */
		unsigned long long startMs;
		/* milisegundos que dura el objeto en la linea de tiempo */
		unsigned long long durationMs;
		/* guardamos la escala por las dudas */
		unsigned long long scale;
		/* Color para setear al pen del painter. */
		QColor color;

};

#endif