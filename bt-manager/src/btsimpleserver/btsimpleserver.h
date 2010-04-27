/*! Es el server mismo (de bajo nivel) que hace el bind / listen en un puerto 
 *  determinado con una direccion determinada.
 *  Tambien se encarga de mostrar/esconder el servicio en el server SDP.
 * NOTE: solo estamos usando RFCOMM_SERVERS
 */  
#ifndef BTSIMPLESERVER_H
#define BTSIMPLESERVER_H

#include <iostream>
#include <string>
#include <list>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
/* libs propias */
#include "../btconnection/btconnection.h"
#include "../btservermanager/btservermanager.h"
#include "../btsdpsessiondata/btsdpsessiondata.h"



using namespace::std;
class BTConnection;

class BTSimpleServer {
	
	public:
		/* constructor:
		 * REQUIRES;
		 * 	dongleMac != NULL
		 *	qSize	> 0	(queueSize)
		 * 	port € [1,30]
		 * 	sdpS (puede ser NULL)
		 */
		BTSimpleServer(const bdaddr_t *dongleMac, int qSize, int port, 
				BTSDPSessionData *sdpS);
		
		/* Funcion que devuelve el socket */
		int getSocket(void);
		
		/* funcion que devuelve el tamaño de la cola */
		int getQueueSize(void);
		
		/* funcion que devuelve el puerto */
		int getPort(void);
		
		/* funcion que devuelve la cantidad de conexiones relacionadas
		 * con este server. 
		 */
		int getNumConnection(void);
		
		/* funcion que devuelve la lista de conexiones */
		const list<BTConnection *>* getConnections(void);
		
		/* funcion que devuelve la mac del dongle por el cual corre */
		const bdaddr_t *getDongleMac(void);
		
		/* Funcion que ejecuta el bind
		 * RETURNS:
		 * 	errCode (man bind -< 0 == error) 
		 */
		int bindSocket(void);
		
		/* funcion que ejecuta el Listen NO BLOQUEANTE, devolviendo
		 * el codigo de error. Luego de hacer esta llamada deberia
		 * usarse el getSocket para meterlo en un select y luego
		 * poder extraer los usuarios 
		 * RETURNS:
		 * 	errCode (listen())
		 */
		int startListen(void);
		
		/* funcion que acepta una conexion determinada.
		 * NOTE: No es bloqueante.
		 * REQUIRES:
		 * 	result != NULL (se le pasa el error de codigo | socket)
		 * RETURNS:
		 * 	newConn == NULL	on error (si no se pudo obtener)
		 * 	newConn != NULL si no hubo error..
		 */
		BTConnection *acceptConn(int *result);
		
		/* Funcion que elimina una conexion de la lista 
		 * REQUIRES:
		 * 	conn != NULL
		 * 	conn € connList
		 */
		void removeConn(const BTConnection *conn);
		
		/* funcion que devuelve la sdpSession actual 
		 * RETURNS:
		 * 	NULL 		if no sdpSession
		 *	sdpS != NULL	otherwise
		 */
		BTSDPSessionData *getSDPSession(void){return this->sdpSession;};
				
		/* Destructor:
		 * Elimina todas las conexiones y las cierra una por una 
		 */
		~BTSimpleServer(void);
		
	private:
		/* atributos */
		
		/* Mac del dongle */
		bdaddr_t localMac;
		/* Tamaño de la cola (de posibles conexiones simultaneas */
		int queueSize;
		/* lista de conexiones activas */
		list<BTConnection *> connList;
		/* socket */
		int sock;
		/* puerto en el que va a tener que escuchar */
		int port;
		/* sesion en el servidor SDP */
		BTSDPSessionData *sdpSession;
};



#endif
