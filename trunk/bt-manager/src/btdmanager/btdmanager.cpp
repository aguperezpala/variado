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
	if (sk < 0) {
		printErr("Error al intentar conectarse al bus HCI\n");
		return;
	}
	
	dl = (hci_dev_list_req*) malloc(HCI_MAX_DEV * sizeof(*dr) + sizeof(*dl));
	if (!dl) {
		printErr("ERROR al crear dl");
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
		if (devIDs.find(dr->dev_id) == devIDs.end()) {
			/* NO lo encontramos ==> generamos uno nuevo y lo 
			 * agregamos*/
			btDev = NULL;
			btDev = new BTDongleDevice(dr->dev_id);
			if (btDev)
				this->devicesList.push_front(btDev);
			else
				debugp("Error al crear el BTDongleDevice");
		} 
	}
	
	devIDs.clear();
	
	free:
	if (dl)
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
	struct hci_dev_info devInfo;
	BTDongleDevice *btDev = NULL;
	
	updateDeviceList();
	
	if (flag < 0)
		return this->devicesList.size();
	
	
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		btDev = (*it);
		if (!btDev){
			this->devicesList.remove(*it);
			continue;
		}		
		if (btDev->getDevInfo(&devInfo) && 
			(hci_test_bit(flag, &devInfo.flags))){
			result++;
		}
	}
	cout <<"asdasda\n";
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
	struct hci_dev_info devInfo;
	BTDongleDevice *btDev = NULL;
	
	updateDeviceList();
	
	/* creamos la lista a devolver */
	result = new list<BTDongleDevice*>();
	
	if (flag < 0){
		*result = this->devicesList;
		return result;
	}
		
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		btDev = (*it);
		if (!btDev){
			this->devicesList.remove(*it);
			continue;
		}
		
		if (btDev->getDevInfo(&devInfo) && 
			(hci_test_bit(flag, &devInfo.flags)))
			result->push_front(btDev);
	}
	return result;
}

/* Funcion que devuelve un Dongle desde una mac determinada
* REQUIRES:
* 	mac != NULL
* RETURNS:
* 	NULL	si no existe dongle con tal mac
* 	dongle	caso contrario.
*/
BTDongleDevice *BTDManager::getDongleFromMac(bdaddr_t *mac)
{
	BTDongleDevice *result = NULL;
	list<BTDongleDevice*>::iterator it;
	
	assert(mac != NULL);
	
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it){
		if(bacmp((*it)->getMac(), mac) == 0)
			return (*it);
	}
	
	return NULL;
}

/* Destructor (elimina todos los BTDongleDevice's creados) */
BTDManager::~BTDManager(void)
{
	list<BTDongleDevice *>::iterator it;
	
	/* TEST:eliminamos todos los elementos uno por uno */
	for (it = this->devicesList.begin(); it != this->devicesList.end(); ++it)
		if (*it)
			delete *it;
	this->devicesList.clear();
}