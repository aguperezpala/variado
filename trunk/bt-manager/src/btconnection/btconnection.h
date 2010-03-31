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
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
/* libs propias */
#include "../btsimpleserver/btsimpleserver.h"


using namespace::std;



class BTConnection {
	
	public:
		/* Constructor: 
		 * REQUIRES:
		 * 	server != NULL
		 *	mac != NULL
		 * NOTE: Se hace una copia de mac (debe ser liberada quien la
		 *	 llamo.
		 */
		BTConnection(BTSimpleServer *server, bdaddr_t *mac);
		
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
		
		/* funcion que devuelve el tiempo en el que se recibio el ultimo
		 * dato. */
		time_t *getLastRecvTime(void);
		
		/* funcion que devuelve el tiempo en el que se envio el ultimo
		* dato. */
		time_t *getLastSendTime(void);
		
		/* funcion que devuelve a que server pertenece */
		BTSimpleServer *getServer(void);
		
		/* funcion que devuelve la mac destino de la conexion */
		const bdaddr_t* getMacDest(void);
		
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
		/* tiempo en el que se recibio el ultimo dato */
		time_t lastRecvT;
		/* tiempo en el que se envio el ultimo dato */
		time_t lastSendT;
		/* estado actual de la conexion... */
		int status;
		/* socket */
		int sock;
		/* server al que pertenece */
		BTSimpleServer *server;
		/* la mac destino */
		bdaddr_t mac;
		
};



#endif
