/*! Este modulo es el encargado de mostrar la linea de tiempo graficamente,
 *  vamos a poder insertar diferentes tipos de objetos. Existen 2 objetos
 *  escenciales, que son la linea de tiempo y el puntero de tiempo actual.
 *  Estos son agregados al momento de la creacion del modulo.
 *  
 *  Vamos a trabajar de la siguiente forma: La linea de tiempo va a comenzar
 *  en el milisegundo 0 y la escala mas chica de la misma son los milisegundos.
 *  El punto de referencia es los milisegundos en el eje X.
 *  
 *  Vamos a soportar 4 tipos de objetos mostrables:
 *   1 - Cajas que van a representar las peliculas.
 *   2 - Saltos, que van a ser los "triggers".
 *   3 - Linea de tiempo.
 *   4 - Puntero de tiempo actual.
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
#include <QPalette>
#include <QtAlgorithms>
#include <QKeyEvent>
/* librerias propias */
#include "consts.h"
#include "debug.h"
#include "gtqobject.h"
#include "timeline.h"



class GTimeQueue : public QWidget
{
	public:
		/* Constructor.
		 * La escala inicial sera seteada en GTQ_NORMAL_SCALE (consts.h)
		*/
		GTimeQueue (void);
		
		
		/* Funcion que devuelve la cantidad de milisegundos que estan
		 * ocupados en la linea de tiempo.
		 */
		unsigned long long getTimeUsedMs(void){return this->timeUsed;};
		
		
		/* ### ### ### Manipulacion de la escala ### ### ### */
		unsigned long long getScale(void) {return this->scale;};
		
		/* Setea la escala, si esta fuera dentro de los rangos 
		 * especificados no hace nada
		 */
		void setScale(unsigned long long scale);
		
		/* Setea el color de fond */
		void setBackColor(QPalette & color){this->backColor = color;};
		
		
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
		
		
		
		/* funcion que setea el color de la linea de tiempo */
		void setTimeLineColor(QColor c);
		
		
		/* Funcion que agrega un elemento a la lista de objetos a ser
		 * impresos por pantalla.
		 * REQUIRES:
		 * 	obj != NULL
		 * 	obj !€ this->boxObjectsList
		 * 	type(obj) == GTQObject
		 * INVARIANTE:
		 * 	Luego de agregar un objeto de este tipo, NO existe
		 * 	la superposicion de estos elementos, y la lista
		 * 	se mantiene ordenada segun el comienzo de cada elemento.
		 * NOTE: El obj NO debe ser liberado, pertenece ahora aca.
		 */
		void insertBoxObject(GTQObject *obj);
		
		/* Funcion que va a mover un objeto determinado de la cola
		 * a un tiempo especifico.
		 * REQUIRES:
		 * 	obj != NULL
		 * 	obj € this->boxObjectsList
		 * POST:
		 * 	asegura que se mantenga ordenada la lista de objetos
		 * 	y no haya sobreposicion de los mismos
		 */
		void moveBoxObject(GTQObject *obj, unsigned long long newStartMs);
		
		/* Funcion que elimina un obj de la lista.
		* REQUIRES:
		* 	obj != NULL
		* 	obj € this->boxObjectsList
		* POST:
		* 	asegura que el ordenamiento de los elementos se mantenga
		* 	sin dejar un "hueco" de tiempo libre.
		* NOTE: se libera la memoria
		*/
		void removeBoxObject(GTQObject *obj);
		
		/* Funcion que va a insertar un trigger
		 * REQUIRES:
		 * 	trig != NULL
		 * 	trig !€ this->triggerObjectsList
		 * NOTE: No debe ser liberada la memoria una vez insertado.
		 */
		void insertTriggerObject(GTQObject *trig);
		
		/* Funcion que elimina un trigger de la lista.
		* REQUIRES:
		* 	trig != NULL
		* 	trig € this->triggerObjectsList
		* NOTE: se libera la memoria
		*/
		void removeTriggerObject(GTQObject *trig);
		
		/* funcion que va a setear el puntero a un tiempo en ms 
		 * determinado */
		 void setPointerMs(unsigned long long ms);
		 
		 /* Funcion que va a devolver la posicion actual de donde
		  * comienza la linea de tiempo (lateral izquierdo), osea la
		  * referencia
		  */
		 unsigned long long getDisplayRef(void){return this->msRef;};
		 
		 /* Funcion que va a mover la referencia a un nuevo "tiempo"
		  * (aca se actualiza que objetos vamos a mostrar y cuales
		  * no)
		  */
		 void setDisplayRef(unsigned long long ref);
		
		/* Destructor */
		~GTimeQueue(void);
	
	
	/* los eventos */
	protected:
		/*void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);*/
		void paintEvent(QPaintEvent *event);
		/*! para debug momentaneamente */
		void keyPressEvent( QKeyEvent * event );
		void resizeEvent(QResizeEvent *event);

		
	private:
		/* Funciones */
		
		/* Funciones que van a hacer un repaint de los diferentes
		 * elementos segun el rectangulo especificado para cada zona.
		 * NOTE: Estas funciones setean la printListObjects con los
		 * 	elementos que querramos imprimir segun el sector
		 * 	determinado.
		 */
		void repaintBox(void);
		void repaintTriggers(void);
		void repaintTimeLine(void);
		void repaintTimePointer(void);
		void repaintAll(void);
		
		/* Funcion que va a establecer las diferentes regiones (QRects)
		 * donde se van a mostrar las diferentes partes segun el tamaño
		 * de la pantalla.
		 * NOTE: Usa los defines para establecer las regiones.
		 */
		void updateRegiones(void);
		 
		/* Funcion que pone un elemento detras de otro teniendo en 
		 * cuenta que la lista esta ordenada de menor a mayor (en cuanto
		 * a los comienzos en ms)
		 * Lo que hace es sacar los posibles espacios en blanco y/o
		 * solapamientos
		 * NOTE: tambien calculamos el tiempo total de reproduccion
		 *  	 (timeUsed).
		 */
		void ordinateElements(void);
		
		/* Atributos */
		
		/* El painter para nuestro display */
		QPainter dispPainter;
		/* Imagen de fondo, puede ser Null o no. */
		QImage * backImg;
		/* en caso de que no haya imagen de fondo vamos a pintar con el
		 * color predeterminado. */
		QPalette backColor;
		/* Lista de objetos (cajas) a ser mostrados */
		QList<GTQObject *> boxObjectsList;
		/* lista de "triggers" a ser mostrados */
		QList<GTQObject *> triggerObjectsList;
		/* Lista de objetos que vamos a imprimir por pantalla. Esto es
		 * para hacer mas eficiente la impresion.
		 */
		QList<GTQObject *> printObjList;
		/* Posicion de referencia de donde comienza a mostrarse la linea
		 * de tiempo, es el lateral izquierdo*/
		unsigned long long msRef;
		/* es la cantidad de milisegundos por pixel, osea, cuantos ms
		 * estan siendo representados en un solo pixel */
		unsigned long long scale;
		/* Linea de tiempo */
		GTQTimeLine *timeLine;
		/*! TODO: GTQTimePointer *timePointer; */
		/* cantidad de milisegundos ocupados en la linea de tiempo */
		unsigned long long timeUsed;
		/* ahora determinamos las zonas donde vamos a pintar las 
		 * distintas partes de la linea de tiempo */
		QRect boxPaintRect;
		QRect linePaintRect;
		QRect triggerPaintRect;
		QRect pointerPaintRect;
		
};

#endif