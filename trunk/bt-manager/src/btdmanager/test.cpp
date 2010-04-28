#include "btdmanager.h"
#include "../btdongledevice/btdongledevice.h"
#include <iostream>


BTDManager *btdm;
BTDongleDevice *btdd;

void test_dongle(BTDongleDevice *btd)
{
	string *info = NULL;
	string *name = NULL;
	list<struct hci_conn_info *> *conList = NULL;
	list<struct hci_conn_info *>::iterator it;
	list<bdaddr_t *> *nearbyDev = NULL;
	list<bdaddr_t *>::iterator nit;
	list<string*> *nerabyNames = NULL;
	list<string*>::iterator sit;
	char mac[20] = {0};
	
	info = btd->getInfo();
	cout << "*****************************************************" << endl;
	cout << *info << endl;
	cout << "*****************************************************" << endl;
	delete info;
	
	name = btd->getDevName();
	cout << "\n\n Status: " << btd->getStatus() << "\tnombre: " << *name << endl;
	if (name)
		delete name;
	
	conList = btd->getPhysicalConnections();
	if(conList){
		for (it = conList->begin(); it != conList->end(); ++it) {
			ba2str(&(*it)->bdaddr, mac);
			cout << "con MAC: " << mac << "\n";
			free(*it);
		}
		delete conList;
	}
	
	/* buscamos dispositivos cercanos */
	nearbyDev = btd->scanNearbyDevices();
	if(nearbyDev){
		nerabyNames = btd->getFriendlyNames(nearbyDev);
		for (nit = nearbyDev->begin(); nit != nearbyDev->end(); ++nit) {
			ba2str(*nit, mac);
			cout << "NearbyDevice MAC: " << mac << "\n";
			free(*nit);
		}
		delete nearbyDev;
	}
	if (nerabyNames) {
		for (sit = nerabyNames->begin(); sit != nerabyNames->end(); ++sit) {
			cout << "NearbyFriendlyName: " << *(*sit) << "\n";
			delete *sit;
		}
		delete nerabyNames;
	}
	
	/* test_sdpsessions(btd);*/
		
}


int main (void)
{
	list<BTDongleDevice *> *listD = NULL;
	list<BTDongleDevice *>::iterator it;
	
	btdm = new BTDManager();
	
	cout << "Numero de dispositivos: " << btdm->getNumberOfDevices(HCI_UP) << endl;
	listD = btdm->getListDevices(HCI_UP);
	
	for(it = listD->begin(); it != listD->end(); ++it) {
		test_dongle(*it);
	}
	listD->clear();
	delete listD;
		
	
	delete btdm;
	
	return 0;
}