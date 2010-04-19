/*! Se encarga de:
* Generar N (7) escuchadores en un rango determinado (pasado por parametro)
*  de puertos (en un principio conexiones RFCOMM) y conectarse con el SDP Server
*   para hacer publicos los servicios (deberia tomar un rango de N UUIDS).
* Manejar los errores de conexiones.
* Determinar cuanto tiempo puede estar activa cada conexion (pasada por parametro).
* Deberia poder devolverse las conexiones establecidas para poder ser manejadas
*  desde otro nivel.
 */  
#ifndef BTSERVERMANAGER_H
#define BTSERVERMANAGER_H

#include <iostream>
#include <string>
#include <list>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
/* libs propias */
#include "../consts.h"
#include "../debug.h"
#include "../btconnection/btconnection.h"
#include "../btsdpsessiondata/btsdpsessiondata.h"
#include "../btdongledevice/btdongledevice.h"
#include "../btsimpleserver/btsimpleserver.h"
#include "../simplethread/simplethread.h"


using namespace::std;

class BTServerManager : public SimpleThread {
	
	public:
		/* Constructor 
		 * REQUIRES:
		 * 	btDD != NULL
		 */
		BTServerManager(BTDongleDevice *btDD);
		
		/* Funcion que setea el tiempo maximo de inactividad para las
		 * conexiones.
		 * FIXME: deberiamos hacer un TAD que se puedan agregar
		 * interrupciones asociadas a funciones que ejecutar ante
		 * el transcurso de un tiempo determinado...
		 * Por el momento vamos a chequear periodicamente todas las
		 * conexiones y vamos a verificar si tenemos 
		 */
		void setMaxInactiveTime(long usec);
		
		/* funcion que devuelve la lista de servidores */
		const list<BTSimpleServer *>* getServers(void);
		
		/* funcion que devuelve una lista con todas las conexiones
		 * que tiene el server manager.
		 * RETURNS:
		 * 	list != NULL		if no error
		 *	NULL			on error
		 * NOTE: Se genera memoria para la lista, pero no para las
		 * conexiones
		 */
		list<const BTConnection *> *getConnections(void);
		
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
		 *	{ -1 = ENOMEM, -2 = SDP_ERROR, -3 = UNKNOWN_ERROR }
		 */
		int createRfcommServer(int *uuid, int port, int qSize);
		
		/* funcion que devuelve el btdongledevice */
		BTDongleDevice *getBTDongleDevice(void);
		
		/* funcion que elimina un server, su service SDP, y todas sus
		 * conexiones. NOTE: notar que las conexiones no deben ser
		 * eliminadas desde otros lados, esta clase se encarga de
		 * manejar todo esto. 
		 */
		void removeServer(BTSimpleServer *btSS);
		
		
		/* Funcion que produce que la clase deje de aceptar las 
		 * conexiones */
		void stopAcceptConnections(void);
		
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
		const BTConnection *getConnectionEvent(int &eventType);
		
		/* funcion que se encarga de empezar a aceptar conexiones de los
		 * servidores, es transparente para el usuario, lo que hace
		 * es en un thread aparte, hace un select de los fd de los 
		 * servers y espera que haya conexiones entrantes, y las va
		 * agregando al fd_set de las conexiones (que estas son 
		 * manejadas por getConnectionEvent())
		 */
		void startAcceptConnections(void);
		/* aca corre el thread verdadero para aceptar conexiones 
		 * NOTE: no llamar a esta funcion
		 */
		void run(void);
		
		
		/* Destructor: 
		 * NOTE: Destruye todas las conexiones y los servidores
		*/
		~BTServerManager(void);
		
	private:
		/* atributos */
		
		/* Dongle sobre el cual se va a realizar toda la estructura
		 * de servidores y conexiones */
		BTDongleDevice *btDD;
		/* tiempo maximo en usec que una conexion puede permanecer
		 * inactiva */
		long maxInactiveT;
		/* lista de servidores, NOTE: la lista de conexiones se puede
		 * obtener pidiendole la lista de conexiones a cada server */
		list<BTSimpleServer *> serversList;
		/* lista que contiene los SDPSessionData en un mapeo uno a uno
		 * con la lista de serversList */
		list<BTSDPSessionData *> sdpList;
		/* mutex usado para manejar el paralelismo */
		pthread_mutex_t mutex;
		/* el set de fd */
		fd_set fdSet;
	
};



#endif
