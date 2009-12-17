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
		GTQObject(void): backImg(NULL),label(""), layer(0) {};
		
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
		
		/* ### ### ### Manipulacion del label ### ### ### */
		QString & getLabel(void) {return this->label;};
		
		/* setea el label */
		void setLabel(QString & nLabel) {this->label = nLabel;};
		
		
		/* ### ### ### Manipulacion del layer ### ### ### */
		int getLayer(void) {return this->layer;};
		
		/* setea el layer*/
		void setLayer(int nLayer) {this->layer = nLayer;};
		
		/* ### ### ### Manipulacion del color ### ### ### */
		QColor & getColor(void) {return this->color;};
		
		/* setea el layer*/
		void setColor(QColor & nColor) {this->color = nColor;};
		
		
		/* Funcion que va a setear la imagen de fondo, si ya existe
		 * alguna, sera eliminada (liberada la memoria) y se seteara
		 * la nueva imagen.
		 * REQUIRES:
		 * 	si img != NULL && no liberar img una vez seteada
		 * 	si img == NULL => solo borramos la anterior y seteamos
		 * 			  ninguna imagen de fondo.
		 * NOTE: si se libera la img tamos al horno
		 */
		void setBackImg(QImage * img);
		
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
		virtual bool haveToPaint(QRect &, unsigned long long initMs){return true;};
		 
		/*! Funcion que va a determinar si un punto esta dentro del
		 *  del objeto (en este caso es para determinar si el mouse
		 *  esta sobre el objeto o no).
		 *  REQUIRES:
		 *  	p.isNull() != false
		 *  RETURNS:
		 *  	true	si el punto esta sobre el objeto
		 *  	false	caso contrario
		 */
		bool havePoint(QPoint & p);
		
		/* Vamos a definir el operador de comparacion '<' para poder
		 * ordenarlos, segun el layer. */
		bool operator<(GTQObject & other);
		
		
		/* Destructor */
		~GTQObject();
	
	
		
	protected:
		/* Atributos */
		
		/* Imagen de fondo, puede ser Null o no. */
		QImage * backImg;
		/* milisegundo en el que empiza el objeto */
		unsigned long long startMs;
		/* milisegundos que dura el objeto en la linea de tiempo */
		unsigned long long durationMs;
		/* guardamos la escala por las dudas */
		unsigned long long scale;
		/* el nombre a mostrar o ninguno... */
		QString label;
		/* este va a determinar el layer, osea la prioridad de quien va
		 * a ser dibujado antes  y quien despues. mientras mas layer
		 * mas prioridad, osea, los de mayor layer van a ser dibujados
		 * por encima de los de menor layer.
		 */
		int layer;
		/* Color para setear al pen del painter. */
		QColor color;
		/* Rectangulo que ocupa el objeto, por ahora vamos a determinar
		 * que todos los objetos estan dentro de un rectangulo para 
		 * detectar colisiones. */
		QRect objRect;

};

#endif