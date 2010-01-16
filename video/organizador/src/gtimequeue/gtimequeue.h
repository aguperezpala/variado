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
#include <QPair>
/* librerias propias */
#include "consts.h"
#include "debug.h"
#include "gtqobject.h"
#include "timeline.h"
#include "timepointer.h"
#include "trigger.h"
#include "normalbox.h"


/*! Definimos las diferentes regiones donde vamos a mostrar las cosas en % */
/* dejamos un espacio en blanco al principio */
#define GTQ_FST_BLANK		12
/* lugar donde van a ir los medidores */
#define GTQ_METERS_SIZE		15
/* ahora definimos el tamaño de las cajas */
#define GTQ_BOXS_SIZE		45
/* definimos donde va a ir la linea de tiempo y que tamaño va a tener */
#define GTQ_TIMELINE_SIZE	5
/* el espacio donde van a ir los triggers */
#define GTQ_TRIGGERS_SIZE	23
/* donde va a ir el puntero de tiempo */
#define GTQ_TIMEPOINTER_SIZE	5



class GTimeQueue : public QWidget
{
	public:
		/* Constructor.
		 * La escala inicial sera seteada en GTQ_NORMAL_SCALE (consts.h)
		*/
		GTimeQueue (void);
		
		/* funcion que devuelve el tamaño que deben tener las cajas */
		int getBoxHeigth(void);
		
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
		
		/* funcion que devuelve la linea de tiempo para que se pueda
		 * configurar mejor :D */
		GTQTimeLine * getTimeLine(void){return this->timeLine;};
		
		/* Funcion que devuelve el timePointer para poder configurarlo
		 * mejor (setearle todo lo que uno quiera) */
		GTQTimePointer * getTimePointer(void){return this->timePointer;};
		
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
		 */
		void insertBoxObject(GTQNormalBox *obj);
		
		/* Funcion que va a mover un objeto determinado de la cola
		 * a un tiempo especifico.
		 * REQUIRES:
		 * 	obj != NULL
		 * 	obj € this->boxObjectsList
		 * POST:
		 * 	asegura que se mantenga ordenada la lista de objetos
		 * 	y no haya sobreposicion de los mismos
		 */
		void moveBoxObject(GTQNormalBox *obj, unsigned long long newStartMs);
		
		/* Funcion que elimina un obj de la lista.
		* REQUIRES:
		* 	obj != NULL
		* 	obj € this->boxObjectsList
		* POST:
		* 	asegura que el ordenamiento de los elementos se mantenga
		* 	sin dejar un "hueco" de tiempo libre.
		*/
		void removeBoxObject(GTQNormalBox *obj);
		
		/* Funcion que va a insertar un trigger
		 * REQUIRES:
		 * 	trig != NULL
		 * 	trig !€ this->triggerObjectsList
		 */
		void insertTriggerObject(GTQTrigger *trig);
		
		/* Funcion que elimina un trigger de la lista.
		* REQUIRES:
		* 	trig != NULL
		* 	trig € this->triggerObjectsList
		*/
		void removeTriggerObject(GTQTrigger *trig);
		
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
		/*void keyPressEvent( QKeyEvent * event );*/
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
		
		/* func auxiliar que va a agregar un par a la lista de pares 
		 * si y solo si no fue agregada anteriormente
		 */
		void addPair(QRect &, QList<GTQObject *>*);
		
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
		/* para las lineas de tiempo que van a ser una sola */
		QList<GTQObject *> lineObjList;
		/* para los time pointers, que pueden ser mas de uno */
		QList<GTQObject *> timePointerObjList;
		/* lista donde vamos a ir encolando que queremos hacer update
		 * que que NO queremos hacer update, vamos a refrezcar el
		 * rectangulo y redibujar la lista de objetos asociada para
		 * cada uno de los pares de la lista pairList */
		QList<QPair<QRect &, QList<GTQObject *>*>* > pairList;
		/* lista de todos los elementos */
		QList<GTQObject *> allObjList;
		/* Posicion de referencia de donde comienza a mostrarse la linea
		 * de tiempo, es el lateral izquierdo*/
		unsigned long long msRef;
		/* es la cantidad de milisegundos por pixel, osea, cuantos ms
		 * estan siendo representados en un solo pixel */
		unsigned long long scale;
		/* Linea de tiempo */
		GTQTimeLine *timeLine;
		GTQTimePointer *timePointer;
		/* cantidad de milisegundos ocupados en la linea de tiempo */
		unsigned long long timeUsed;
		/* ahora determinamos las zonas donde vamos a pintar las 
		 * distintas partes de la linea de tiempo */
		QRect boxPaintRect;
		QRect linePaintRect;
		QRect triggerPaintRect;
		QRect pointerPaintRect;
		QRect metersPaintRect;
		
};

#endif