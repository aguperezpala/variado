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


/* vamos a definir en porcentajes con respecto al rectangulo donde vamos a
 * dibujar la linea de tiempo, cuanto va a valer la linea corta y la larga
 * (las longMark y SmallMark)
 */
#define GTQTL_LONGMARK_SIZE		40
#define GTQTL_SMALLMARK_SIZE		15


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
		
		
		/* ### 		funciones propias	### */
		
		/* Funcion que setea el delta en ms para generar una marca
		 * chiquita 
		 */
		void setDeltaSmallMark(unsigned long long ms)
		{this->deltaSmallMark = ms;};
		
		/* Funcion que setea el delta en ms para generar una marca
		* grande. 
		*/
		void setDeltaLongMark(unsigned long long ms)
		{this->deltaLongMark = ms;};
		
		/* Destructor */
		~GTQTimeLine();
	
	
		
	private:
		/* Atributos */
		
		/* para determinar cada cuantos ms hacemos una rayita (chica) */
		unsigned long long deltaSmallMark;
		/* determinamos cada cuantos ms vamos a hacer una raya larga */
		unsigned long long deltaLongMark;
};

#endif