
#include <iostream>
#include <string>
#include <list>
#include <vector>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <poll.h>


#include "btsimpleserver.h"
#include "../btconnection/btconnection.h"

#include "../tester.h"

BTSimpleServer *ss;


bool addFdToSet(struct pollfd *pSet, int fd, int flags, int *size)
{
	bool result = false;
	
	/* verificamos que no hayamos salido por la guarda */
	if (*size >= 11) {
		/* no agregamos un chori */
		return result;
	}
	
	pSet[*size].fd = fd;
	pSet[*size].events = flags;
	/* si lo agregamos */
	result = true;
	*size = *size + 1;
	
	return result;
}

/* Funcion que saca un fd del set y lo re-ordena
* RETURNS:
* 	true 	if success
*	false	on error
*/
bool removeFdFromSet(struct pollfd *pSet, int fd, int *size)
{
	int i = 0;
	bool result = false;
	
	/* lo buscamos */
	for (i = 0; i <= *size; i++){
		if (pSet[i].fd == fd)
			/* Lo encontramos ==> salimos */
			break;
	}
	
	/* verificamos que lo hayamos encontrado... */
	if (i == *size) {
		/* NO lo encontramos ==> devolvemos error */
		return result;
	}
	/* si lo encontramos, debemos ver ahora que pos esta, si es la ultima
	* entonces directamente igualamos a 0 el arreglo */
	result = true;
	/* swap */
	if (i == *size - 1) {
		pSet[i].fd = 0;
	} else {		
		pSet[i].fd = pSet[*size - 1].fd;
		pSet[i].revents = pSet[*size - 1].revents;
		pSet[*size  - 1].fd = 0;
		pSet[*size  - 1].revents = 0;
		
	}
	/* borramos 1 */
	*size = *size - 1;
	
	return result;
}


void work_connection(BTConnection *con, struct pollfd *pSet, int *size, int status)
{
	
	if (status & (POLLIN | POLLPRI)) {
		/* estamos recibienbdo */
		int bytes = 0;
		
		bytes = con->recvData();
		cout << "recibymos bytes: " << bytes << "\n";
		if (bytes == 0) {
			cout << "nos cerraron la conexion\n";
			removeFdFromSet(pSet, con->getSocket(), size);
		}
		if (bytes < 0) {
			perror("no pudimos recibir nada\n");
			return;
		}
		cout << "Recibimos: " << con->getRcvBuff() << endl;
	} 
}


int main (void)
{
	bdaddr_t *dMac = strtoba("6A:54:01:7D:1A:00");
	//bdaddr_t *dMac = strtoba("11:11:11:11:11:11");
	int status = 0, i = 0;
	struct pollfd pSet[10];
	int pollSize = 1;
	bool running = true;
	BTConnection *connections[11];
	
	
	//connections = malloc(10*sizeof(BTConnection *));
	ss = new BTSimpleServer(dMac, 7, 11);
	
	cout << "socket: "<< ss->getSocket() << "\tport: " << ss->getPort();
	cout << "\tqSize: " << ss->getQueueSize() << endl;
	
	status = ss->bindSocket();
	if (status < 0) {
		perror("error al hacer bind\n");
		goto endP;
	}
	status = ss->startListen();
	if(status < 0) {
		perror("Error al hacer listen\n");
		goto endP;
	}
	
	for (i = 0; i <= 10; i++) {
		pSet[i].fd = 0;
		pSet[i].events = POLLIN | POLLPRI | POLLNVAL;
		pSet[i].revents = 0;
	}
	pSet[0].fd = ss->getSocket();
	
	while (running) {
		status = poll(pSet, pollSize, -1);
		if (status < 0) {
			perror("Error en poll\n");
			goto endP;
		}
		
		if (pSet[0].revents & (POLLIN | POLLPRI )) {
			cout << "recibimos una nueva conexion\n";
			/* agregamos una conexion */
			connections[pollSize] = ss->acceptConn(&status);
			if (status >= 0 && connections[pollSize]) {
				int flags = POLLIN | POLLPRI | POLLNVAL | POLLOUT |
				POLLRDHUP | POLLERR | POLLHUP | POLLNVAL;
				cout << "agregamos la conexion al pSet\n";
				addFdToSet(pSet, status, flags, &pollSize);
			} else 
				cout << "Error al hacer el accept" << status << "\n";
			
			pSet[0].revents = 0;
		}
		for (i = 1; i <= pollSize; i++) {
			if (pSet[i].revents != 0){
				if ((pSet[i].revents & POLLOUT) && 
					connections[i]->getSendBuff().size() == 0) {
					pSet[i].revents = 0;
					pSet[i].events = POLLIN | POLLPRI;
					continue;
				
				} else if(pSet[i].revents & (POLLHUP | POLLRDHUP)) {
					cout << "Nos cerraron la conexion \n";
					removeFdFromSet(pSet, i, &pollSize);
					continue;
				}
				cout << "encontramos una conexion con revents != 0\n";
				work_connection(connections[i], pSet, &pollSize,
						 (int)pSet[i].revents);
				pSet[i].revents = 0;
			}
		}
		
		
	}
	
	endP:
	delete ss;
	return 0;
}