#include "btservermanager.h"

/* Constructor 
* REQUIRES:
* 	btDD != NULL
*/
BTServerManager::BTServerManager(BTDongleDevice *btDD)
{
	if (!btDD) {
		debugp("Error al crear el BTServerManager, btDD Null\n");
		ASSERT(btDD != NULL);
	}
	this->btDD = btDD;
	this->serversList.clear();
	this->sdpList.clear();
	FD_ZERO(&this->fdSet);
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
* RETURNS:
* 	list != NULL		if no error
*	NULL			on error
* NOTE: Se genera memoria para la lista, pero no para las
* conexiones
*/
list<const BTConnection *> *BTServerManager::getConnections(void)
{
	list<const BTConnection *> *result = NULL;
	const list<const BTConnection *> *connList = NULL;
	const list<const BTConnection *>::iterator it2;
	list<BTSimpleServer *>::iterator it;
	
	result = new list<const BTConnection *>();
	if (!result)
		return result;
	
	/* recorremos la lista de servers */
	for (it = this->serversList.begin(); it != this->serversList.end(); 
		++it){
		if (!(*it)) {
			serversList.remove(*it);
			continue;
		}
		/* ahora para cada server obtenemos la lista de conexiones */
		connList = (*it)->getConnections();
		if (!connList)
			continue;
		/* iteramos sobre la lista de conexiones de cada server y vamos
		 * agregando las conexiones a la lista a devolver */
		for(it2 = connList->begin(); it2 != connList.end(); ++it2) {
			if (!(*it2))
				continue;
			result->push_front((*it2));
		}
	
	}
	
	return result;
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
int BTServerManager::createRfcommServer(int *uuid, int port, int qSize)
{
	BTSimpleServer *ss = NULL;
	BTSDPSessionData *sdpS = NULL;
	
	if (!uuid || por < 0 || qsize < 0) {
		debupg("error al crear el rfcommServer NULL\n");
		return -1;
	}
	/* intentamos crear el sdp session */
	sdpS = new BTSDPSessionData(uuid, (uint8_t) port);
	if (!sdpS) {
		debug("no se pudo crear la SDP Session\n");
		return -1;
	}
	/* se pudo crear correctamente ==> creamos el server */
	ss = new BTSimpleServer(this->btDD->getMac(), qSize, port, sdpS);
	if (!ss) {
		debupg("Error creando el server\n");
		/* eliminamos los datos */
		delete sdpS;
		return -1;
	}
	/*! deberiamos mostrar ahora el servicio */
	if (this->btDD->addSDPSession(sdpS) < 0){
		debugp("Error al intentar publicar el servicio\n");
		/*FIXME: que hacemos? borramos todo y devolvemos error? */
		delete sdpS;
		delete ss;
		return -2;
	}
	/* pudimos crear todo bien => agregamos a las listas */
	this->serversList.push_front(ss);
	/* FIXME: probablemente esta lista no haga falta, en el btDD tenemos una
	 * identica */
	this->sdpList.push_front(sdpS);
	
	/* ahora agregamos al fdSet el fd del server */
	FD_SET(ss->getSocket(), &this->fdSet);
	
	return 0;
}
	

/* funcion que devuelve el btdongledevice */
BTDongleDevice *BTServerManager::getBTDongleDevice(void);

/* funcion que elimina un server, su service SDP, y todas sus
* conexiones. NOTE: notar que las conexiones no deben ser
* eliminadas desde otros lados, esta clase se encarga de
* manejar todo esto. 
*/
void BTServerManager::removeServer(BTSimpleServer *btSS);


/* Funcion que produce que la clase deje de aceptar las 
* conexiones */
void BTServerManager::stopAcceptConnections(void);

/*! Funcion bloqueante, que se encarga de esperar nuevos
* eventos (nuevas conexiones, nuevos datos recibidos, 
* desconecciones, o lo que sea). devolviendo la conexion
* involucrada en el evento y pasando el tipo de evento como
* parametro (notar que no se manejan los servers aca, solo
* las conexiones)
* REQUIRES;
* 	eventType != NULL
* RETURNS:
* 	if error => eventType = errCode && conn == NULL
* 	else
* 	conn != NULL && evenType > 0
* NOTE: esto es practicamente un select (debemos llamarlo
*	 todo el tiempo.
*/
const BTConnection *BTServerManager::getConnectionEvent(int &eventType);

/* funcion que se encarga de empezar a aceptar conexiones de los
* servidores, es transparente para el usuario, lo que hace
* es en un thread aparte, hace un select de los fd de los 
* servers y espera que haya conexiones entrantes, y las va
* agregando al fd_set de las conexiones (que estas son 
* manejadas por getConnectionEvent())
*/
void BTServerManager::startAcceptConnections(void);
/* aca corre el thread verdadero para aceptar conexiones 
* NOTE: no llamar a esta funcion
*/
void BTServerManager::run(void);


/* Destructor: 
* NOTE: Destruye todas las conexiones y los servidores
*/
BTServerManager::~BTServerManager(void)
