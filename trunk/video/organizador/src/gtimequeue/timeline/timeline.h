#ifndef GTQTIMELINE_H
#define GTQTIMELINE_H

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


class GTQTimeLine : public GTQObject
{
	public:
		/* Constructor, no vamos a pedir nada  */
		GTQTimeLine(void);
		
		
		/*! Vamos a dibujar la linea de tiempo teniendo en cuenta el
		*   el punto de referencia (pos), la escala (que nos va a
		*   determinar el zoom => segundos, minutos, horas) y ademas
		*   el rect donde debemos dibujar la linea.
		*   TODO: chequear el tema del estilo.
		* REQUIRES: 
		* 	painter != NULL
		*/
		void paint(QPainter * painter);
		
		/*! Siempre vamos a devolver true, siempre queremos mostrar
		*   la linea de tiempo...
		* RETURNS:
		* 	true	si tiene que ser pintado
		*/
		bool haveToPaint(QRect &rect);
		
		
		/* Destructor */
		~GTQTimeLine();
	
	
		
	private:
		/* Atributos */
		
		/* Rectangulo donde vamos a mostrar la linea de tiempo. */
		QRect rect;

};

#endif