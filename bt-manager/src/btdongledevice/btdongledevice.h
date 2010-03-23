/*! Manejador de dongles del sistema.
 *  Se encarga de determinar cuantos Dongles tiene el sistema, 
 *  y obtener determinada informacion (address, name, etc).
 *  Genera instancias de BTDongleDevices
 *  
 */  
#ifndef BTDONGLEDEVICE_H
#define BTDONGLEDEVICE_H

#include <iostream>
#include <string>
#include <list>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
/* libs propias */


using namespace::std;

class BTDongleDevice {
	
	public:
		/*FIXME*/
		BTDongleDevice(void){};
		
		
		/* FIXME:Destructor */
		~BTDongleDevice(void){};
		
	
	private:
		/* Argumentos */
		
};



#endif
