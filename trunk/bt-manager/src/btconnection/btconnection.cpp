#include "btconnection.h"

/* Constructor: 
* REQUIRES:
* 	server != NULL
*/
BTConnection::BTConnection(BTSimpleServer *server);

/*! Las funciones de envio y recepcion NO SON BLOQUEANTES */

/* Funcion que envia datos por la conexion
* REQUIRES:
* 	data &
* RETURNS:
* 	bytesSended > 0		if no error
* 	<= 0			on error
*/
int BTConnection::sendData(string &data);

/* Funcion que recibe datos del socket, almacenandolos en el
* buffer de recepcion (tener en cuenta que se hace un append
* con los nuevos datos en el buffer).
* RETURNS:
* 	bytesRecv > 0		if no error
* 	<= 0			on error
*/
int BTConnection::recvData(void);

/* funcion que limpia el buffer de envio */
void BTConnection::clearSendBuffer(void);

/* funcion que limpia el buffer de recepcion */
void BTConnection::clearRecvBuffer(void);

/* Funcion que devuelve la referencia del buffer de datos a
* enviar (para ser modificados a discrecion */
string &BTConnection::getSendBuff(void);

/* funcion que devuelve una referencia al buffer de datos a 
* recibir (para ser modificado a discrecion */
string &BTConnection::getRcvBuff(void);

/* Funcion que devuelve el socket */
int BTConnection::getSocket(void);

/* funcion que devuelve el estado de la conexion */
int BTConnection::getStatus(void);

/* funcion que devuelve el tiempo en el que se recibio el ultimo
* dato. */
time_t *BTConnection::getLastRecvTime(void);

/* funcion que devuelve el tiempo en el que se envio el ultimo
* dato. */
time_t *BTConnection::getLastSendTime(void);

/* funcion que devuelve a que server pertenece */
BTSimpleServer *BTConnection::getServer(void);

/* funcion que devuelve la mac destino de la conexion */
const bdaddr_t* BTConnection::getMacDest(void);


/* Destructor.
* NOTE; Cierra la conexion en caso de que este abierta 
* y libera los buffers.
*/
BTConnection::~BTConnection(void);

