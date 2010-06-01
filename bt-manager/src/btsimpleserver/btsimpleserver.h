/*! Es el server mismo (de bajo nivel) que hace el bind / listen en un puerto 
 *  determinado con una direccion determinada.
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
#include <bluetooth/rfcomm.h>
/* libs propias */
#include "btconnection.h"



using namespace::std;

class BTSimpleServer {
	
	public:
		/* constructor:
		 * REQUIRES;
		 * 	dongleMac != NULL
		 *	qSize	> 0	(queueSize)
		 * 	port € [1,30]
		 */
		BTSimpleServer(const bdaddr_t *dongleMac, int qSize, int port);
		
		/* Funcion que devuelve el socket */
		int getSocket(void);
		
		/* funcion que devuelve el tamaño de la cola */
		int getQueueSize(void);
		
		/* funcion que devuelve el puerto */
		int getPort(void);
		
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
		
		/* Destructor:
		 * Elimina todas las conexiones y las cierra una por una 
		 */
		~BTSimpleServer(void);
		
		#ifdef __DEBUG
		/*! esto es para poder crear servers desde un socket determinado
		 * por lo que se debera hacer el bind, y el listen de otro lado
		 * solo va a funcionar el acceptConn()...
		 * REQIURES:
		 * 	fd >= 0
		 * 	fd (bindeado & en escucha (listen))
		 */
		BTSimpleServer(int fd);
		#endif
		
	private:
		/* atributos */
		
		/* Mac del dongle */
		bdaddr_t localMac;
		/* Tamaño de la cola (de posibles conexiones simultaneas */
		int queueSize;
		/* socket */
		int sock;
		/* puerto en el que va a tener que escuchar */
		int port;
		
};



#endif
