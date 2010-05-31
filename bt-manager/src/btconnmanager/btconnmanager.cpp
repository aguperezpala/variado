#include "btconnmanager.h"

/*!	###		FUNCIONES PRIVADAS		###	*/

/* funcion que hace cambia un struct fd de la posicion j a la posicion i
 * fdSet[i] = fdSet[j];
* REQUIRES:
* 	i,j <= fdSetSize
*/
void BTConnManager::fdSetAssign(int i, int j)
{
	assert(j < this->fdSetSize);
	assert(i < this->fdSetSize);
	
	this->fdSet[i].fd = this->fdSet[j].fd;
	this->fdSet[i].revents = this->fdSet[j].revents;
	this->fdSet[i].events = this->fdSet[j].events;
	
}


/* funcion que duplica el tamaño del fdSet */
void BTConnManager::increasefdSetSize(void)
{
	this->fdSetMaxSize *= 2;
	this->fdSet = (struct pollfd*) realloc((void *) this->fdSet,
						this->fdSetMaxSize);
	
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
		this->fdSetSize--;
	} else if (i < this->connPtr) {
		/* es un server.. */
		/* primero traemos el ultimo server */
		fdSetAssign(i, this->connPtr - 1);
		/* movemos la ultima conexion al espacio vacio */
		fdSetAssign(this->connPtr - 1, this->fdSetSize - 1);
		this->connPtr--;
		this->fdSetSize--;
		
	} else {
		/* es una conexion normal */
		fdSetAssign(i, this->fdSetSize - 1);
		this->fdSetSize--;
	}
	
	
	
	return result;
}

/* funcion que devuelve una conexion desde un fd
* RETURNS:
* 	NULL 	on error || if not exists
* 	BTConnection != NULL	on success
*/
BTConnection *BTConnManager::getConFromFd(int fd)
{
	list<BTConnection *>::iterator it;
	BTConnection *result = NULL;
	
	
	/* si no hubo error debemos buscar cual conexion produjo el evento.. */
	if (this->connList.size() == 0) {
		debugp("no tenemos conexiones!!!?\n");
		return result;
	}
	for (it = this->connList.begin(); it != this->connList.end(); ++it) {
		if (!(*it)) {
			debugp("con null?\n");
			ASSERT(false);
			continue;
		}
		if ((*it)->getSocket() == fd) {
			/* lo encontramos */
			result = *it;
			break;
		}
	}
	
	return result;
}

/* funcion que devuelve un server desde un fd
* RETURNS:
* 	NULL 	on error || if not exists
* 	BTSimpleServer != NULL	on success
*/
BTSimpleServer *BTConnManager::getServerFromFd(int fd)
{
	list<BTSimpleServer *>::iterator it;
	BTSimpleServer *result = NULL;
	
	
	/* si no hubo error debemos buscar cual conexion produjo el evento.. */
	if (this->serverList.size() == 0) {
		debugp("no tenemos servers!!!?\n");
		return result;
	}
	for (it = this->serverList.begin(); it != this->serverList.end(); ++it) {
		if (!(*it)) {
			debugp("con null?\n");
			ASSERT(false);
			continue;
		}
		if ((*it)->getSocket() == fd) {
			/* lo encontramos */
			result = *it;
			break;
		}
	}
	
	return result;
}


/* Funcion que devuelve el indice correspondiente en el fdSet
* segun un socket determinado.
* RETURNS:
* 	< 0	if not found
* 	i > 0	otherwise
*/
int BTConnManager::getIndexFromFd(int fd)
{
	int i = 0;
	
	for(i = 0; i < this->fdSetSize; i++) {
		if (this->fdSet[i].fd == fd)
			break;
	}
	
	if (i == this->fdSetSize)
		return -1;
	
	return i;
}

/* Funcion que devuelve la primera conexion que tiene revens
* distinto de 0, y en caso de que no haya ninguna conexion
* con revents != 0, hace poll esperando alguna.
* RETURNS:
* 	index (fdSet[index].revents != 0)
*	< 0	on error
*/
int BTConnManager::getIndexFdWithEvents(void)
{
	int i = 0;
	int pollResult = 0;
	
	for (i = 0; i < this->fdSetSize; i++){
		if (this->fdSet[i].revents != 0) {
			break;
		}
	}
	
	/* verificamos si hay alguna */
	if (i < this->fdSetSize) {
		/* tenemos la conexion iesima */
		return i;
	} else {
		/* hacemos poll */
		pollResult = poll(this->fdSet, this->fdSetSize, -1);
		if (pollResult <= 0) {
			perror("error en poll\n");
			return -1;
		}
	}
	for (i = 0; i < this->fdSetSize; i++){
		if (this->fdSet[i].revents != 0) {
			break;
		}
	}
	return i;
	
}



/*!	###		FUNCIONES PUBLICAS		### 	*/

/* constructor */
BTConnManager::BTConnManager(void)
{
	this->serverList.clear();
	this->connList.clear();
	this->fdSet = (struct pollfd *) calloc(10,sizeof(struct pollfd));
	this->fdSetMaxSize = 10;
	this->fdSetSize = 0;
	this->connPtr = 0;
}

/* funcion que permite agregar un server para chequear sus
* eventos.
* REQUIRES:
* 	server != NULL
*/
void BTConnManager::insertServer(BTSimpleServer *server)
{
	assert(server != NULL);
	this->serverList.push_back(server);
	addServerFdToSet(server->getSocket(), BTCM_SERVER_POLL_FLAGS);
}

/* Funcion que elimina un server de la lista para que
* deje de ser verificado
* REQUIRES:
* 	server != NULL
*/
void BTConnManager::removeServer(BTSimpleServer *server)
{
	assert(server != NULL);
	removeFdFromSet(server->getSocket());
	this->serverList.remove(server);
}
void BTConnManager::removeServer(int fd)
{
	removeServer(getServerFromFd(fd));
}

/* funcion que permite agregar una conexion para verificar
* sus eventos.
* REQUIRES:
* 	c 	!= NULL
*/
void BTConnManager::insertConnection(BTConnection *c)
{
	assert(c != NULL);
	this->connList.push_back(c);
	addConnFdToSet(c->getSocket(), BTCM_POLL_IN_FLAGS | BTCM_POLL_OUT_FLAGS);
}

/* Funcion que elimina una conexion de la lista para que
* deje de ser verificada
* REQUIRES:
* 	conn != NULL
*/
void BTConnManager::removeConnection(BTConnection *con)
{
	assert(con != NULL);
	removeFdFromSet(con->getSocket());
	this->connList.remove(con);
	
}
void BTConnManager::removeConnection(int fd)
{
	removeConnection(getConFromFd(fd));
}

/* Funcion que permite setear flags (events) de chequeo en una
* conexion determinada. (cuando se quiera enviar datos por ej
* debemos setearle el flag POLLOUT a la conexion..
* REQUIRES:
* 	c 	!= NULL
* 	c 	€ connList
*/
void BTConnManager::setFlags(BTConnection *c, short events)
{	
	int index = 0;
	
	if (c == NULL) {
		debugp("SetFlags con c null\n");
		return;
	}
	index = getIndexFromFd(c->getSocket());
	if (index < 0) {
		debugp("No se encuentra el fd\n");
		return;
	}
	/* si se encontro, entonces cambiamos las flags */
	this->fdSet[index].events = events;
	
}


/*! Funcion principal, encargada de devolver la conexion
* involucrada, y que tipo de evento sucedio. 
* ES BLOQUEANTE! hasta que sucede un evento
* NOTE: cuando una conexion no tiene mas datos que enviar
*	  automaticamente se desactiva el POLLOUT.
* NOTE: automaticamente se hace el recv | send.
* RETURNS:
* 	eType	= see poll
* 	addr 	= connection involved in the event
* 	result	= (operation result, i.e: bytes Send | received)
*
* NOTE: genera memoria
*/
BTConnection *BTConnManager::getConnEvent(eventType_t &ev, int &result)
{
	int i = 0;
	int fd = 0, events = 0;
	bool isFinish = false;
	BTConnection *newCon = NULL;
	
	
	
	while (!isFinish) {
		/* primero obtenemos la posicion de la conexion que tiene 
		* revents != 0 */
		i = getIndexFdWithEvents();
		if (i < 0){
			/* nos dio error. devolvemos error */
			ev = BTCM_EV_ERR;
			return NULL;
		}
		fd = this->fdSet[i].fd;
		events = this->fdSet[i].revents;
		this->fdSet[i].revents = 0;
		/*cout << "index: " << i << "\t events: " << events << endl;
		cout << "POLLOUT: " << POLLOUT << endl;
		cout << "POLLIN: " << POLLIN << endl;
		cout << "POLLPRI: " << POLLPRI << endl;
		cout << "POLLNVAL: " << POLLNVAL << endl;
		cout << "POLLRDHUP: " << POLLRDHUP << endl;
		cout << "POLLERR: " << POLLERR << endl;
		cout << "POLLHUP: " << POLLHUP << endl;
		*/
		/* tenemos el i-esimo fd con eventos... */
		if (i < this->connPtr) {
			/* es un server... */
			if(events & (BTCM_SERVER_POLL_FLAGS)) {
				/* estamos por aceptar una conexion */
				BTSimpleServer *ss = getServerFromFd(fd);
				/*! aca debemos asegurarnos que exista */
				assert(ss != NULL);
				newCon = ss->acceptConn(&result);
				result = fd;
				if (newCon == NULL){
					/*!### ERROR al aceptar una conexion */
					ev = BTCM_EV_ACCEPT_ERR;
					return NULL;
				}
				/* si la aceptamos la agregamos a la lista y agregamos
				* su fd */
				insertConnection(newCon);
				ev = BTCM_EV_NEW_CONN;
				return newCon;
			} else {
				/* estamos recibiendo un error de un server */
				debugp("Un server genero un evento inesperado\n");
				result = fd;
				ev = BTCM_EV_SERVER_ERR;
				return NULL;
			}
			
		} else {
			BTConnection *con = getConFromFd(fd);
			
			result = fd;
			assert(con != NULL);
			/* es una conexion... hay 3 casos, recepcion, envio,
			 * error. */
			/* verificamos si tiene o no datos para mandar */
			
			if (con->getSendBuff().size() != 0) {
				events = (events | (BTCM_POLL_OUT_FLAGS));
				this->fdSet[i].events = events;
			}
			if (events & (BTCM_POLL_IN_FLAGS)) {
				/* tenemos datos para leer */
				result = con->recvData();
				this->fdSet[i].revents = events ^ (BTCM_POLL_IN_FLAGS);
				ev = BTCM_EV_RCV;
				return con;
			} else if (events & (BTCM_POLL_OUT_FLAGS)) {
				/* si estamos aca es porque realmente debemos
				 * enviar datos */
				result = con->sendData();
				/* vemos si le quedaron datos */
				if (con->getSendBuff().size() == 0) {
					this->fdSet[i].revents = events ^ 
						(BTCM_POLL_OUT_FLAGS);
					continue;
				}
				ev = BTCM_EV_OUT;
				return con;
			} else{
			
				/*! si estamos aca es porque hubo un error, ya sea
				* porque nos cerraron la conexion o lo que sea */
				con->recvData();
				con->closeConnection();
				ev = BTCM_EV_CLOSE_CONN;
				return con;
			}
		}
	}
	
	ev = BTCM_EV_ERR;
	return NULL;
}


BTConnManager::~BTConnManager(void)
{
	if (this->fdSet != NULL)
		free(this->fdSet);
	
}

