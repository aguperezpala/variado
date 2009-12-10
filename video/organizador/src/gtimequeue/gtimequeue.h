/*! Este modulo es el encargado de mostrar la linea de tiempo graficamente,
 *  vamos a poder insertar diferentes tipos de objetos. Existen 2 objetos
 *  escenciales, que son la linea de tiempo y el puntero de tiempo actual.
 *  Estos son agregados al momento de la creacion del modulo.
 */
#ifndef GTIMEQUEUE_H
#define GTIMEQUEUE_H

/* librerias generales */
#include <QWidget>
#include <QImage>
#include <QList>
#include <QString>
#include <QPoint>
#include <QColor>
#include <QtAlgorithms>
/* librerias propias */
#include "consts.h"
#include "debug.h"
#include "gtqobject.h"


class GTimeQueue : public QWidget
{
	public:
		/* Constructor.
		 * La escala inicial sera seteada en GTQ_NORMAL_SCALE (consts.h)
		*/
		GTimeQueue (void);
		
		
		/* ### ### ### Manipulacion de la escala ### ### ### */
		float getScale(void) {return this->scale;};
		
		/* Setea la escala, si esta fuera dentro de los rangos 
		 * especificados no hace nada
		 */
		void setScale(float scale);
		
		
		/* Funcion que agrega un elemento a la lista de objetos a ser
		 * impresos por pantalla.
		 * REQUIRES:
		 * 	obj != NULL
		 * NOTE: El obj NO debe ser liberado, pertenece ahora aca.
		 */
		void append(GTQObject *obj);
		
		/* Funcion que elimina un obj de la lista.
		* REQUIRES:
		* 	obj != NULL
		* NOTE: se libera la memoria
		*/
		void remove(GTQObject *obj);
		/* auxiliar */
		void remove(int pos);
		
		/* Funcion que obtiene un objeto determinado.
		 * REQUIRES:
		 * 	queueSize >= i >= 0
		 * RETURNS:
		 * 	GTQObject != 	NULL
		 * 	NULL		si hubo error
		 * NOTE: NO debe ser liberado el object
		 */
		const GTQObject* getObject(int i);
		
		
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
		
		/* Funcion que va a mover a derecha el visor de la cola.
		 * Esto hace que el punto de referencia se mueva hacia izquierda
		 * n lugares, donde n es la cantidad de pixeles que nos movemos
		 * REQUIRES:
		 * 	n >= 0
		 */
		void moveRigth(int n);
		
		/* Funcion que va a mover a izquierda el visor de la cola.
		* Esto hace que el punto de referencia se mueva hacia derecha
		* n lugares, donde n es la cantidad de pixeles que nos movemos
		* REQUIRES:
		* 	n >= 0
		*/
		void moveLeft(int n);
		
		
		/* Destructor */
		~GTimeQueue(void);
	
	
	/* los eventos */
	protected:
		/*void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);*/
		void paintEvent(QPaintEvent *event);
		/*void resizeEvent(QResizeEvent *event);*/

		
	private:
		/* Funciones */
		/* Funcion que se va a encargar de setear correctamente las
		 * propiedades (scale, pos, etc) de cada uno de los elementos
		 * ante alguna actualizacion (movimiento, reescala, etc) de 
		 * la pantalla.
		 */
		void updateElements(void);
		
		
		/* Atributos */
		
		/* En esta QImage vamos a mostrar todo, sobre esta nos vamos
		 * a basar, seria la "pizarra" */
		QImage display;
		/* El painter para nuestro display */
		QPainter * dispPainter;
		/* Imagen de fondo, puede ser Null o no. */
		QImage * backImg;
		/* en caso de que no haya imagen de fondo vamos a pintar con el
		 * color predeterminado. */
		QColor backColor;
		/* Lista de objetos a ser mostrados */
		QList<GTQObject *> objectsList;
		/* Lista de objetos que vamos a imprimir por pantalla. Esto es
		 * para hacer mas eficiente la impresion.
		 */
		QList<GTQObject *> printObjList;
		/* Posicion de referencia en cuanto al centro (esto va a servir
		 * cuando movemos y/o hacemos zoom, para determinar la nueva
		 * posicion de cada uno de los elementos a dibujar */
		QPoint refPos;
		
		/* escala a la que tienen que ser representados las cosas */
		float scale;
		
		
		
		
};

#endif