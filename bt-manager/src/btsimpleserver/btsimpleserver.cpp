#include "btsimpleserver.h"


/* constructor:
* REQUIRES;
* 	dongleMac != NULL
*	qSize	> 0	(queueSize)
*/
BTSimpleServer::BTSimpleServer(const bdaddr_t *dongleMac, int qSize, int port,
				BTSDPSessionData *sdpS)
{
	if (!dongleMac)
		debugp("estamos recibiendo un dongleMac NULL\n");
	else
		bacpy(&this->localMac, dongleMac);
	if (qSize <= 0) {
		debugp("Estamos recibiendo un tamaño de cola invalido\n");
		/* por defecto metemos 1 */
		this->queueSize = 1;
	} else
		this->queueSize = qSize;
	
	this->connList.clear();
	this->sock = 0;
	this->port = port;
	/* ahora creamos el SDPSessionData que vamos a usar */
	
	this->sdpSession = sdpS;
	
}

/* Funcion que devuelve el socket */
int BTSimpleServer::getSocket(void)
{
	return this->sock;
}

/* funcion que devuelve el tamaño de la cola */
int BTSimpleServer::getQueueSize(void)
{
	return this->queueSize;
}

/* funcion que devuelve el puerto */
int BTSimpleServer::getPort(void)
{
	return this->port;
}

/* funcion que devuelve la cantidad de conexiones relacionadas
* con este server. 
*/
int BTSimpleServer::getNumConnection(void)
{
	return this->connList.size();
}

/* funcion que devuelve la lista de conexiones */
const list<BTConnection *>* BTSimpleServer::getConnections(void)
{
	return &this->connList;
}

/* funcion que devuelve la mac del dongle por el cual corre */
const bdaddr_t *BTSimpleServer::getDongleMac(void)
{
	return &this->localMac;
}

/* Funcion que ejecuta el bind
* RETURNS:
* 	errCode (man bind -< 0 == error) 
*/
int BTSimpleServer::bindSocket(void)
{
	struct sockaddr_rc localAddr = {0};
	
	/* creamos el socket primero */
	this->sock = socket(AF_BLUETOOTH, SOCK_STREAM | SOCK_NONBLOCK,
			     BTPROTO_RFCOMM);
	if (this->sock < 0) {
		perror("Error al crear el socket\n");
		return this->sock;
	}
	
	/* intentamos hacer el bind ahora */
	locAddr.rc_family = AF_BLUETOOTH;
	bacpy(&locAddr.rc_bdaddr, &this->localMac);
	locAddr.rc_channel = (uint8_t) this->port;
	
	if (bind(this->sock, (struct sockaddr *)&locAddr, sizeof(locAddr)) < 0){
		perror("Error al hacer el bind\n");
		return -1;
	}
	
	/* llegamos aca no hubo error */
	return 0;
}
	

/* funcion que ejecuta el Listen NO BLOQUEANTE, devolviendo
* el codigo de error. Luego de hacer esta llamada deberia
* usarse el getSocket para meterlo en un select y luego
* poder extraer los usuarios 
* RETURNS:
* 	errCode (listen())
*/
int BTSimpleServer::startListen(void)
{
	return listen(this->sock, this->queueSize);
}

/* funcion que acepta una conexion determinada.
* NOTE: No es bloqueante.
* REQUIRES:
* 	result != NULL (se le pasa el error de codigo | socket)
* RETURNS:
* 	newConn == NULL	on error (si no se pudo obtener)
* 	newConn != NULL si no hubo error..
*/
BTConnection *BTSimpleServer::acceptConn(int *result)
{
	struct sockaddr_rc remoteAddr = {0};
	BTConnection *conn = NULL;
	
	
	if (!result){
		debugp("Nos dieron un parametro nulo\n");
		return NULL;
	}
	
	*result = accept(this->sock, (struct sockaddr*) &remoteAddr,
			 sizeof(remoteAddr));
	if (*result <= 0) {
		perror("Error al aceptar una conexion\n");
		return NULL;
	}
	
	/* si estamos aca es porque realmente tenemos una nueva conexion 
	 * creamos una nueva BTConnection entonces */
	conn = new BTConnection(this, &remoteAddr.rc_bdaddr, *result);
	if (!conn) {
		debugp("Error al intentar crear una nueva conexion\n");
		return NULL;
	}
	/* agregamos la conexion a la lista de conexiones */
	this->connList.push_front(conn);
	
	return conn;
	
	
	
}

/* Funcion que elimina una conexion de la lista 
* REQUIRES:
* 	conn != NULL
* 	conn € connList
*/
void BTSimpleServer::removeConn(const BTConnection *conn)
{
	if (!conn)
		return;
	this->connList.remove(conn);
}

/* Destructor:
* Elimina todas las conexiones y las cierra una por una 
*/
BTSimpleServer::~BTSimpleServer(void)
{
	list<BTConnection *>::iterator it;
	
	for (it = this->connList.begin(); it != this->connList.end(); ++it) {
		if(!(*it)) {
			this->connList.remove(*it);
			continue;
		}
		(*it)->closeConnection();
		/* hacemos un detach del server */
		(*it)->setServer(NULL);
	}
	close(this->sock);
}