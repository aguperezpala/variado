#ifndef GTQTRIGGER_H
#define GTQTRIGGER_H

/* librerias generales */
#include <QImage>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QRect>

/* librerias propias */
#include "gtqobject.h"
#include "consts.h"
#include "debug.h"

/*! vamos a enumerar los diferentes tipos de trigger que puede haber */
typedef enum {
	GTQT_JUMP,	/* hay un salto a otro video o algo */
	GTQT_STOP	/* hay un stop, no hay ningun salto */
} gtqt_type_t;
	

class GTQTrigger: public GTQObject
{
	public:
		/* Constructor: 
		 * Cuando lo creamos vamos a determinar que tipo de trigger es.
		 */
		GTQTrigger(gtqt_type_t kind);
		
		/* Esta funcion va a modificar el tamaño del objeto segun la
		* escala que se pase por parametro.
		* NOTE: la escala va a representar cuantos ms son representados
		* 	 por 1 pixel.
		*/
		void setScale(unsigned long long scale){this->scale = scale;};
		
		/* Dibujamos el trigger segun que tipo de trigger sea.
		 * NOTE: Por el momento va a ser una simple linea, despues
		 * podemos reemplazarlo por una imagen o algo mas lindo
		 */
		void paint(QPainter *, const QRect &dest, unsigned long long msRef);
		
		/* Verificamos si debemos o no pintar el trigger.*/
		bool haveToPaint(const QRect &rect,  unsigned long long initMs);
		
		/* setea el comienzo del trigger */
		void setStart(unsigned long long ms){this->start = ms;};
		
		/* setea el final del trigger */
		void setEnd(unsigned long long ms){this->end = ms;};
		
		/* Destructor */
		~GTQTrigger();
	
	
		
	private:
		/* Atributos */
		
		/* comienzo */
		unsigned long long start;
		/* final */
		unsigned long long end;
		/* tipo del trigger */
		gtqt_type_t kind;

};

#endif