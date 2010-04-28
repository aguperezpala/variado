#include "btsimpleserver.h"


/* constructor:
* REQUIRES;
* 	dongleMac != NULL
*	qSize	> 0	(queueSize)
*/
BTSimpleServer::BTSimpleServer(const bdaddr_t *dongleMac, int qSize, int port)
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
	
	this->sock = 0;
	this->port = port;
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
	struct sockaddr_rc localAddr;
	
	memset(&localAddr, 0, sizeof(struct sockaddr_rc));
	/* creamos el socket primero */
	this->sock = socket(AF_BLUETOOTH, SOCK_STREAM | SOCK_NONBLOCK,
			     BTPROTO_RFCOMM);
	if (this->sock < 0) {
		perror("Error al crear el socket\n");
		return this->sock;
	}
	
	/* intentamos hacer el bind ahora */
	localAddr.rc_family = AF_BLUETOOTH;
	bacpy(&localAddr.rc_bdaddr, &this->localMac);
	localAddr.rc_channel = (uint8_t) this->port;
	
	if (bind(this->sock, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0){
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
	socklen_t opt = sizeof(remoteAddr);
	
	
	if (!result){
		debugp("Nos dieron un parametro nulo\n");
		return NULL;
	}
	
	*result = accept(this->sock, (struct sockaddr*) &remoteAddr,
			 &opt);
	if (*result <= 0) {
		perror("Error al aceptar una conexion\n");
		return NULL;
	}
	
	/* si estamos aca es porque realmente tenemos una nueva conexion 
	 * creamos una nueva BTConnection entonces */
	conn = new BTConnection(&remoteAddr.rc_bdaddr, *result);
	if (!conn) {
		debugp("Error al intentar crear una nueva conexion\n");
		return NULL;
	}

	return conn;
}


/* Destructor:
* Elimina todas las conexiones y las cierra una por una 
*/
BTSimpleServer::~BTSimpleServer(void)
{
	close(this->sock);
}