#include "btservermanager.h"

/*!			funciones privadas				*/

/* Funcion que agrga un fd al ultimo lugar del fdSet.
* RETURNS:
* 	false	on error || fdSet.size() == MAX_CON_PER_DONGLE
* 	true	if success
*/
bool BTServerManager::addFdToSet(int fd, int flags)
{
	bool result = false;
	
	/* verificamos que no hayamos salido por la guarda */
	if (this->fdSetSize >= MAX_CON_PER_DONGLE + BTSM_MAX_SERVERS) {
		/* no agregamos un chori */
		return result;
	}
	
	this->fdSet[this->fdSetSize].fd = fd;
	this->fdSet[this->fdSetSize].events = flags;
	/* si lo agregamos */
	result = true;
	this->fdSetSize++;
	
	return result;
}

/* Funcion que saca un fd del set y lo re-ordena
* RETURNS:
* 	true 	if success
*	false	on error
*/
bool BTServerManager::removeFdFromSet(int fd)
{
	int i = 0;
	bool result = false;
	
	/* lo buscamos */
	for (i = 0; i < MAX_CON_PER_DONGLE + BTSM_MAX_SERVERS; i++){
		if (this->fdSet[i].fd == fd)
			/* Lo encontramos ==> salimos */
			break;
	}
	
	/* verificamos que lo hayamos encontrado... */
	if (i == MAX_CON_PER_DONGLE + BTSM_MAX_SERVERS) {
		/* NO lo encontramos ==> devolvemos error */
		return result;
	}
	/* si lo encontramos, debemos ver ahora que pos esta, si es la ultima
	 * entonces directamente igualamos a 0 el arreglo */
	result = true;
	/* swap */
	if (i == this->fdSetSize - 1) {
		this->fdSet[i].fd = 0;
	} else {		
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
BTConnection *BTServerManager::getConFromFd(int fd)
{
	list<BTConnection *>::iterator it;
	BTConnection *result = NULL;
	
	
	/* si no hubo error debemos buscar cual conexion produjo el evento.. */
	if (this->conList.size() == 0) {
		debugp("no tenemos conexiones!!!?\n");
		return result;
	}
	for (it = this->conList.begin(); it != this->conList.end(); ++it) {
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

/* funcion que devuelve un server de un fd determinado:
* RETURNS;
* 	NULL 		if no Server with that fd
* 	BTSS != NULL	otherwise
*/
BTSimpleServer *BTServerManager::getSSFromFd(int fd)
{
	BTSimpleServer *result = NULL;
	list<BTSimpleServer *>::iterator it;
	
	for(it = this->serversList.begin();it != this->serversList.end(); ++it){
		if (!(*it)) {
			debugp("server null?\n");
			ASSERT(false);
			continue;
		}
		if((*it)->getSocket() == fd){
			result = (*it);
			break;
		}
	}
	
	return result;
}


/*! 			Funciones publicas 				*/

/* Constructor 
* REQUIRES:
* 	btDD != NULL
*/
BTServerManager::BTServerManager(BTDongleDevice *btDD)
{
	int i = 0;
	
	if (!btDD) {
		debugp("Error al crear el BTServerManager, btDD Null\n");
		ASSERT(btDD != NULL);
	}
	this->btDD = btDD;
	
	this->serversList.clear();
	this->sdpList.clear();
	this->conList.clear();
	
	pthread_mutex_init(&this->mutex, NULL);
	
	memset(&this->fdSet, 0, sizeof(this->fdSet));
	/* seteamos los flags para todos los pollfd */
	for (i = 0; i < MAX_CON_PER_DONGLE + BTSM_MAX_SERVERS; i++) {
		this->fdSet[i].events = (short) BTSM_POLL_FLAGS;
	}
	this->fdSetSize = 0;
}

/* Funcion que setea el tiempo maximo de inactividad para las
* conexiones.
* FIXME: deberiamos hacer un TAD que se puedan agregar
* interrupciones asociadas a funciones que ejecutar ante
* el transcurso de un tiempo determinado...
* Por el momento vamos a chequear periodicamente todas las
* conexiones y vamos a verificar si tenemos 
*/
void BTServerManager::setMaxInactiveTime(long usec)
{
	this->maxInactiveT = usec;
}

/* funcion que devuelve la lista de servidores */
const list<BTSimpleServer *>* BTServerManager::getServers(void)
{
	return &this->serversList;
	
}

/* funcion que devuelve una lista con todas las conexiones
* que tiene el server manager.
* NOTE: NO ELIMINAR NINGUNA CONEXION (usar removeConnection)
*/
const list<BTConnection *> *BTServerManager::getConnections(void)
{
	return &this->conList;
}

/*! Funcion que crea un servidor en base a un uuid, un puerto
* y un tamaño de cola. Esta funcion automaticamente establece
* el servicio con el SDP y lo saca una vez que ya no haya mas
* posibles conexiones.
* REQUIRES:
* 	uuid != NULL (sizeof(uuid) = 4 == 128 bits)
*	port > 0
*	qSize > 0
* RETURNS:
* 	errCode (< 0 on error | 0 = No error)
*/
int BTServerManager::createRfcommServer(uint32_t *uuid, int port, int qSize)
{
	BTSimpleServer *ss = NULL;
	BTSDPSessionData *sdpS = NULL;
	
	if (!uuid || port < 0 || qSize < 0) {
		debugp("error al crear el rfcommServer NULL\n");
		return -1;
	}
	/* intentamos crear el sdp session */
	sdpS = new BTSDPSessionData(uuid, (uint8_t) port);
	if (!sdpS) {
		debugp("no se pudo crear la SDP Session\n");
		return -1;
	}
	/* se pudo crear correctamente ==> creamos el server */
	ss = new BTSimpleServer(this->btDD->getMac(), qSize, port);
	if (!ss) {
		debugp("Error creando el server\n");
		/* eliminamos los datos */
		delete sdpS;
		return -1;
	}
	/* hacemos esto de forma aotmica */
	if (pthread_mutex_lock(&this->mutex) != 0) {
		perror("tomando el mutex\n");	
	}
	/*! deberiamos mostrar ahora el servicio */
	if (this->btDD->addSDPSession(sdpS) < 0){
		debugp("Error al intentar publicar el servicio\n");
		/*FIXME: que hacemos? borramos todo y devolvemos error? */
		delete sdpS;
		delete ss;
		if (pthread_mutex_unlock(&this->mutex) != 0) {
			perror("error al liberar el mutex\n");
		}
		return -2;
	}
	/* pudimos crear todo bien => agregamos a las listas */
	this->serversList.push_front(ss);
	this->sdpList.push_front(sdpS);
	
	/* ahora agregamos al fdSet el fd del server */
	addFdToSet(ss->getSocket(), BTSM_POLL_FLAGS);
	
	/* desbloqueamos el mutex */
	if (pthread_mutex_unlock(&this->mutex) != 0) {
		perror("error al liberar el mutex\n");
	}
	
	return 0;
}
	

/* funcion que devuelve el btdongledevice */
BTDongleDevice *BTServerManager::getBTDongleDevice(void)
{
	return this->btDD;
}

/* funcion que elimina un server, su service SDP
* RETURNS:
* 	true 	if no error
* 	false 	otherwise
* NOTE: las conexiones siguen abiertas hasta que sean 
*	cerradas o eliminadas por medio de removeConnection()
*/
bool BTServerManager::removeServer(BTSimpleServer *btSS)
{
	list<BTSimpleServer *>::iterator it;
	list<BTSDPSessionData *>::iterator itSdp;
	int i = 0, j = 0;
	
	
	/* hacemos atomica la operacion */
	if (pthread_mutex_lock(&this->mutex) != 0) {
		perror("tomando el mutex\n");	
	}
	
	if (this->sdpList.size() != this->serversList.size()) {
		debugp("Distinto tamaño de listas, imposible!\n");
		ASSERT(false);
		/* desbloqueamos el mutex */
		if (pthread_mutex_unlock(&this->mutex) != 0) {
			perror("error al liberar el mutex\n");
		}
		return false;
	}
	
	/* buscamos el elemento y vemos en que posicion esta, para despues
	 * poder borrar el SDP correspondiente */
	for (it = this->serversList.begin(); it != this->serversList.end(); 
		++it){
		if ((*it) == btSS)
			break;
		i++;
	}
	/* en la posicion i tenemos el sdp correspondiente */
	for (itSdp = this->sdpList.begin(); itSdp != this->sdpList.end(); 
		++itSdp) {
		if (j == i) {
			this->sdpList.remove(*itSdp);
			break;
		}
		j++;
	}
	
	if (j != i) {
		debugp("Error, no hay sdp correspondiente... :S\n");
		ASSERT(j == i);
		/* desbloqueamos el mutex */
		if (pthread_mutex_unlock(&this->mutex) != 0) {
			perror("error al liberar el mutex\n");
		}
		return false;
	}
	
	/* borramos el fd del fdPollSet */
	removeFdFromSet(btSS->getSocket());
	/* borramos ahora el correspondiente server */
	this->serversList.remove(btSS);
	delete btSS;
	
	/* desbloqueamos el mutex */
	if (pthread_mutex_unlock(&this->mutex) != 0) {
		perror("error al liberar el mutex\n");
	}
	
	return true;
}

/* Funcion que va a eliminar una conexion (cerrandola tambien)
* REQUIRES:
* 	con != NULL
* 	con € conList
* RETURNS:
* 	true 		on success
* 	false		otherwise
*/
bool BTServerManager::removeConnection(BTConnection *con)
{
	list<BTConnection *>::iterator it;
	bool result = false;
	
	if (!con) {
		debugp("Nos estan dando una con Null para borrar\n");
		return result;
	}
	
	/* hacemos atomica la operacion */
	if (pthread_mutex_lock(&this->mutex) != 0) {
		perror("tomando el mutex\n");	
	}
	
	/* lo buscamos */
	for (it = this->conList.begin(); it != this->conList.end(); ++it) {
		if (!(*it)) {
			debugp("conexion nula..\n");
			ASSERT(false);
			this->conList.remove(*it);
			continue;
		}
		if (con == (*it)) {
			this->conList.remove(con);
			result = true;
			/* eliminamos del fdset */
			if (!removeFdFromSet(con->getSocket()))
				debugp("No teniamos el socket en el fdSet\n");
			delete con;
		}
	}
	/* desbloqueamos el mutex */
	if (pthread_mutex_unlock(&this->mutex) != 0) {
		perror("error al liberar el mutex\n");
	}
	
	return result;
}

/*! Funcion bloqueante, que se encarga de esperar nuevos
* eventos (nuevas conexiones, nuevos datos recibidos, 
* desconecciones, o lo que sea). devolviendo la conexion
* involucrada en el evento y pasando el tipo de evento como
* parametro (en caso de que se reciba una nueva conexion se
* devuelve la misma y eventType = 0)
* REQUIRES;
* 	eventType != NULL
* RETURNS:
* 	if error => eventType = errCode (< 0) && conn == NULL
* 	else
* 	conn != NULL && evenType >= 0
* NOTE: esto es practicamente un select (debemos llamarlo
*	 todo el tiempo.
*/
BTConnection *BTServerManager::getConnectionEvent(int &eventType)
{
	int pollResult = 0;
	BTConnection *result = NULL;
	BTSimpleServer *ss = NULL;
	int i = 0;
	
	
	/*! primero que todo vamos a verificar si en alguna llamada anterior
	 * a poll quedo alguna coneccion por atender... si es asi, vamos a 
	 * devolverla antes de hacer un nuevo poll 
	 */
	for (i = 0; i < this->fdSetSize; i++){
		if (this->fdSet[i].revents != 0) {
			
			break;
		}
	}
	
	/* verificamos si hay alguna */
	if (i < this->fdSetSize) {
		/* tenemos la conexion iesima */
		pollResult = i;
	} else {
		/* hacemos poll */
		pollResult = poll(this->fdSet, this->fdSetSize, -1);
		if (pollResult <= 0) {
			perror("error en poll\n");
			eventType = pollResult;
			return result;
		}
		/* si no hubo error seteamos en pollResult el valor de la 
		 * i-esima conexion */
		for (i = 0; i < this->fdSetSize; i++)
			if (this->fdSet[i].revents != 0)
				break;
		pollResult = i;
	}
	
	/* verificamos ahora si es un server para aceptar la conexion */
	ss = getSSFromFd(this->fdSet[pollResult].fd);
	if (ss) {
		int serverResult = 0;
		
		/* ES un server, debemos aceptar la conexion y luego devolverla */
		
		/* Debemos verificar que no haya poll no haya retornado error */
		if((this->fdSet[pollResult].revents & (POLLPRI | POLLIN)) != 0){
			eventType = 0; /* indicamos que es una nueva conn */
			result = ss->acceptConn(&serverResult);
			if (serverResult <= 0 || !result)
				/* error al aceptar la conexion... */
				eventType = serverResult;
			else {
				/* aceptamos la conexion bien, la agregamos
				 * a la lista de conexiones y agregamos su
				 * socket al set */
				addFdToSet(result->getSocket(), BTSM_POLL_FLAGS);
				this->conList.push_front(result);
			}
			
			/* liberamos los revents */
			this->fdSet[pollResult].revents = 0;
			
			return result;
		} else {
			debugp("error con poll y los servers al aceptar una "
			"conexion\n");
			/* deberiamos destruir el server... */
			eventType = this->fdSet[pollResult].revents;
			this->fdSet[pollResult].revents = 0;
			removeServer(ss);
			
			return NULL;
		}
	}
	/*! no fue un server el que detecto poll => es una conexion */
	/* obtenemos la conexion desde el fd */
	result = getConFromFd(this->fdSet[pollResult].fd);
	if (!result) {
		debugp("no se encontro la conexion desde el fd..\n");
		/*! IMPOSIBLE, estamos manejando conexiones de otro lado? */
		ASSERT(false);
		return result;
	}
	
	eventType = (short) this->fdSet[pollResult].revents;
	this->fdSet[pollResult].revents = 0;
	
	return result;
	
}

/* Funcion que se encarga de verificar todas las conexiones
* y sus ultimas actividades (recepcion/envio), si no se
* detecta actividad dentro de los "maxInactivT" entonces
* automaticamente se cierra la conexion.
* NOTE: Usa un thread aparte => no es bloqueante
*/
void BTServerManager::startCheckConnTimes(void)
{
	this->start();
}

/* deja de verificar la inactividade de las conexiones
* (para el thread aparte)
*/
void BTServerManager::stopCheckConnTimes(void)
{
	this->stop();
}

/* aca corre el thread verdadero para aceptar conexiones 
* NOTE: no llamar a esta funcion
*/
void BTServerManager::run(void)
{	
	list<BTConnection *>::iterator it;
	long deltaTime = 0, rt = 0, st = 0;
	
	while(this->isRunning()) {
		/* conseguimos las conexiones */
		if (pthread_mutex_lock(&this->mutex) != 0) {
			perror("tomando el mutex\n");	
		}
		if (pthread_mutex_unlock(&this->mutex) != 0) {
			perror("error al liberar el mutex\n");
		}
		if (this->conList.size() == 0){ 
			/* vamos a esperar 5 veces el tiempo de inactividad 
			 * si no hay ninguna conexion... */
			usleep(this->maxInactiveT * 5);
			continue;
		}
		if (pthread_mutex_lock(&this->mutex) != 0) {
			perror("tomando el mutex\n");	
		}
		/* iteramos sobre las conexiones para verificar los tiempos 
		 * de actividades */
		for (it = this->conList.begin(); it != this->conList.end();
			++it) {
			if (!(*it)) {
				debugp("Conn null\n");
				ASSERT(false);
				this->conList.remove(*it);
				continue;
			}
			rt = (*it)->getLastRecvTime();
			st = (*it)->getLastSendTime();
			deltaTime = min(rt, st);

			if (deltaTime > this->maxInactiveT) {
				/* cerramos la conexion y esperamos que este
				 * evento salte por medio de getConnEvent
				 * para hacer una eliminacion sincronizada
				 */
				(*it)->closeConnection();
			}
		}
		if (pthread_mutex_unlock(&this->mutex) != 0) {
			perror("error al liberar el mutex\n");
		}
		
		/* dormimos durante el tiempo seteado */
		usleep(this->maxInactiveT);
	}
}


/* Destructor: 
* NOTE: Destruye todas las conexiones y los servidores
*/
BTServerManager::~BTServerManager(void)
{
	list<BTSimpleServer *>::iterator it;
	list<BTConnection *>::iterator cit;
	
	/* liberamos todos los servers */
	/* frenamos todo */
	this->stop();
	
	for(it = this->serversList.begin();it != this->serversList.end(); ++it){
		removeServer(*it);
	}
	
	for(cit = this->conList.begin();cit != this->conList.end(); ++cit){
		removeConnection(*cit);
	}
	
	pthread_mutex_destroy(&this->mutex);
}
