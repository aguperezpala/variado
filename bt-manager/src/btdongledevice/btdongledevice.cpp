#include <stdio.h>
#include "btdongledevice.h"


/*! 			Funciones privadas 			*/
void BTDongleDevice::initialize(void)
{
	this->sock = 0;
	this->devID = 0;
	this->sdpList.clear();
}

int BTDongleDevice::connectToDev(uint16_t dev_id)
{
	this->sock = hci_open_dev(dev_id);
	
	if(this->sock < 0)
		perror("Error al tratar de conectarse al usbdongle ");
	
	/* vamos a buscar la bdaddr del dispositivo */
	if (hci_devba(dev_id, &this->mac) < 0) 
		perror("Error cuando intentamos pedir la mac ");
	
	return this->sock;
}


/* Funcion que transforma un struct hci_dev_info en un string
* generando memoria...
* RETURNS:
* 	string with info	if no error
* 	NULL			on error
*/
string * BTDongleDevice::devInfoToStr(struct hci_dev_info * devInfo)
{
	string * result = NULL;
	struct hci_dev_stats *dStats = NULL;
	char aux[10];
	char *aux2 = NULL;
	
	
	if (!devInfo)
		return NULL;
	dStats = &(devInfo->stat);
	
	result = new string();
	if (!result)
		return NULL;
	
	result.append("Dev id: ");
	sprintf(aux, "%d",(int) devInfo->dev_id);
	result.append(aux);
	
	result.append("\nName: ");
	result.append(devInfo->name);
	
	result.append("\nMac: ");
	aux2 = batostr(&devInfo->bdaddr);
	result.append(aux2);
	free(aux2);
	
	result.append("\nFlags: ");
	aux2 = hci_dflagstostr(devInfo->flags);
	result.append(aux2);
	free(aux2);
	
	result.append("\nType: ");
	aux2 = hci_dtypetostr((int)devInfo->type);
	result.append(aux2);
	free(aux2);
	
	result.append("\nFeatures: FIXME ");
	/* uint8_t  features[8]; */
	
	result.append("\nPaketType: ");
	aux2 = hci_ptypetostr(devInfo->pkt_type);
	result.append(aux2);
	free(aux2);
	
	result.append("\nLinkPolicy: ");
	aux2 = hci_lptostr(devInfo->link_policy);
	result.append(aux2);
	free(aux2);
	
	result.append("\LinkMode: ");
	aux2 = hci_lmtostr(devInfo->link_mode);
	result.append(aux2);
	free(aux2);
	result.append("\nPaketType: ");
	aux2 = hci_ptypetostr(devInfo->pkt_type);
	result.append(aux2);
	free(aux2);
	
	result.append("\nACL_MTU: ");
	sprintf(aux, "%d",(int) devInfo->acl_mtu);
	result.append(aux);
	
	result.append("\nACL_PKTS: ");
	sprintf(aux, "%d",(int) devInfo->acl_pkts);
	result.append(aux);
	
	result.append("\nSCO_MTU: ");
	sprintf(aux, "%d",(int) devInfo->sco_mtu);
	result.append(aux);
	
	result.append("\nSCO_PKTS: ");
	sprintf(aux, "%d",(int) devInfo->sco_pkts);
	result.append(aux);
	
	/* Ahora las stasts */
	result.append("\n\nStats:\n");
	
	result.append("\nERR_TX: ");
	sprintf(aux, "%d",(int) dStats->err_tx);
	result.append(aux);
	
	result.append("\tERR_RX: ");
	sprintf(aux, "%d",(int) dStats->err_rx);
	result.append(aux);
	
	result.append("\nCMD_TX: ");
	sprintf(aux, "%d",(int) dStats->cmd_tx);
	result.append(aux);
	
	result.append("\nEVT_RX: ");
	sprintf(aux, "%d",(int) dStats->evt_rx);
	result.append(aux);
	
	result.append("\nACL_TX: ");
	sprintf(aux, "%d",(int) dStats->acl_tx);
	result.append(aux);
	
	result.append("\tACL_RX: ");
	sprintf(aux, "%d",(int) dStats->acl_rx);
	result.append(aux);
		
	result.append("\nSCO_TX: ");
	sprintf(aux, "%d",(int) dStats->sco_tx);
	result.append(aux);
	
	result.append("\tSCO_RX: ");
	sprintf(aux, "%d",(int) dStats->sco_rx);
	result.append(aux);
	
	result.append("\nBYTE_TX: ");
	sprintf(aux, "%d",(int) dStats->byte_tx);
	result.append(aux);
	
	result.append("\tBYTE_RX: ");
	sprintf(aux, "%d",(int) dStats->byte_rx);
	result.append(aux);
	
	return result;
	
}






/*!			Funciones publicas			*/

/* Constructor: Toma un dev_id */
BTDongleDevice::BTDongleDevice(uint16_t dev_id)
{
	initialize();
	
	/* inicializamos la conexion con el dispositivo */
	this->devID = dev_id;
	connectToDev(dev_id);
	
	
}

/* Constructor opcional, tomando una bdaddr_t del device */
BTDongleDevice::BTDongleDevice(const bdaddr_t *devMac)
{
	int result = 0;
	char auxMac[20] = {0};
	
	
	if (!devMac) {
		debugp("devMac == NULL\n");
		return;
	}
	/* copiamos la direccion */
	memcpy(&this->mac, devMac, sizeof(this->mac));
	
	/* ahora obtenemos el dev_id en base a la MAC */
	if (batostr(&this->mac, auxMac) < 0){
		debugp("Mac invalida\n");
		return;
	}
	result = hci_devid(auxMac);
	if (result < 0) {
		perror("No se pudo obtener el devID de la mac determinada ");
		return;
	}
	this->devID = (uint16_t) result;
	
	/* ahora nos conectamos */
	connectToDev(dev_id);
	
}

/* Funcion que establece la conexion con el dispositivo */
int BTDongleDevice::connectToDevice(void)
{
	return connectToDev(this->devID);
}


/* Funcion que devuelve el ID del dispositivo */
uint16_t BTDongleDevice::getID(void)
{
	return this->devID;
}

/* Funcion que devuelve la MAC del dispositivo 
* RETURNS:
* 	bdaddr_t *	
* 	NULL 		if error
*/
const bdaddr_t * BTDongleDevice::getMac(void)
{
	return &this->mac;
}

/* Funcion que devuelve el socket de la conexion establecida
* con el device.
*/
int BTDongleDevice::getSocket(void)
{
	return this->sock;
}

/* Funcion que devuelve un string con informacion (parseada) 
* sobre el dispositivo. No tiene ninguna utilidad.
*/
string * BTDongleDevice::getInfo(void)
{
	string * result = NULL;
	struct hci_dev_info devInfo;
	
	
	/* vamos a intentar primero que todo conectarnos con el dev */
	memset(&devInfo, '\0', sizeof(devInfo));
	if (hci_devinfo((int)this->devID, &devInfo) < 0) {
		perror("error al intentar conseguir la info del dev: ");
		return NULL;
	}
	
	/* si estamos aca => ya obtuvimos la info => la parseamos */
	result = devInfoToStr(&devInfo);
}

/* Devuelve el estado en el que se encuentra el dispositivo 
* FIXME: verificar donde se puede obtener el estado 
*/
int BTDongleDevice::getStatus(void)
{
	/*! FIXME: podriamos intentar hacer un pedido al microcontrolador
	 * y de esta forma vemos si nos responde bien o no, por ejemplo,
	 * podriamos pedir el nombre del dispositivo o alguna otra cosa */
	
}

/* Funcion que devuelve el nombre del dispositivo.
* NOTE: Eliminar luego el nombre (genera memoria).
*/
string * BTDongleDevice::getDevName(void)
{
	string *result = NULL;
	char devName[248] = {0};
	
	if (hci_read_local_name(this->sock, sizeof(devName), devName,
		HCI_REQ_TIMEOUT) < 0) {
		
		perror("No se pudo leer el nombre del dispositivo ");
		return NULL;
	}
	/* si lo pudimos leer */
	result = new string(devName);
	return result;
}

/* Funcion que setea el nombre al dispositivo local. */
void BTDongleDevice::setDevName(const string *name)
{
	if (!name) {
		debugp("Nos dieron un name NULL\n");
		return;
	}
	
	if (hci_write_local_name(this->sock, (char *)name->c_str()), 
		HCI_REQ_TIMEOUT) < 0) {
		perror("No se pudo escribir el nombre del dispositivo ");
		return;
	}
}

/* Funcion que obtiene una lista de conexiones establecidas con
* el dispositivo. Son las conexiones de bajo "nivel" (physical
* links).
* RETURNS:
* 	list<hci_conn_info *> *		on success
*	NULL				if no conn or error.
* NOTE: se genera memoria para cada hci_conn_info y para
* 	 la lista. (veer hci,h para la struct hci_conn_info).
*/
list<struct hci_conn_info *> * BTDongleDevice::getPhysicalConnections(void)
{
	list<struct hci_conn_info*> *result = NULL;
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	int i;
	int adapter_id , sock;
	
	
	if (!(cl = malloc(HCI_MAX_DEV * sizeof(*ci) + sizeof(*cl)))) {
		perror("No pudimos alojar memoria");
		return NULL;
	}
	cl->dev_id = adapter_id;
	cl->conn_num = HCI_MAX_DEV;
	ci = cl->conn_info;
	
	if (ioctl(sock, HCIGETCONNLIST, (void*)cl)) {
		perror("No se pudo obtener la lista de conexiones");
		free (cl);
		return NULL;
	}
	
	/* verificamos si obtuvimos alguna conexion */
	if (cl->conn_num <= 0)
		/* no obtuvimos ninguna */
		return NULL;
	
	/* si estamos aca es porque SI hay conexiones! */
	
	for (i = 0; i < cl->conn_num; i++, ci++) {
		char addr[18];
		ba2str(&ci->bdaddr, addr);
		printf("\t%s %s %s handle %d state %d lm %s\n",
			ci->out ? "<" : ">",
			ci->type == ACL_LINK ? "ACL" : "SCO",
			addr, ci->handle, ci->state,
			hci_lmtostr(ci->link_mode));
	}
	return 0;

/* Funcion que va a forzar el cierre de una conexion con un
* dispositivo determinado.
* REQUIRES:
* 	baDst		!= NULL
* 	reason		(Reason close code)
* RETURENS;
* 	>= 0 	on success
* 	<  0	on error
*/
int BTDongleDevice::closePhysicalConnection(bdaddr_t * baDst, uint8_t reason);


/* Funcion que hace un escaneo de dispositivos devolviendo
* una lista de bdaddr_t * (de macs) las cuales tienen
* que ser liberadas una por una incluyendo la lista.
* RETURN:
* 	list * of mac	if no error
* 	NULL		on error
* NOTE: genera memoria para cada bdaddr_t y la lista.
*/
list<bdaddr_t *> * BTDongleDevice::scanNearbyDevices(void);

/* Funcion que devuelve una lista de friendly names en base
* a una lista de MACs.
* REQUIRES:
* 	macList != NULL.
* RESULT:
* 	result == list<string> * with friendlynames.
* 	 result.size() == macList.size().
*	NULL on error
* NOTE: se genera memoria para cada friendlyname & list.
*/
list<string*> * BTDongleDevice::getFriendlyNames(list<bdaddr_t *> *macList);

/*! 			SDP SERVER			*/

/* Funcion que devuelve una lista con las sessiones al servidor
* SDP del dongle actual.
* RETURNS:
* 	list<const BTSDPSessionData*> *list
* 	NULL	on error
*/
const list<const BTSDPSessionData*>& BTDongleDevice::getSDPSessions(void);

/* Funcion que elimina un servicio (SDP) determinado,
* REQUIRES:
* 	btS 	!= NULL
* 	btS € this.btSDPSessionList
* RETURNS:
* 	true 	on success (==> free(btS))
* 	false	otherwise (==> !free(btS))
* NOTE: btS es eliminado luego de esta llamada.
*/
bool BTDongleDevice::removeSDPSession(BTSDPSessionData *btS);

/* Funcion que inserta un nuevo servicio al SDP local, si y solo
* si este no se encuentra ya corriendo en el server.
* REQUIRES:
* 	btS 	!= NULL
* RETURNS:
* 	> 0	on succes
* 	< 0	on error
* NOTE: No debe ser eliminado btS luego de haber llamado a esta
* 	 funcion.
*/
int BTDongleDevice::addSDPSession(BTSDPSessionData *btS);


/*! Podriamos agregar mas funciones para comunicarnos con el
* dispositivo pero son muchas (pero para poder ejecutar estas
* basta con pedir el dev_id y el socket en algunos casos).
* Vamos a implementar el envio de comandos y el envio de 
* requests.
*/

/* Funcion que envia un commando determinado al dispositivo.
* RETURNS:
* 	codeErr (veer hci.h/hci_lib.h)
*/
int BTDongleDevice::sendCmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param);

/* Funcion que realiza un request al dipositivo.
* RETURNS:
* 	codeErr (ver hci.h/hci_lib.h)
*/
int BTDongleDevice::sendReq(struct hci_request *req, int timeout);

/* Destructor.
* Se desconecta del dispositivo y elimina todo los datos
*/
BTDongleDevice::~BTDongleDevice(void);