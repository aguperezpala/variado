/*! Manejador de dongles del sistema.
 *  Se encarga de determinar cuantos Dongles tiene el sistema, 
 *  y obtener determinada informacion (address, name, etc).
 *  Genera instancias de BTDongleDevices
 *  
 */  
#ifndef BTDMANAGER_H
#define BTDMANAGER_H


#include <list>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <stdlib.h>
#include <assert.h>
/* libs propias */
#include "debug.h"
#include "consts.h"
#include "btdongledevice.h"



using namespace::std;

class BTDManager {
	
	public:
		
		BTDManager(void);
		
		/* Obtiene la cantidad de dispositivos HCI que cumplan con una
		 * flag determinada (ej: HCI_UP, revisar hci.h) y devuelve
		 * un entero indicando la cantidad.
		 * Si flag = -1 ==> no tenemos en cuenta flag
		 * RETURNS:
		 * 	numOfDevices
		 */
		int getNumberOfDevices(int flag);
		
		/* Devuelve una referencia de la lista de BTDonglesDevices 
		 * del sistema segun un flag determinado (ver hci.h para flags).
		 * Si flag == 0 => devolvemos lista completa
		 * RETURNS:
		 * 	NULL	if no devices are connected or error.
		 * 	list	otherwise
		 * NOTE: tener cuidado de no usar 2 veces el mismo dongle. 
		 * NOTE: NO eliminar ningun BTDongleDevice. Esta clase se 
		 * 	 encarga de hacerlo pero SI eliminar la lista.
		 */
		list<BTDongleDevice *> * getListDevices(int flag);
		
		/* Funcion que devuelve un Dongle desde una mac determinada
		 * REQUIRES:
		 * 	mac != NULL
		 * RETURNS:
		 * 	NULL	si no existe dongle con tal mac
		 * 	dongle	caso contrario.
		 */
		BTDongleDevice *getDongleFromMac(bdaddr_t *mac);
		
		/* Destructor (elimina todos los BTDongleDevice's creados) */
		~BTDManager(void);
		
	
	private:
		/* Funciones */
		/* Funcion que actualiza los usb dongles (la lista). */
		void updateDeviceList(void);
		
		/* Argumentos */
		
		/* lista de los dispositivos */
		list<BTDongleDevice *> devicesList;
		
		
};



#endif
