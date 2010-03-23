#include <errno.h>
#include <set>
#include "btdmanager.h"

/* Funcion que actualiza los usb dongles (la lista). */
void BTDManager::updateDeviceList(void)
{
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
	int i, sk;
	BTDongleDevice * btDev = NULL;
	set<uint16_t> devIDs; 
	list<BTDongleDevice*>::iterator it;
	
	sk = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
	if (sk < 0)
		printErr("Error al intentar conectarse al bus HCI\n");
		return;
	
	dl = (hci_dev_list_req*) malloc(HCI_MAX_DEV * sizeof(*dr) + sizeof(*dl));
	if (!dl) {
		printErr("");
		goto done;
	}
	
	dl->dev_num = HCI_MAX_DEV;
	dr = dl->dev_req;
	
	if (ioctl(sk, HCIGETDEVLIST, (void *) dl) < 0) {
		printErr("");
		goto free;
	}
	
	/* vamos a obtener la lista de dispositivos (ids) que ya tenemos */
	devIDs.clear();
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it)
		if (*it != NULL)
			devIDs.insert((*it)->getID());
		else
			this->devicesList.remove((*it));
	
	/* ahora vamos a verificar la lista de dispositivos, y vamos a verificar
	 * si tenemos o no ese elemento... */
	for (i = 0; i < dl->dev_num; i++, dr++) {
		if (devIDs.find(dr->dev_id) == set::end) {
			/* NO lo encontramos ==> generamos uno nuevo y lo 
			 * agregamos*/
			btDev = NULL;
			btDev = new BTDongleDevice(dr->dev_id);
			if (btDev)
				this->devicesList.insert(btDev);
			else
				debugp("Error al crear el BTDongleDevice");
		}
	}
	
	devIDs.clear();
	
	free:
	free(dl);
	
	done:
	close(sk);
}


BTDManager::BTDManager(void)
{
	/* inicializamos */
	devicesList.clear();
}

/* Obtiene la cantidad de dispositivos HCI que cumplan con una
* flag determinada (ej: HCI_UP, revisar hci.h) y devuelve
* un entero indicando la cantidad.
* RETURNS:
* 	numOfDevices
*/
int BTDManager::getNumberOfDevices(int flag)
{
	int result = 0;
	list<BTDongleDevice*>::iterator it;
	BTDongleDevice *btDev = NULL;
	uint32_t opt = 0;
	
	if (flag < 0)
		return this->devicesList.size();
	
	updateDeviceList();
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		btDev = (*it);
		if (!btDev){
			this->devicesList.remove(*it);
			continue;
		}
		opt = btDev->getOpt();
		if (hci_test_bit(flag, &opt))
			result++;
	}
	return result;
}
	

/* Devuelve una referencia de la lista de BTDonglesDevices 
* del sistema segun un flag determinado (ver hci.h para flags).
* RETURNS:
* 	NULL	on error
* 	list	otherwise
* NOTE: tener cuidado de no usar 2 veces el mismo dongle. 
* NOTE: NO eliminar ningun BTDongleDevice. Esta clase se 
* 	 encarga de hacerlo 
*/
list<BTDongleDevice *> * BTDManager::getListDevices(int flag)
{
	list<BTDongleDevice*>::iterator it;
	list<BTDongleDevice*> *result = NULL;
	BTDongleDevice *btDev = NULL;
	uint32_t opt = 0;
	
	updateDeviceList();
	
	/* creamos la lista a devolver */
	result = new list<BTDongleDevice*>();
	
	if (flag < 0){
		result = this->devicesList();
		return result;
	}
		
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		btDev = (*it);
		if (!btDev){
			this->devicesList.remove(*it);
			continue;
		}
		opt = btDev->getOpt();
		if (hci_test_bit(flag, &opt))
			result.insert(btDev);
	}
	return result;
}


/* Destructor (elimina todos los BTDongleDevice's creados) */
BTDManager::~BTDManager(void)
{
	list<BTDongleDevice *>::iterator it;
	
	/* TEST:eliminamos todos los elementos uno por uno 
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		if (*it)
			delete *it;*/
	this.devicesList.clear();
}