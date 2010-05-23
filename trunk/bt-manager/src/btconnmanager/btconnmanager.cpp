#include "btconnmanager.h"

/* funcion que duplica el tamaño del fdSet */
void BTConnManager::increasefdSetSize(void)
{
	this->fdSetMaxSize *= 2;
	realloc((void *) this->fdSet,this->fdSetMaxSize);
	
}


/* Funcion que agrga un fd de una BTConnection al fd set
* RETURNS:
* 	false	on error 
* 	true	if success
*/
void BTConnManager::addConnFdToSet(int fd, int flags)
{
	/* primero verificamos que podamos meter mas en el fdSet */
	if (this->fdSetSize == fdSetMaxSize)
		increasefdSetSize();
	/* ahora vamos a agregar en la ultima posicion el fd .*/
	this->fdSet[this->fdSetSize].fd = fd;
	this->fdSet[this->fdSetSize].events = flags;
	/* si lo agregamos */
	this->fdSetSize++;
	
	
}

/* Funcion que agrga un fd de un Server al fd set
* RETURNS:
* 	false	on error 
* 	true	if success
*/
void BTConnManager::addServerFdToSet(int fd, int flags)
{
	/* vamos a agregar en las primeras posiciones del arreglo, donde se 
	 * encuentran los servers, asique vamos a mandar al primer fd de la
	 * conn al ultimo lugar
	 */
	addConnFdToSet(this->fdSet[this->connPtr].fd, 
			this->fdSet[this->connPtr].events);
	this->fdSet[this->connPtr].fd = fd;
	this->fdSet[this->connPtr].events = flags;
	this->connPtr++;
}

/* Funcion que saca un fd del set y lo re-ordena
* RETURNS:
* 	true 	if success
*	false	on error
*/
bool BTConnManager::removeFdFromSet(int fd)
{
	int i = 0;
	bool result = false;
	
	/* lo buscamos */
	for (i = 0; i < this->fdSetMaxSize; i++){
		if (this->fdSet[i].fd == fd)
			/* Lo encontramos ==> salimos */
			break;
	}
	
	/* verificamos que lo hayamos encontrado... */
	if (i == this->fdSetMaxSize) {
		/* NO lo encontramos ==> devolvemos error */
		return result;
	}
	/* si lo encontramos, debemos ver ahora que pos esta, si es la ultima
	* entonces directamente igualamos a 0 el arreglo */
	result = true;
	/* swap */
	if (i == this->fdSetSize - 1) {
		this->fdSet[i].fd = 0;
	} else if (i < this->connPtr) {
		/* es un server.. */
		this->fdSet[i].fd = this->fdSet[this->fdSetSize - 1].fd;
		this->fdSet[i].revents = this->fdSet[this->fdSetSize - 1].revents;
		this->fdSet[this->fdSetSize - 1].fd = 0;
		this->fdSet[this->fdSetSize - 1].revents = 0;
		
	}
	/* borramos 1 */
	this->fdSetSize--;
	
	return result;
}

/* funcion que devuelve una conexion desde un fd
* RETURNS:
* 	NULL 	on error || if not exists
* 	BTConnection != NULL	on success
*/
BTConnection *BTConnManager::getConFromFd(int fd);


/* constructor */
BTConnManager::BTConnManager(void)
{
	this->serverList.clear();
	this->connList.clear();
	this->fdSet = (struct pollfd *) calloc(10,sizeof(struct pollfd));
	int fdSetMaxSize = 10;
	int fdSetSize = 0;
	int connPtr = 0;
}

/* funcion que permite agregar un server para chequear sus
* eventos.
* REQUIRES:
* 	server != NULL
*/
void BTConnManager::insertServer(BTSimpleServer *server);

/* funcion que permite agregar una conexion para verificar
* sus eventos.
* REQUIRES:
* 	c 	!= NULL
*/
void BTConnManager::insertConnection(BTConnection *c);

/* Funcion que permite setear flags (events) de chequeo en una
* conexion determinada. (cuando se quiera enviar datos por ej
* debemos setearle el flag POLLOUT a la conexion..
* REQUIRES:
* 	c 	!= NULL
* 	c 	€ connList
*/
void BTConnManager::setFlags(short events);


/*! Funcion principal, encargada de devolver la conexion
* involucrada, y que tipo de evento sucedio. 
* ES BLOQUEANTE! hasta que sucede un evento
* NOTE: cuando una conexion no tiene mas datos que enviar
*	  automaticamente se desactiva el POLLOUT.
* NOTE: automaticamente se hace el recv | send.
* REQUIRES:
* 	addr != NULL
* RETURNS:
* 	eType	= see poll
* 	addr 	= connection involved in the event
* 	result	= (operation result, i.e: bytes Send | received)
*
* NOTE: genera memoria
*/
int BTConnManager::getConnEvent(btaddr_t *addr, int &result);



