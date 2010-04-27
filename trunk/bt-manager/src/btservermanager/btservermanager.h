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


#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <iostream>
#include <string>
#include <list>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>
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


/*! definimos los flags para el poll */
#define BTSM_POLL_FLAGS		POLLIN | POLLPRI | POLLOUT | POLLRDHUP | POLLERR \
| POLLHUP | POLLNVAL

/*! definimos que cantidad de servidores podemos tener */
#define BTSM_MAX_SERVERS	5

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
		 * RETURNS:
		 * 	true 	if no error
		 * 	false 	otherwise
		 */
		bool removeServer(BTSimpleServer *btSS);
		
		/*! Funcion bloqueante, que se encarga de esperar nuevos
		 * eventos (nuevas conexiones, nuevos datos recibidos, 
		 * desconecciones, o lo que sea). devolviendo la conexion
		 * involucrada en el evento y pasando el tipo de evento como
		 * parametro (en caso de que se reciba una nueva conexion se
		 * devuelve la misma y eventType = 0)
		 * REQUIRES;
		 * 	eventType != NULL
		 * RETURNS:
		 * 	if error => eventType = errCode (< 0) && conn == NULL
		 * 	else
		 * 	conn != NULL && evenType >= 0
		 * NOTE: esto es practicamente un select (debemos llamarlo
		 *	 todo el tiempo.
		 */
		const BTConnection *getConnectionEvent(int &eventType);
		
		/* Funcion que se encarga de verificar todas las conexiones
		 * y sus ultimas actividades (recepcion/envio), si no se
		 * detecta actividad dentro de los "maxInactivT" entonces
		 * automaticamente se cierra la conexion.
		 * NOTE: Usa un thread aparte => no es bloqueante
		 */
		void startCheckConnTimes(void);
		
		/* deja de verificar la inactividade de las conexiones
		 * (para el thread aparte)
		 */
		void stopCheckConnTimes(void);
		
		/* aca corre el thread verdadero para aceptar conexiones 
		 * NOTE: no llamar a esta funcion
		 */
		void run(void);
		
		
		/* Destructor: 
		 * NOTE: Destruye todas las conexiones y los servidores
		*/
		~BTServerManager(void);
		
	private:
		
		/* Funcion que agrga un fd al ultimo lugar del fdSet.
		 * RETURNS:
		 * 	false	on error || fdSet.size() == MAX_CON_PER_DONGLE
		 * 	true	if success
		 */
		bool addFdToSet(int fd);
		
		/* Funcion que saca un fd del set y lo re-ordena
		 * RETURNS:
		 * 	true 	if success
		 *	false	on error
		 */
		bool removeFdFromSet(int fd);
		
		/* funcion que devuelve una conexion desde un fd
		 * RETURNS:
		 * 	NULL 	on error || if not exists
		 * 	BTConnection != NULL	on success
		 */
		const BTConnection *getConFromFd(int fd);
		
		/* funcion que devuelve un server de un fd determinado:
		 * RETURNS;
		 * 	NULL 		if no Server with that fd
		 * 	BTSS != NULL	otherwise
		 */
		BTSimpleServer *getSSFromFd(int fd);
		
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
		struct pollfd fdSet[MAX_CON_PER_DONGLE + BTSM_MAX_SERVERS];
		int fdSetSize;
	
};



#endif
