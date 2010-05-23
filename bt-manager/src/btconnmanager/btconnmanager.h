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
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <poll.h>
/* libs propias */
#include "simpleserver.h"
#include "btconnection.h"

#include "consts.h"
#include "debug.h"

using namespace::std;

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
		
		/* funcion que permite agregar una conexion para verificar
		 * sus eventos.
		 * REQUIRES:
		 * 	c 	!= NULL
		 */
		void insertConnection(BTConnection *c);
		
		/* Funcion que devuelve todos los servers a los cuales se estan
		 * verificando los eventos.
		 * RETURNS:
		 * 	list != null
		 */
		 const list<BTSimpleServer *>& getServersList(void) 
		 {return this->serversList;};
		 
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
		 void setFlags(short events);
		 
		
		 /*! Funcion principal, encargada de devolver la conexion
		  * involucrada, y que tipo de evento sucedio. 
		  * ES BLOQUEANTE! hasta que sucede un evento
		  * NOTE: cuando una conexion no tiene mas datos que enviar
		  *	  automaticamente se desactiva el POLLOUT.
		  * NOTE: automaticamente se hace el recv | send.
		  * REQUIRES:
		  * 	addr != NULL
		  * RETURNS:
		  * 	eType	= see poll
		  * 	addr 	= connection involved in the event
		  * 	result	= (operation result, i.e: bytes Send | received)
		  *
		  * NOTE: genera memoria
		  */
		 int getConnEvent(btaddr_t *addr, int &result);
		 
		 /* Destructor.
		  * NOTE: NO libera ninguna conexion ni nada 
		  */
		~BTConnManager(void){};
		
	private:
		/* 			funciones			*/
		
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
