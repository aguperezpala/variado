#include "btconnection.h"

/* Constructor: 
* REQUIRES:
* 	mac != NULL
*/
BTConnection::BTConnection(bdaddr_t *mac, int sock)
{
	/* inicializamos */
	this->rBuff.clear();
	this->sBuff.clear();
	
	/* seteamos el tiempo en el que se creo la conexion */
	gettimeofday(&this->rcvT, NULL);
	gettimeofday(&this->sndT, NULL);
	
	this->status = BTCON_NO_ERROR;
	if (sock <= 0) {
		debugp("Nos dieron un socket invalido\n");
		this->status = BTCON_SOCK_ERROR;
	}
	this->sock = sock;
	
	if(!mac) {
		debugp("Mac nula\n");
		this->status = BTCON_MAC_ERROR;
	} else
		bacpy(&this->mac, mac);
}

/*! Las funciones de envio y recepcion NO SON BLOQUEANTES */

/* Funcion que envia datos por la conexion
* REQUIRES:
* 	data &
* RETURNS:
* 	bytesSended > 0		if no error
* 	<= 0			on error
*/
int BTConnection::sendData(string &data)
{
	ssize_t result = 0;
	size_t len = 0;
	
	/*!FIXME: debemos verificar si string data soporta cualquier caracter
	 * o si hay que utilizar otro tipo de datos, byte_buff o algo raro */
	
	/* primero que todo lo que hacemos es concatenar data al sBuff */
	this->sBuff += data;
	
	/* ahora intentamos mandar datos */
	len = this->sBuff.size();
	result = send(this->sock, this->sBuff.c_str(), len, MSG_DONTWAIT);
	
	/*! actualizamos el tiempo de envio */
	gettimeofday(&this->sndT, NULL);
	
	if (result < 0) {
		perror("Error al intentar enviar: ");
		this->status = BTCON_SEND_ERROR;
	} else {
		/* pudimos transmitir => debemos sacar result bytes del sBuff */
		this->sBuff.erase(0,result);
	}
	return result;
	
}

/* Funcion que recibe datos del socket, almacenandolos en el
* buffer de recepcion (tener en cuenta que se hace un append
* con los nuevos datos en el buffer).
* RETURNS:
* 	bytesRecv > 0		if no error
* 	<= 0			on error
*/
int BTConnection::recvData(void)
{
	ssize_t result = 0;
	char auxBuff[BTC_RCV_AUXBUFF];
	
	result = recv(this->sock, auxBuff, BTC_RCV_AUXBUFF, MSG_DONTWAIT);
	
	/*! actualizamos el tiempo lastRecvT */
	gettimeofday(&this->rcvT, NULL);
	
	if (result <= 0) {
		if (result == 0) {
			/* nos cerraron la conexion */
			this->status = BTCON_CLOSED;
		} else {
			/* hubo algun error */
			this->status = BTCON_RECV_ERROR;
		}
	} else {
		/* recibimos correctamente datos los guardamos en el buffer */
		this->rBuff += auxBuff;
	}
	return result;
}

/* funcion que limpia el buffer de envio */
void BTConnection::clearSendBuffer(void)
{
	this->sBuff.clear();
}

/* funcion que limpia el buffer de recepcion */
void BTConnection::clearRecvBuffer(void)
{
	this->rBuff.clear();
}

/* Funcion que devuelve la referencia del buffer de datos a
* enviar (para ser modificados a discrecion */
string &BTConnection::getSendBuff(void)
{
	return this->sBuff;
}

/* funcion que devuelve una referencia al buffer de datos a 
* recibir (para ser modificado a discrecion */
string &BTConnection::getRcvBuff(void)
{
	return this->rBuff;
}

/* Funcion que devuelve el socket */
int BTConnection::getSocket(void)
{
	return this->sock;
}

/* funcion que devuelve el estado de la conexion */
int BTConnection::getStatus(void)
{
	return this->sock;
}

/* funcion que devuelve el tiempo en el que se recibio el ultimo
* dato. */
long BTConnection::getLastRecvTime(void)
{
	long result = 0;
	struct timeval aux;
	
	gettimeofday(&aux, NULL);
	
	
	result = (long) ((aux.tv_sec - this->rcvT.tv_sec) * 1000000 + 
		(aux.tv_usec - this->rcvT.tv_usec));
	
	return result;
}

/* funcion que devuelve el tiempo en el que se envio el ultimo
* dato. */
long BTConnection::getLastSendTime(void)
{
	struct timeval aux;
	long result = 0;
	
	gettimeofday(&aux, NULL);
	
	
	result = (long) ((aux.tv_sec - this->sndT.tv_sec) * 1000000 + 
		(aux.tv_usec - this->sndT.tv_usec));
	
	return result;
}

/* funcion que devuelve la mac destino de la conexion */
const bdaddr_t* BTConnection::getMacDest(void)
{
	return &this->mac;
}
	

/* Funcion que cierra la conexion */
void BTConnection::closeConnection(void)
{
	close(this->sock);
}

/* Destructor.
* NOTE; Cierra la conexion en caso de que este abierta 
* y libera los buffers.
*/
BTConnection::~BTConnection(void)
{
	/* no borra nada pero llama a closeConnection() */
	closeConnection();
}

