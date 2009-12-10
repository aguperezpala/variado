#ifndef GTQOBJECT_H
#define GTQOBJECT_H

/* librerias generales */
#include <QImage>
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QRect>

/* librerias propias */
#include "consts.h"
#include "debug.h"


class GTQObject
{
	public:
		/* Constructor, no vamos a pedir nada  */
		GTQObject(void): backImg(NULL),scale(1), label(""), layer(0) {};
		
		
		/* ### ### ### Manipulacion de la escala ### ### ### */
		float getScale(void) {return this->scale;};
		
		/* Setea la escala, si esta fuera dentro de los rangos 
		 * especificados no hace nada
		 */
		void setScale(float scale);
		
		/* ### ### ### Manipulacion de la posicion ### ### ### */
		QPoint & getPos(void) {return this->pos;};
		
		/* setea la posicion */
		void setPos(QPoint & nPos) {this->pos = nPos;};
		
		/* ### ### ### Manipulacion del label ### ### ### */
		QString & getLabel(void) {return this->label;};
		
		/* setea el label */
		void setLabel(QString & nLabel) {this->label = nLabel;};
		
		
		/* ### ### ### Manipulacion del layer ### ### ### */
		int getLayer(void) {return this->layer;};
		
		/* setea el layer*/
		void setLayer(int nLayer) {this->layer = nLayer;};
		
		
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
		 * REQUIRES: 
		 * 	painter != NULL
		 */
		virtual void paint(QPainter *){};
		
		/*! Esta funcion determina si el objeto esta dentro del rectangulo
		 * a ser impreso, osea, si tiene que ser pintado o no. Cada
		 * herencia de este objeto debe redefinir esta funcion
		 * RETURNS:
		 * 	true	si tiene que ser pintado
		 * 	false	caso contrario
		 */
		virtual bool haveToPaint(QRect &){return true;};
		 
		
		/* Vamos a definir el operador de comparacion '<' para poder
		 * ordenarlos, segun el layer. */
		bool operator<(GTQObject & other);
		
		
		/* Destructor */
		~GTQObject();
	
	
		
	private:
		/* Atributos */
		
		/* Imagen de fondo, puede ser Null o no. */
		QImage * backImg;
		/* posicion donde tiene que ser dibujado en el qpainter */
		QPoint pos;
		/* escala a la que tienen que ser representados las cosas */
		float scale;
		/* el nombre a mostrar o ninguno... */
		QString label;
		/* este va a determinar el layer, osea la prioridad de quien va
		 * a ser dibujado antes  y quien despues. mientras mas layer
		 * mas prioridad, osea, los de mayor layer van a ser dibujados
		 * por encima de los de menor layer.
		 */
		int layer;
		

};

#endif