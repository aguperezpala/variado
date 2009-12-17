#include "gtimequeue.h"


/*! ###			Funciones privadas			### */
/* Funcion que se va a encargar de setear correctamente las
* propiedades (scale, pos, etc) de cada uno de los elementos
* ante alguna actualizacion (movimiento, reescala, etc) de 
* la pantalla.
*/
void GTimeQueue::updateElements(void)
{
	QList<GTQObject *>::iterator i;
	QRect aux;
	/* vamos a chequear que elementos deberian ser mostrados en pantalla
	 * y que elementos no. por lo que vamos a vaciar la lista de elementos
	 * actuales y agregamos de cero.
	 */
	
	this->printObjList.clear();	/* limpiamos */
	for (i = this->objectsList.begin(); i != this->objectsList.end(); ++i)
		if (*i) {
			aux = this->rect();
			if ((*i)->haveToPaint(aux, this->refPos.x()))
				/* si es un objeto que lo tenemos que mostrar =>
				* lo agregamos a la lista de mostrables */
				this->printObjList.append((*i));
		}
	
	/* hacemos un repaint para actualizar la pantalla */
	repaint();
}



/*! ###			Funciones publicas			### */

/* Constructor.
* La escala inicial sera seteada en GTQ_NORMAL_SCALE (consts.h)
*/
GTimeQueue::GTimeQueue (void)  : backImg(NULL)
{
	QColor timeLineColor = QColor::fromRgb(0,0,0);
	
	/* seteamos los valores por defecto */
	this->backColor = QPalette(QColor::fromRgb(255,255,255));
	this->setPalette (this->backColor);
	this->scale = GTQ_NORMAL_SCALE;
	
	/* configuramos el punto de referencia de la linea de tiempo */
	this->refPos.setX(0);
	/* la vamos a posicionar 3/4 abajo del display */
	this->refPos.setY((int)(this->height()/2));
	/* TODO: ahora deberiamos insertar la linea de tiempo y el puntero
	 * 	 que señalaria en que "tiempo" actual estamos... */
	/*! aca deberiamos setear las configuraciones de la linea de tiempo,
	 *  el estilo y esas cosas.. */ 
	this->timeLine = new GTQTimeLine();
	this->timeLine->setScale(this->scale);
	this->timeLine->setStartMs(this->refPos.x());
	this->timeLine->setColor(timeLineColor);
	append(this->timeLine);
	updateElements();
	
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
	for (i = this->objectsList.begin(); i != this->objectsList.end(); ++i)
		if (*i)
			(*i)->setScale(scale);
	
	updateElements();
}

/* Funcion que agrega un elemento a la lista de objetos a ser
* impresos por pantalla.
* REQUIRES:
* 	obj != NULL
* NOTE: El obj NO debe ser liberado, pertenece ahora aca.
*/
void GTimeQueue::append(GTQObject *obj)
{
	/* pre */
	if(obj == NULL) {
		ASSERT(obj != NULL);
		return;
	}
	
	/* agregamos un elemento... */
	this->objectsList.append(obj);
	/* seteamos la escala */
	obj->setScale(this->scale);
	/* reordenamos los elementos para dibujarlos en orde correcto */
	/*qSort(this->objectsList.begin(), this->objectsList.end());*/
	
}

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
	if (this->objectsList.removeOne(obj))
		/* si estaba aca puede que este aca */
		this->printObjList.removeOne(obj);
	updateElements();
	
}
/* auxiliar */
void GTimeQueue::remove(int pos)
{
	GTQObject * obj = NULL;
	
	/* pre */
	if(pos < 0 || pos >= this->objectsList.size()) {
		ASSERT(false);
		return;
	}
	/* ahora obtenemos el elemento i-esimo y lo eliminamos */
	obj = this->objectsList.at(pos);
	if (obj != NULL)
		remove(obj);
	
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
	if(i < 0 || i >= this->objectsList.size()) {
		ASSERT(false);
		return NULL;
	}
	obj = this->objectsList.at(i);
	
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
	for (i = this->objectsList.begin(); i != this->objectsList.end(); ++i)
		if (*i)
			delete *i;
	
	/* ahora vamos a liberar la imagen de fondo si es que la hay */
	if (this->backImg != NULL)
		delete this->backImg;
	
	/* borramos todo de las listas */
	this->printObjList.clear();
	this->objectsList.clear();
}


/*! ###			EVENTOS			### */

void GTimeQueue::paintEvent(QPaintEvent *event)
{
	QList<GTQObject *>::iterator i;
	
	this->dispPainter.begin(this);
	/* verificamos si tenemos una imagen de fondo que re-dibjar */
	if (this->backImg != NULL) {
		/* redibujamos toda la pantalla */
		this->dispPainter.drawImage(QPoint(0, 0), *this->backImg);
	} else {
		/* si no dibujamos con el fondo de pantalla correspondiente */
		/*!nada?*/
	}
	/* lo que hacemos en este caso es dibujar todos los elementos que tenemos
	 * sobre nuestro display */
	for (i = this->printObjList.begin(); i != this->printObjList.end(); ++i)
		/*! if (*i) esto es fucking seguro.. no puede ser NULL */
		(*i)->paint(&this->dispPainter, this->refPos);
	
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