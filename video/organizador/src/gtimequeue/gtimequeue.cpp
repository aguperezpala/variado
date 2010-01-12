#include "gtimequeue.h"


/*! ###			Funciones privadas			### */

/* Funciones que van a hacer un repaint de los diferentes
* elementos segun el rectangulo especificado para cada zona.
* NOTE: Estas funciones setean la printListObjects con los
* 	elementos que querramos imprimir segun el sector
* 	determinado.
*/
void repaintBox(void)
{
	QList<GTQObject *>::iterator i;
	
	/* limpiamos la lista de objetos a imprimir */
	this->printObjList.clear();
	
	/* ahora le asignamos los elementos que queremos */
	for (i = this->boxObjectsList.begin(); i != this->boxObjectsList.end(); ++i)
		if (*i) {
			if ((*i)->haveToPaint(this->boxPaintRect, this->msRef))
				/* si es un objeto que lo tenemos que mostrar =>
				* lo agregamos a la lista de mostrables */
				this->printObjList.append((*i));
		}
	/* ahora los dibujamos */
	repaint(this->boxPaintRect);
}
void repaintTriggers(void)
{
	QList<GTQObject *>::iterator i;
	
	/* limpiamos la lista de objetos a imprimir */
	this->printObjList.clear();
	
	/* ahora le asignamos los elementos que queremos */
	for (i = this->triggerObjectsList.begin(); i != this->triggerObjectsList.end(); ++i)
		if (*i) {
			if ((*i)->haveToPaint(this->triggerPaintRect, this->msRef))
				/* si es un objeto que lo tenemos que mostrar =>
				* lo agregamos a la lista de mostrables */
				this->printObjList.append((*i));
		}
		/* ahora los dibujamos */
	repaint(this->triggerPaintRect);
}
void repaintTimeLine(void)
{
	/* limpiamos la lista de objetos a imprimir */
	this->printObjList.clear();
	
	/* la agregamos a la linea de tiempo */
	this->printObjList.append(this->timeLine);
	
	repaint(this->linePaintRect);
}
void repaintTimePointer(void)
{
	/* limpiamos la lista de objetos a imprimir */
	this->printObjList.clear();
	
	/* agregamos el o los punteros */
	this->printObjList.append(this->timePointer);
	
	repaint(this->pointerPaintRect);
}
void repaintAll(void)
{
	/* lo hacemos facil */
	repaintBox();
	repaintTriggers();
	repaintTimeLine();
	repaintTimePointer();
	
	repaint(this->rect());
}

/* Funcion que va a establecer las diferentes regiones (QRects)
* donde se van a mostrar las diferentes partes segun el tamaño
* de la pantalla.
* NOTE: Usa los defines para establecer las regiones.
*/
void updateRegiones(void)
{
	/*! TODO: establecer los porcentajes como defines */
}



/* Funcion que pone un elemento detras de otro teniendo en 
* cuenta que la lista esta ordenada de menor a mayor (en cuanto
* a los comienzos en ms)
* Lo que hace es sacar los posibles espacios en blanco y/o
* solapamientos.
* NOTE: tambien calculamos el tiempo total de reproduccion (timeUsed).

*/
void ordinateElements(void)
{
	int i = 0;
	int size = this->boxObjectsList.size()-1;
	unsigned long long delta = 0;
	unsigned long long totalTime = 0;
	GTQObject *obj = NULL;
	
	for (i = 0; i < size; i++) {
		obj = this->boxObjectsList.at(i);
		if (obj == NULL) {
			debugp("Estamos teniendo un error, tenemos un elemento "
				"nulo aca\n");
			/* lo sacamos de la lista */
			this->boxObjectsList.removeOne(obj);
			continue;
		}
		/* lo posicionamos donde corresponde */
		obj->setStartMs(delta);
		
		/* calculamos el nuevo fin == comienzo del siguiente elemento */
		delta = obj->getStartMs() + obj->getDurationMs();
		totalTime += obj->getDurationMs();
	}
	this->timeUsed = totalTime;
}



/*! ###			Funciones publicas			### */

/* Constructor.
* La escala inicial sera seteada en GTQ_NORMAL_SCALE (consts.h)
*/
GTimeQueue::GTimeQueue (void)  : backImg(NULL), timeUsed(0)
{
	QColor timeLineColor = QColor::fromRgb(0,0,0);
	
	/* seteamos los valores por defecto */
	this->backColor = QPalette(QColor::fromRgb(255,255,255));
	this->setPalette (this->backColor);
	this->scale = GTQ_NORMAL_SCALE;
	
	/* configuramos el punto de referencia de la linea de tiempo */
	this->msRef = 0;
	
	/* configuramos las diferentes regiones */
	updateRegiones();
	
	this->scale = 0;
	setPointerMs(0);
	
	/*! aca deberiamos setear las configuraciones de la linea de tiempo,
	 *  el estilo y esas cosas.. */ 
	this->timeLine = new GTQTimeLine();
	this->timeLine->setScale(this->scale);
	this->timeLine->setStartMs(this->refPos.x());
	this->timeLine->setColor(timeLineColor);
	
	/*! TODO: configurar / crear el timePointer */
	
}
	
	

/* Setea la escala, si esta fuera dentro de los rangos 
* especificados no hace nada
*/
void GTimeQueue::setScale(unsigned long long scale)
{
	QList<GTQObject *>::iterator i;
	
	if(scale > GTQ_MAX_SCALE || scale < GTQ_MIN_SCALE) {
		/* no esta dentro de los rangos posibles */
		debugp("Escala incorrecta\n");
		return;
	}
	/* si esta dentro de los rangos... seteamos */
	this->scale = scale;
	/* ahora para cada uno de los objetos hacemos lo mismo. */
	for (i = this->boxObjectsList.begin(); i != this->boxObjectsList.end(); ++i)
		if (*i)
			(*i)->setScale(scale);
	
	updateElements();
}

/* funcion que setea el color de la linea de tiempo */
void setTimeLineColor(QColor c)
{
	/*! TODO */
}


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
void insertBoxObject(GTQObject *obj)
{
	unsigned long long actualStart = 0;
	QList<GTQObject *>::iterator i;
	int pos = 0;
	
	/* pre */
	if(obj == NULL || this->boxObjectsList.contains(obj)) {
		ASSERT(obj != NULL);
		/* estamos agregandolo 2 veces? */
		ASSERT(this->boxObjectsList.contains(obj));
		return;
	}
	
	/* vamos a buscar cual es la posicion que le corresponde, una vez
	 * encontrada la posicion vamos a insertarlo e iterar sobre los
	 * demas elementos para establecer sus nuevos comienzos 
	 */
	actualStart = obj->getStartMs();
	for (i = this->boxObjectsList.begin(); i != this->boxObjectsList.end(); ++i)
		if (*i) {
			/* verificamos quien va primero */
			if ((*i)->getStartMs() <= actualStart)
				pos++;
			else
				/* salimos, tenemos que insertarlo aca */
				i = this->boxObjectsList.end(); /* == break; */
		}
	
	/* ahora en pos debemos insertar el elemento */
	ASSERT(pos <= this->boxObjectsList.size());
	this->boxObjectsList.insert(pos, obj);
	
	/* seteamos la escala */
	obj->setScale(this->scale);
	
	
	/* acomodamos la linea de tiempo actualizando todos los elementos
	 * posteriores al recien insertado si y solo si el que acabamos
	 * de insertar no fue metido al ultimo */
	ordinateElements();
	/* aca tenemos ya los elementos ordenados y actualizados */

	/* dibujamos las cajas de nuevo */
	repaintBox();
}


/* Funcion que va a mover un objeto determinado de la cola
* a un tiempo especifico.
* REQUIRES:
* 	obj != NULL
* 	obj € this->boxObjectsList
* POST:
* 	asegura que se mantenga ordenada la lista de objetos
* 	y no haya sobreposicion de los mismos
*/
void moveBoxObject(GTQObject *obj, unsigned long long newStartMs)
{
	
	/* pre */
	if (obj == NULL || !(this->boxObjectsList.contains(obj))) {
		ASSERT(false);
		return;
	}
	/* si pertenece entonces lo que vamos hacer es sacar el elemento de la
	 * lista, ordenar los "boxs" e insertar de nuevo para mantener el orden
	 * en la nueva posicion */
	
	/* lo eliminamos */
	if (!this->boxObjectsList.removeOne(obj)) {
		debugp("No pudimos mover el objeto....:S\n");
		return;
	}
	/* ordenamos los elementos */
	ordinateElements();
	
	/* modificamos su "comienzo" */
	obj->setStartMs(newStartMs);
	
	/* lo re-insertamos como si fuese uno nuevo para mantener el orden */
	insertBoxObject(obj);
}

/* Funcion que elimina un obj de la lista.
* REQUIRES:
* 	obj != NULL
* 	obj € this->boxObjectsList
* POST:
* 	asegura que el ordenamiento de los elementos se mantenga
* 	sin dejar un "hueco" de tiempo libre.
* NOTE: se libera la memoria
*/
void removeBoxObject(GTQObject *obj)
{
	/* pre */
	if (obj == NULL || !(this->boxObjectsList.contains(obj))) {
		ASSERT(false);
		return;
	}
	/* eliminamos el objeto de la lista y re-ordenamos los elementos */
	this->boxObjectsList.removeOne(obj);
	ordinateElements();
	
	/* verificamos si tenemos que eliminarlo de la printList */
	if (this->printObjList.contains(obj))
		this->printObjList.removeOne(obj);
	
	/* dibujamos de nuevo las cajas */
	repaintBox();
	
	/* liberamos memoria */
	delete obj; obj = NULL;
	
}

/* Funcion que va a insertar un trigger
* REQUIRES:
* 	trig != NULL
* 	trig !€ this->triggerObjectsList
* NOTE: No debe ser liberada la memoria una vez insertado.
*/
void insertTriggerObject(GTQObject *trig)
{
	/* pres */
	if (trig == NULL || this->triggerObjectsList.contains(trig)) {
		ASSERT(false);
		return;
	}
	/* agregamos el elemento a la lista, no importa el orden aca */
	this->triggerObjectsList.append(trig);
	repaintTrigger();
}

/* Funcion que elimina un trigger de la lista.
* REQUIRES:
* 	trig != NULL
* 	trig € this->triggerObjectsList
* NOTE: se libera la memoria
*/
void removeTriggerObject(GTQObject *trig)
{
	/* pres */
	if (trig == NULL || !(this->triggerObjectsList.contains(trig))) {
		ASSERT(false);
		return;
	}
	this->triggerObjectsList.removeOne(trig);
	
	/* si se encuentra en la lista de elementos a pintar lo eliminamos */
	if (this->printObjList.contains(trig))
		this->printObjList.removeOne(trig);
	
	repaintTrigger();
	
	/* liberamos memoria */
	delete trig; trig = NULL;
	
}

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


/* Funcion que elimina un obj de la lista.
* REQUIRES:
* 	obj != NULL
* NOTE: se libera la memoria
*/
void GTimeQueue::remove(GTQObject *obj)
{
	/* pre */
	if(obj == NULL) {
		ASSERT(obj != NULL);
		return;
	}
	/* lo sacamos de las listas, ya que puede ser que este siendo graficado */
	if (this->boxObjectsList.removeOne(obj)) {
		/* si estaba aca puede que este aca */
		this->printObjList.removeOne(obj);
		
		/* decrementamos el tiempo total utilizado por la linea de 
		 * tiempo */
		this->timeUsed -= obj->getDurationMs();
	}
	/* liberamos memoria */
	delete obj;
	updateElements();
	
}


/* Funcion que obtiene un objeto determinado.
* REQUIRES:
* 	queueSize >= i >= 0
* RETURNS:
* 	GTQObject != NULL
* NOTE: NO debe ser liberado el object
*/
const GTQObject* GTimeQueue::getObject(int i)
{
	GTQObject * obj = NULL;
	
	/* pre */
	if(i < 0 || i >= this->boxObjectsList.size()) {
		ASSERT(false);
		return NULL;
	}
	obj = this->boxObjectsList.at(i);
	
	return obj;
}


/* Funcion que va a setear la imagen de fondo, si ya existe
* alguna, sera eliminada (liberada la memoria) y se seteara
* la nueva imagen.
* REQUIRES:
* 	si img != NULL && no liberar img una vez seteada
* 	si img == NULL => solo borramos la anterior y seteamos
* 			  ninguna imagen de fondo.
* NOTE: si se libera la img tamos al horno
*/
void GTimeQueue::setBackImg(QImage * img)
{
	if (this->backImg != NULL)
		/* borramos la img actual */
		delete this->backImg;
	this->backImg = img;
}


/* Funcion que va a mover a derecha el visor de la cola.
* Esto hace que el punto de referencia se mueva hacia izquierda
* n lugares, donde n es la cantidad de pixeles que nos movemos
* REQUIRES:
* 	n >= 0
*/
void GTimeQueue::moveRigth(int n)
{
	QList<GTQObject *>::iterator i;
	unsigned long long ms = 0;
	
	if(n < 0) {
		/* no esta dentro de los rangos posibles */
		debugp("Movimiento a derecha incorrecto\n");
		return;
	}
	
	/* si nos vamos a mover a la derecha debemos ver que no estamos saliendo
	 * de las 24 hs, osea que debemos ver que la cantidad de pixeles
	 * que nos vamos a mover no deba superar los GTQ_MAX_MS_SUPPORTED ms
	 */
	
	/* primero calculamos la cantidad de milisegundos que nos vamos a mover
	 * teniendo en cuenta la escala y n. */
	ms = n * this->scale;
	/* TODO: overflow? */
	/* ahora verificamos si nos vamos al chori o no */
	if ((unsigned long long) (ms + this->refPos.x()) > GTQ_MAX_MS_SUPPORTED)
		return; /* no nos podemos mover mas */
	/* else */
	this->refPos.rx() += ms;
	
	updateElements();
}
/* Funcion que va a mover a izquierda el visor de la cola.
* Esto hace que el punto de referencia se mueva hacia derecha
* n lugares, donde n es la cantidad de pixeles que nos movemos
* REQUIRES:
* 	n >= 0
*/
void GTimeQueue::moveLeft(int n)
{
	QList<GTQObject *>::iterator i;
	unsigned long long ms = 0;
	
	if(n < 0) {
		/* no esta dentro de los rangos posibles */
		debugp("Movimiento a izquierda incorrecto\n");
		return;
	}
	
	ms = n * this->scale;
	/* TODO: overflow? */
	/* ahora verificamos si nos vamos al chori o no */
	if ((int)(this->refPos.x() - ms) < 0) {
		return; /* no nos podemos mover mas */
	}
	/* else */
	this->refPos.rx() -= ms;
	
	
	updateElements();
}

/* Destructor */
GTimeQueue::~GTimeQueue(void)
{
	QList<GTQObject *>::iterator i;
	
	/* vamos a liberar cada uno de los objects */
	for (i = this->boxObjectsList.begin(); i != this->boxObjectsList.end(); ++i)
		if (*i)
			delete *i;
	
	/* ahora vamos a liberar la imagen de fondo si es que la hay */
	if (this->backImg != NULL)
		delete this->backImg;
	
	/* borramos todo de las listas */
	this->printObjList.clear();
	this->boxObjectsList.clear();
}


/*! ###			EVENTOS			### */

void GTimeQueue::paintEvent(QPaintEvent *event)
{
	QList<GTQObject *>::iterator i;
	
	this->dispPainter.begin(this);
	
	/* verificamos si tenemos una imagen de fondo que re-dibjar, y solo
	 * vamos a redibujar el sector correspondiente */
	
	if (this->backImg != NULL) {
		QRect normalizedRect;
		
		/* calculamos el rectangulo que deberiamos usar para extraer
		 * la porcion deseada de la imagen */
		normalizedRect.setWidth(this->backImg->width() *
			event->rect().width() / this->rect().width());
		normalizedRect.setHeigth(this->backImg->heigth() *
			event->rect().heigth() / this->rect().heigth());
		
		/* redibujamos solo la parte que tenemos que actualizar */
		this->dispPainter.drawImage(event->rect(), *this->backImg,
					     normalizedRect);
	} else {
		/* si no dibujamos con el fondo de pantalla correspondiente */
		this->dispPainter.fillRect(event->rect(), this->backColor.base());
	}
	
	/*! NOTE: aca debemos tener en cuenta que tenemos en printObjList los
	 * 	 elementos que queremos que sean impresos. Estos van a ser
	 * 	 seteados en las repaintXXX functions
	 */
	
	for (i = this->printObjList.begin(); i != this->printObjList.end(); ++i)
		/*! if (*i) esto es fucking seguro.. no puede ser NULL */
		(*i)->paint(&this->dispPainter, event->rect(), this->msRef);
	
	this->dispPainter.end();
}


void GTimeQueue::resizeEvent(QResizeEvent *event)
{
	updateElements();
}


void GTimeQueue::keyPressEvent ( QKeyEvent * event )
{
	switch (event->key()) {
		case Qt::Key_Right:
			moveRigth(5);
			printf("Moviendo Derecha\n");
			break;
		
		case Qt::Key_Left:
			moveLeft(5);
			printf("Moviendo Izq\n");
			break;
	}
}