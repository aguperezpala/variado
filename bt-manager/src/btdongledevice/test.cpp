
#include <iostream>
#include <string>
#include <list>
#include <vector>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "btdongledevice.h"
#include "../btsdpsessiondata/btsdpsessiondata.h"
#include "../tester.h"

#define REAL_MAC	"00:1A:7D:01:54:6A"


BTDongleDevice * btd;


void test_physicalConn(void)
{
	list<struct hci_conn_info *> *phyList = NULL;
	
	phyList = btd->getPhysicalConnections();
	fail_if(phyList != NULL);
	/*! FIXME: falta testear cuando realmente haya una conexion, si
	 * se puede cerrar o no la conexion.*/
	
}

void test_scanDevices(void)
{
	
	list<bdaddr_t *> * devList = NULL;
	list<string*> *devNames = NULL;
	list<string*>::iterator it2;
	list<bdaddr_t *>::iterator it;
	char devName[20] = {0};
	
	devList = btd->scanNearbyDevices();
	fail_if(devList == NULL);
	fail_if(devList->size() != 1);
	
	devNames = btd->getFriendlyNames(devList);
	fail_if(devNames == NULL);
	fail_if(devNames->size() != 1);
	
	
	for (it = devList->begin(); it != devList->end(); ++it) {
		ba2str((*it), devName);
		cout << "encontramos dev: " << devName << endl;
		free(*it);
	}
	for (it2 = devNames->begin(); it2 != devNames->end(); ++it2) {
		cout << "nombre de dispositivo: " << *(*it2) << endl;
		delete (*it2);
	}
	delete devList;
	delete devNames;
	
	
}

void test_sdpServices(void)
{
	const list<BTSDPSessionData*>* sessions = NULL;
	BTSDPSessionData *btS = new BTSDPSessionData();
	
	sessions = btd->getSDPSessions();
	fail_if(sessions == NULL);
	fail_if(sessions->size() != 0);
	fail_if(btd->removeSDPSession(btS) != false);
	fail_if(btd->addSDPSession(btS) != 0);
	fail_if(btd->addSDPSession(btS) >= 0);
	sessions = btd->getSDPSessions();
	fail_if(sessions == NULL);
	fail_if(btd->removeSDPSession(btS) != true);

}


int main (void)
{
	const bdaddr_t *mac = NULL;
	char strMac[20] = {0};
	string *info = NULL;
	string newName = "Dongle Agu-desktop";
	
	btd = new BTDongleDevice((uint16_t)0);
	
	
	
	fail_if(btd->getSocket() <= 0);
	
	/* chequeamos dev_id */
	fail_if(btd->getID() != 0);
	
	/* obtenemos la mac */
	mac = btd->getMac();
	fail_if(mac == NULL);
	ba2str(mac, strMac);
	fail_if(strcmp(REAL_MAC,strMac) != 0);
	
	/* obtenemos la info */
	cout << "LLEGAMOS ACA " << endl;	
	info = btd->getInfo();
	cout << "****** INFO *********" << endl << *info << endl;
	delete info;
	
	/* probamos el estado */
	cout << "estado: " << btd->getStatus() << endl;
	
	/* chequeamos el nombre */
	info = btd->getDevName();
	fail_if(info == NULL);
	cout << "Nombre del dispositivo: " << *info << endl;
	delete info;
	
	btd->setDevName(&newName);
	info = btd->getDevName();
	fail_if (*info != newName);
	delete info;
	
	/* testeamos las conexiones physicas */
	test_physicalConn();
	
	/* testeamos buscar dispositivos y obtener sus nombres */
	test_scanDevices();
	
	/* testeamos funciones relacionadas con el sdpServer */
	test_sdpServices();
	
	delete btd;
	
	return 0;
}