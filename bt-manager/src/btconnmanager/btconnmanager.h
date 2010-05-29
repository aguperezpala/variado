/*! 
 * Clase que se encarga de manejar los eventos que producen las conexiones
 * y los servers, en caso de que un server recibe una conexion, automaticamente
 * se agrega a la lista de conexiones.
*/  
#ifndef BTCONNMANAGER_H
#define BTCONNMANAGER_H

#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <assert.h>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <poll.h>
/* libs propias */
#include "btsimpleserver.h"
#include "btconnection.h"
#include "consts.h"
#include "debug.h"

using namespace::std;

/*! definimos las banderas para las conexiones / servers que se insertan */
#define BTCM_POLL_IN_FLAGS		POLLIN | POLLPRI | POLLNVAL
#define BTCM_POLL_OUT_FLAGS		POLLOUT


#define BTCM_SERVER_POLL_FLAGS		BTCM_POLL_IN_FLAGS

/*! definimos tipos de eventos que pueden devolverse */
typedef enum {
	BTCM_EV_NEW_CONN,	/* cuando se recibe una conexion */
	BTCM_EV_OUT,		/* cuando se puede enviar, y se envian datos */
	BTCM_EV_CLOSE_CONN,	/* conexion cerrada */
	BTCM_EV_RCV,		/* cuando se puede recibir */
	BTCM_EV_SERVER_ERR,	/* cuando un servidor produce un error */
	BTCM_EV_ACCEPT_ERR,	/* cuando hay un error al aceptar una conexion */
	BTCM_EV_ERR		/* algun error */
} eventType_t;

class BTConnManager{
	
	public:
		/*! El BTConnManager no genera memoria nunca, y si un server
		 * es eliminado o una conexion es liberada, debe ser extraida
		 * de aca.
		 */
		
		/* constructor */
		BTConnManager(void);
		
		/* funcion que permite agregar un server para chequear sus
		 * eventos.
		 * REQUIRES:
		 * 	server != NULL
		 */
		void insertServer(BTSimpleServer *server);
		
		/* Funcion que elimina un server de la lista para que
		 * deje de ser verificado
		 * REQUIRES:
		 * 	server != NULL
		 */
		void removeServer(BTSimpleServer *server);
		
		/* funcion que permite agregar una conexion para verificar
		 * sus eventos.
		 * REQUIRES:
		 * 	c 	!= NULL
		 */
		void insertConnection(BTConnection *c);
		
		/* Funcion que elimina una conexion de la lista para que
		* deje de ser verificada
		* REQUIRES:
		* 	conn != NULL
		*/
		void removeConnection(BTConnection *con);
		
		/* Funcion que devuelve todos los servers a los cuales se estan
		 * verificando los eventos.
		 * RETURNS:
		 * 	list != null
		 */
		 const list<BTSimpleServer *>& getServersList(void) 
		 {return this->serverList;};
		 
		 /* Funcion que devuelve la lista de todas las conexiones que
		  * se estan verificando los eventos
		  * RETURNS:
		  * 	list != null
		  */
		 const list<BTConnection *>& getConList(void)
		 {return this->connList;};
		 
		 /* Funcion que permite setear flags (events) de chequeo en una
		  * conexion determinada. (cuando se quiera enviar datos por ej
		  * debemos setearle el flag POLLOUT a la conexion..
		  * REQUIRES:
		  * 	c 	!= NULL
		  * 	c 	€ connList
		  */
		 void setFlags(BTConnection *c, short events);
		 
		
		 /*! Funcion principal, encargada de devolver la conexion
		  * involucrada, y que tipo de evento sucedio. 
		  * ES BLOQUEANTE! hasta que sucede un evento
		  * NOTE: cuando una conexion no tiene mas datos que enviar
		  *	  automaticamente se desactiva el POLLOUT.
		  * NOTE: automaticamente se hace el recv | send.
		  * NOTE: Se cierra automaticamente una conexion cuando se 
		  *	  produce un error, y se devuelve la conexion
		  * RETURNS:
		  * 	eType	= type of the event
		  * 	conn = connection involved in the event
		  * 	result	= (operation result, i.e: bytes Send | received)
		  *
		  * En caso de error (eType = algun error) en result devolvemos
		  * el fd que lo produjo.
		  * NOTE: genera memoria
		  */
		 BTConnection *getConnEvent(eventType_t &ev, int &result);
		 
		 /* Destructor.
		  * NOTE: NO libera ninguna conexion ni nada 
		  */
		~BTConnManager(void);
		
	private:
		/* 			funciones			*/
		
		/* funcion que hace cambia un struct fd de la posicion j a la posicion i
		* fdSet[i] = fdSet[j];
		* REQUIRES:
		 * 	i,j <= fdSetSize
		 */
		void fdSetAssign(int i, int j);
		
		/* funcion que duplica el tamaño del fdSet */
		void increasefdSetSize(void);
		
		
		/* Funcion que agrga un fd de una BTConnection al fd set
		* RETURNS:
		* 	false	on error 
		* 	true	if success
		*/
		void addConnFdToSet(int fd, int flags);
		
		/* Funcion que agrga un fd de un Server al fd set
		* RETURNS:
		* 	false	on error 
		* 	true	if success
		*/
		void addServerFdToSet(int fd, int flags);
		
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
		BTConnection *getConFromFd(int fd);
		
		/* funcion que devuelve un server desde un fd
		* RETURNS:
		* 	NULL 	on error || if not exists
		* 	BTSimpleServer != NULL	on success
		*/
		BTSimpleServer *getServerFromFd(int fd);
		
		/* Funcion que devuelve el indice correspondiente en el fdSet
		 * segun un socket determinado.
		 * RETURNS:
		 * 	< 0	if not found
		 * 	i > 0	otherwise
		 */
		int getIndexFromFd(int fd);
		
		/* Funcion que devuelve la primera conexion que tiene revens
		 * distinto de 0, y en caso de que no haya ninguna conexion
		 * con revents != 0, hace poll esperando alguna.
		 * RETURNS:
		 * 	index (fdSet[index].revents != 0)
		 *	< 0	on error
		 */
		int getIndexFdWithEvents(void);
		
		/*			 atributos 			*/
		/* Lista de servidores */
		list<BTSimpleServer *> serverList;
		/* lista de conexiones */
		list<BTConnection *> connList;
		/* el set de fd para las conexiones*/
		struct pollfd *fdSet;
		int fdSetMaxSize; 	/* tamaño maximo (para el realloc) */
		int fdSetSize; 		/* tamaño actual de conexiones */
		/* como vamos a tener servers y conexiones debemos distinguir
		 * cuales son los servers y cuales son las conexiones. asique
		 * vamos a tener un puntero que indica donde comienzan las
		 * conexiones, y de 0 a conPtr son los servers */
		int connPtr;

};



#endif
