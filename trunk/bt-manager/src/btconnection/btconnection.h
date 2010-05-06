/*! Es una conexion bluetooth (muy similar a la tcp).
* Deberia poder:
* Enviar Datos.
* Recibir Datos.
* Tener un "delta time" que indique cuando fue la ultima vez que recibio datos 
* (para verificar la frecuencia con la que se mandan datos para poder determinar
* si se debe cerrar o no la conexion).
*/  
#ifndef BTCONNECTION_H
#define BTCONNECTION_H

#include <iostream>
#include <string>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
/* libs propias */
#include "consts.h"
#include "debug.h"

/*! Definimos el tamaño maximo del buffer auxiliar para recibir datos... */
#define BTC_RCV_AUXBUFF		1024	/* bytes */

/*! FIXME: deberiamos definir un estado de la conexion, por ejemplo, conectado
 *	   desconectado, error, etc... por ahora lo dejamos asi nomas 
 */
enum {
	BTCON_NO_ERROR,
	BTCON_SOCK_ERROR,
	BTCON_MAC_ERROR,
	BTCON_SERVER_ERROR,
	BTCON_SEND_ERROR,
	BTCON_RECV_ERROR,
	BTCON_CLOSED
};

using namespace::std;

class BTConnection {
	
	public:
		/* Constructor: 
		 * REQUIRES:
		 *	mac != NULL
		 * NOTE: Se hace una copia de mac (debe ser liberada quien la
		 *	 llamo.
		 */
		BTConnection(bdaddr_t *mac, int sock);
		
		/*! Las funciones de envio y recepcion NO SON BLOQUEANTES */
		
		/* Funcion que envia datos por la conexion
		 * REQUIRES:
		 * 	data &
		 * RETURNS:
		 * 	bytesSended > 0		if no error
		 * 	<= 0			on error
		 */
		int sendData(string &data);
		
		/* Funcion que recibe datos del socket, almacenandolos en el
		 * buffer de recepcion (tener en cuenta que se hace un append
		 * con los nuevos datos en el buffer).
		 * RETURNS:
		 * 	bytesRecv > 0		if no error
		 * 	<= 0			on error
		 */
		int recvData(void);
		
		/* funcion que limpia el buffer de envio */
		void clearSendBuffer(void);
		
		/* funcion que limpia el buffer de recepcion */
		void clearRecvBuffer(void);
		
		/* Funcion que devuelve la referencia del buffer de datos a
		 * enviar (para ser modificados a discrecion */
		string &getSendBuff(void);
		
		/* funcion que devuelve una referencia al buffer de datos a 
		 * recibir (para ser modificado a discrecion */
		string &getRcvBuff(void);
		
		/* Funcion que devuelve el socket */
		int getSocket(void);
		
		/* funcion que devuelve el estado de la conexion */
		int getStatus(void);
		
		/*!		FUNCIONES DE TIEMPO		*/
		/* funcion que devuelve el tiempo en el que se recibio el ultimo
		 * dato en usec (1.000.000). */
		long getLastRecvTime(void);
		
		/* funcion que devuelve el tiempo en el que se envio el ultimo
		* dato en usec (1.000.000). */
		long getLastSendTime(void);
		
		/* funcion que devuelve la mac destino de la conexion */
		const bdaddr_t* getMacDest(void);
		
		/* Funcion que cierra la conexion */
		void closeConnection(void);
		
		/* Destructor.
		 * NOTE; Cierra la conexion en caso de que este abierta 
		 * y libera los buffers.
		 */
		~BTConnection(void);
		
	private:
		/* atributos */
		
		/* buffer de recepcion de datos */
		string rBuff;
		/* buffer de envio de datos */
		string sBuff;
		/* tiempo en el que se recibio el ultimo dato en ms */
		struct timeval rcvT;
		/* tiempo en el que se envio el ultimo dato en ms */
		struct timeval sndT;
		/* estado actual de la conexion... */
		int status;
		/* socket */
		int sock;
		/* la mac destino */
		bdaddr_t mac;
		
};



#endif
