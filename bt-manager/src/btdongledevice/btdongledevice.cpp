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
	/* primero verificamos si tenemos abierta una conexion, si es asi
	 * la cerramos */
	if (this->sock) {
		close(this->sock); this->sock = 0;
	}
	
	/* abrimos */
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
		
	result->append("Dev id: ");
	sprintf(aux, "%d",(int) devInfo->dev_id);
	result->append(aux);
	
	result->append("\nName: ");
	result->append(devInfo->name);
	
	result->append("\nMac: ");
	aux2 = batostr(&devInfo->bdaddr);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	
	result->append("\nFlags: ");
	aux2 = hci_dflagstostr(devInfo->flags);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	
	result->append("\nType: ");
	aux2 = hci_dtypetostr((int)devInfo->type);
	if (aux2) {
		result->append(aux2);
	}
	
	result->append("\nFeatures: FIXME ");
	/* uint8_t  features[8]; */
	
	result->append("\nPaketType: ");
	aux2 = hci_ptypetostr(devInfo->pkt_type);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	
	result->append("\nLinkPolicy: ");
	aux2 = hci_lptostr(devInfo->link_policy);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	
	result->append("\nLinkMode: ");
	aux2 = hci_lmtostr(devInfo->link_mode);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	result->append("\nPaketType: ");
	aux2 = hci_ptypetostr(devInfo->pkt_type);
	if (aux2) {
		result->append(aux2);
		free(aux2);
	}
	
	result->append("\nACL_MTU: ");
	sprintf(aux, "%d",(int) devInfo->acl_mtu);
	result->append(aux);
	
	result->append("\nACL_PKTS: ");
	sprintf(aux, "%d",(int) devInfo->acl_pkts);
	result->append(aux);
	
	result->append("\nSCO_MTU: ");
	sprintf(aux, "%d",(int) devInfo->sco_mtu);
	result->append(aux);
	
	result->append("\nSCO_PKTS: ");
	sprintf(aux, "%d",(int) devInfo->sco_pkts);
	result->append(aux);
	
	/* Ahora las stasts */
	result->append("\n\nStats:\n");
	
	result->append("\nERR_TX: ");
	sprintf(aux, "%d",(int) dStats->err_tx);
	result->append(aux);
	
	result->append("\tERR_RX: ");
	sprintf(aux, "%d",(int) dStats->err_rx);
	result->append(aux);
	
	result->append("\nCMD_TX: ");
	sprintf(aux, "%d",(int) dStats->cmd_tx);
	result->append(aux);
	
	result->append("\nEVT_RX: ");
	sprintf(aux, "%d",(int) dStats->evt_rx);
	result->append(aux);
	
	result->append("\nACL_TX: ");
	sprintf(aux, "%d",(int) dStats->acl_tx);
	result->append(aux);
	
	result->append("\tACL_RX: ");
	sprintf(aux, "%d",(int) dStats->acl_rx);
	result->append(aux);
		
	result->append("\nSCO_TX: ");
	sprintf(aux, "%d",(int) dStats->sco_tx);
	result->append(aux);
	
	result->append("\tSCO_RX: ");
	sprintf(aux, "%d",(int) dStats->sco_rx);
	result->append(aux);
	
	result->append("\nBYTE_TX: ");
	sprintf(aux, "%d",(int) dStats->byte_tx);
	result->append(aux);
	
	result->append("\tBYTE_RX: ");
	sprintf(aux, "%d",(int) dStats->byte_rx);
	result->append(aux);
	
	return result;
	
}



/* funcion que cierra una conexion segun su identificador. y
* con un codigo de razon de cierre de conexion.
* RETURNS:
* 	< 0 	on error
* 	0	on success (ver hci_disconnect -hci_lib.h-) 
*/
int BTDongleDevice::disconnectPhyCon(uint16_t cHanndle, uint8_t reason)
{
	return hci_disconnect(this->sock, cHanndle, reason, 0);
}


/* Libera una conexion con el SDP server.
* REQUIRES:
* 	sdpData != NULL
*/
void BTDongleDevice::destroySDPService(BTSDPSessionData *sdpData)
{
	if(!sdpData){ 
		debugp("Estamos recibiendo sdpData NULL\n");
		return;
	}
	sdp_data_free (sdpData->channel ) ;
	sdp_list_free (sdpData->l2cap_list , 0 ) ;
	sdp_list_free (sdpData->rfcomm_list , 0 ) ;
	sdp_list_free (sdpData->root_list , 0 ) ;
	sdp_list_free (sdpData->proto_list ,0);
	sdp_list_free (sdpData->access_proto_list , 0 ) ;
	sdp_list_free (sdpData->svc_class_list , 0 ) ;
	sdp_list_free (sdpData->profile_list , 0 ) ;
	sdp_record_free(sdpData->record); sdpData->record = NULL;
	sdp_close(sdpData->session); sdpData->session = NULL;
}

/* Funcion que se conecta con el servidor SDP y establece
* un nuevo servicio devolviendo un codigo de error
* REQUIRES:
* 	sdpData 	!= NULL
* RETURNS:
* 	< 0		on error
* 	== 0		if success
*/
int BTDongleDevice::createSDPService(BTSDPSessionData *sdpData)
{
	
	if (!sdpData) {
		debugp("Recibimos un sdpData NULL\n");
		return -1;
	}
	
	// set the general service ID
	sdp_set_service_id ( sdpData->record , sdpData->svc_uuid ) ;
	
	// set the service class
	sdp_uuid16_create(&sdpData->svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
	sdpData->svc_class_list = sdp_list_append(0, &sdpData->svc_class_uuid);
	sdp_set_service_classes(sdpData->record , sdpData->svc_class_list);
	
	// set the Bluetooth profile information
	sdp_uuid16_create(&sdpData->profile.uuid , SERIAL_PORT_PROFILE_ID ) ;
	sdpData->profile.version = 0x0100 ;
	sdpData->profile_list = sdp_list_append( 0 , &sdpData->profile ) ;
	sdp_set_profile_descs (sdpData->record , sdpData->profile_list ) ;
	
	// make the service record publicly browsable
	sdp_uuid16_create(&sdpData->root_uuid , PUBLIC_BROWSE_GROUP ) ;
	sdpData->root_list = sdp_list_append ( 0 , &sdpData->root_uuid ) ;
	sdp_set_browse_groups ( sdpData->record , sdpData->root_list ) ;
	
	// set l2cap information
	sdp_uuid16_create(&sdpData->l2cap_uuid , L2CAP_UUID ) ;
	sdpData->l2cap_list = sdp_list_append ( 0 , &sdpData->l2cap_uuid ) ;
	sdpData->proto_list = sdp_list_append ( 0 , sdpData->l2cap_list ) ;
	
	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&sdpData->rfcomm_uuid , RFCOMM_UUID );
	sdpData->channel = sdp_data_alloc(SDP_UINT8, &sdpData->rfcomm_channel);
	sdpData->rfcomm_list = sdp_list_append (0, &sdpData->rfcomm_uuid);
	sdp_list_append (sdpData->rfcomm_list , sdpData->channel);
	sdp_list_append (sdpData->proto_list , sdpData->rfcomm_list);
	sdpData->access_proto_list = sdp_list_append(0, sdpData->proto_list);
	sdp_set_access_protos (sdpData->record , sdpData->access_proto_list);
	
	// set the name, provider, and description
	sdp_set_info_attr(sdpData->record , sdpData->serviceName,
			   sdpData->serviceProv , sdpData->serviceDsc);
	
	/*! nos vamos a conectar al sdp server del dispositivo local... */
	sdpData->session = sdp_connect(&(this->mac), BDADDR_LOCAL, 
					SDP_RETRY_IF_BUSY);
	if (sdpData->session == NULL){
		perror("Error al conectarse con el sdp local\n");
		/* liberamos todo */
		destroySDPService(sdpData);
		return -1;
	}
	sdp_record_register(sdpData->session, sdpData->record, 0);
	
	/* todo lindo devolvemos 0 */
	return 0;
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
	char *auxMac = NULL;
	
	
	if (!devMac) {
		debugp("devMac == NULL\n");
		return;
	}
	/* copiamos la direccion */
	memcpy(&this->mac, devMac, sizeof(this->mac));
	
	/* ahora obtenemos el dev_id en base a la MAC */
	auxMac = batostr(&this->mac);
	if (auxMac == NULL){
		debugp("Mac invalida\n");
		return;
	}
	result = hci_devid(auxMac);
	free(auxMac);
	if (result < 0) {
		perror("No se pudo obtener el devID de la mac determinada ");
		return;
	}
	this->devID = (uint16_t) result;
	
	/* ahora nos conectamos */
	connectToDev(this->devID);
	
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
	struct hci_version ver;
	/*! FIXME: podriamos intentar hacer un pedido al microcontrolador
	 * y de esta forma vemos si nos responde bien o no, por ejemplo,
	 * podriamos pedir el nombre del dispositivo o alguna otra cosa */
	return hci_read_local_version(this->sock, &ver, 0);
	
}

/* Funcion que devuelve el nombre del dispositivo.
* NOTE: Eliminar luego el nombre (genera memoria).
*/
string * BTDongleDevice::getDevName(void)
{
	string *result = NULL;
	char devName[248] = {0};
	
	if (hci_read_local_name(this->sock, sizeof(devName), devName,
		0) < 0) {
		
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
	
	if (hci_write_local_name(this->sock, (char *)name->c_str(), 0) < 0) {
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
	struct hci_conn_info *ci, *conInfo = NULL;
	int i;
	int adapter_id;
	
	cl = (struct hci_conn_list_req *) malloc(HCI_MAX_DEV * sizeof(*ci) + sizeof(*cl));
	if (!(cl)) {
		perror("No pudimos alojar memoria");
		return NULL;
	}
	cl->dev_id = adapter_id;
	cl->conn_num = HCI_MAX_DEV;
	ci = cl->conn_info;
	
	if (ioctl(this->sock, HCIGETCONNLIST, (void*)cl)) {
		perror("No se pudo obtener la lista de conexiones");
		free (cl);
		return NULL;
	}
	
	/* verificamos si obtuvimos alguna conexion */
	if (cl->conn_num <= 0) {
		/* no obtuvimos ninguna */
		debugp("No hay establecida ninguna conexion\n");
		free(cl);
		return NULL;
	}
	
	/* si estamos aca es porque SI hay conexiones! */
	result = new list<struct hci_conn_info*>();
	if (!result){
		debugp("No hay memoria para crear la lista\n");
		free(cl);
		return NULL;
	}
	
	
	for (i = 0; i < cl->conn_num; i++, ci++) {
		conInfo = (struct hci_conn_info *) malloc(sizeof(*conInfo));
		if (!conInfo) {
			debugp("Error al pedir memoria para hci_conn_info\n");
			continue;
		}
		/* copiamos los datos del connection_info */
		memcpy(conInfo, ci, sizeof(*conInfo));
		/* agregamos a la lista */
		result->push_front(conInfo);
		conInfo = NULL;
	}
	/* liberamos la memoria */
	free(cl);
	
	return result;
}

/* Funcion que va a forzar el cierre de una conexion con un
* dispositivo determinado.
* REQUIRES:
* 	baDst		!= NULL
* 	reason		(Reason close code)
* RETURENS;
* 	>= 0 	on success
* 	<  0	on error
*/
int BTDongleDevice::closePhysicalConnection(bdaddr_t * baDst, uint8_t reason)
{
	list<struct hci_conn_info*> *conList = NULL;
	list<struct hci_conn_info*>::iterator it;
	int result = 0;
	
	if (!baDst) {
		debugp("Nos pasaron una baDst NULL para cerrar\n");
		return -1;
	}
	
	/* primero obtenemos la lista de conexiones que tiene el dongle y
	 * vamos a buscar en la lista si tenemos o no una conexion con dir 
	 * baDst */
	conList = getPhysicalConnections();
	if (!conList) {
		return -1;
	}
	
	for (it = conList->begin(); it != conList->end(); ++it) {
		if(!(*it))
			continue;
		/* verificamos si la bdaddr == baDst */
		if (bacmp(&(*it)->bdaddr, baDst) == 0) {
			/* son iguales ==> cerramos la conexion y salimos */
			result = disconnectPhyCon((*it)->handle, reason);
			/*! break; Si sacamos este break lo que conseguimos
			 * es poder desconectar TODAS las conexiones asociadas
			 * a una mac determinada, que en fin es lo que queremos
			 */
		}
	}
	/* eliminamos la lista y todos los elementos */
	for (it = conList->begin(); it != conList->end(); ++it) {
		if (*it)
			free(*it);
	}
	conList->clear();
	delete conList;
	
	return result;
}
	

int BTDongleDevice::closePhysicalConnection(struct hci_conn_info *dst, 
					     uint8_t reason)
{
	if (!dst)
		return -1;
	
	return disconnectPhyCon(dst->handle, reason);
}
int BTDongleDevice::closePhysicalConnection(uint16_t handle, uint8_t reason)
{
	return disconnectPhyCon(handle, reason);
}

/* Funcion que hace un escaneo de dispositivos devolviendo
* una lista de bdaddr_t * (de macs) las cuales tienen
* que ser liberadas una por una incluyendo la lista.
* RETURN:
* 	list * of mac	if no error
* 	NULL		on error
* NOTE: genera memoria para cada bdaddr_t y la lista.
*/
list<bdaddr_t *> * BTDongleDevice::scanNearbyDevices(void)
{
	list<bdaddr_t *> *result = NULL;
	inquiry_info *devices = NULL;
	int maxRsp = 0, numRsp = 0, len = 0, flags = 0;
	int i = 0;
	bdaddr_t *mac = NULL;
	
	len = 8 ; /*! tiempo a esperar (1.28 secs * len) */
	maxRsp = 255; /*! max cant de dispositivos */
	/*! limpiamos el cache */
	flags = IREQ_CACHE_FLUSH ; 
	devices = (inquiry_info *) malloc(maxRsp * sizeof(inquiry_info));
	numRsp = hci_inquiry((int) this->devID, len, maxRsp, NULL, &devices,
                 flags);
	if ( numRsp < 0 ) {
		perror(" hci inquiry");
		free(devices);
		return NULL;
	}
	/* generamos la lista */
	result = new list<bdaddr_t *>();
	if (!result) {
		debugp("error al crear la lista\n");
		free(devices);
		return NULL;
	}
  
	for (i = 0; i < numRsp; i++) {
		mac = (bdaddr_t *) malloc(sizeof(*mac));
		if (!mac) {
			debugp("Error pidiendo memoria para la mac\n");
			continue;
		}
		bacpy(mac, &(devices+i)->bdaddr);
		/* ahora agregamos a la lista la mac */
		result->push_front(mac);
		mac = NULL;
	}
	
	free(devices);
	return result;
}

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
list<string*> * BTDongleDevice::getFriendlyNames(list<bdaddr_t *> *macList)
{
	list<string*> *result = NULL;
	list<bdaddr_t*>::iterator it;
	string * fName = NULL;
	char name[248];
	
	if(macList == NULL || macList->size() == 0) {
		debugp("macList NULL\n");
		return NULL;
	}
	
	result = new list<string*>();
	if(!result) {
		debugp("No se pudo generar la lista\n");
		return NULL;
	}
	
	for (it = macList->begin(); it != macList->end(); ++it){
		if (!(*it))
			continue;
		
		/* inicializamos */
		fName = NULL;
		memset(name, '\0', 248);
		
		hci_read_remote_name(this->sock, (*it), 248, name, 0);
		/* vamos a agregar un string null si hubo error para mantener
		 * el mapeo */
		fName = new string(name);
		result->push_front(fName);
	}
	
	return result;
}

/*! 			SDP SERVER			*/

/* Funcion que devuelve una lista con las sessiones al servidor
* SDP del dongle actual.
* RETURNS:
* 	list<const BTSDPSessionData*> *list
* 	NULL	on error
*/
const list<BTSDPSessionData*>* BTDongleDevice::getSDPSessions(void)
{
	return &this->sdpList;
}

/* Funcion que elimina un servicio (SDP) determinado,
* REQUIRES:
* 	btS 	!= NULL
* 	btS € this.btSDPSessionList
* RETURNS:
* 	true 	on success (==> free(btS))
* 	false	otherwise (==> !free(btS))
* NOTE: btS es eliminado luego de esta llamada.
*/
bool BTDongleDevice::removeSDPSession(BTSDPSessionData *btS)
{
	list<BTSDPSessionData*>::iterator it;
	bool found = false;
	
	if (!btS)
		return false;
	
	/* verificamos que la tengamos en este btS en la lista sdpList */
	for (it = this->sdpList.begin(); it != this->sdpList.end(); ++it) {
		if ((*it) && (*it) == btS) {
			found = true;
			break;
		}
	}
	/* vemos si encontramos */
	if (!found)
		return false;
	
	/* si lo encontramos ==> eliminamos y destruimos */
	destroySDPService(btS);
	this->sdpList.remove(btS);
	delete btS;
	
	return true;
}

/* Funcion que inserta un nuevo servicio al SDP local, si y solo
* si este no se encuentra ya corriendo en el server.
* REQUIRES:
* 	btS 	!= NULL
* RETURNS:
* 	>= 0	on succes
* 	< 0	on error
* NOTE: No debe ser eliminado btS luego de haber llamado a esta
* 	 funcion.
*/
int BTDongleDevice::addSDPSession(BTSDPSessionData *btS)
{
	list<BTSDPSessionData*>::iterator it;
	int result = -1;
	bool found = false;
	
	if (!btS) {
		debugp("recibimos un btS para agregar null\n");
		return result;
	}
	
	/* primero buscamos que no se encuentre ese servicio segun el uuid */
	for (it = this->sdpList.begin(); it != this->sdpList.end(); ++it) {
		if (!(*it))
			continue;
		/* comparamos */
		if(sdp_uuid128_cmp(&(*it)->svc_uuid, &btS->svc_uuid) == 0) {
			/* ya existe entonces no lo agrgamos */
			found = true;
			break;
		}
	}
	if (found) {
		debugp("El servicio ya existe corriendo en el SDP "
		"Server..\n");
		return -1;
	}
		
	/* si estamos aca es porque no lo encontramos ==> lo agregamos */
	if (createSDPService(btS) < 0) {
		/* :( no se pudo crear... devolvemos error */
		result = -1;
	} else {
		/* si pudo crearse => lo agrgamos a la lista */
		this->sdpList.push_front(btS);
		result = 0;
	}
	
	return result;
}

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
int BTDongleDevice::sendCmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param)
{
	return hci_send_cmd(this->sock, ogf, ocf, plen, param);
}

/* Funcion que realiza un request al dipositivo.
* RETURNS:
* 	codeErr (ver hci.h/hci_lib.h)
*/
int BTDongleDevice::sendReq(struct hci_request *req, int timeout)
{
	return hci_send_req(this->sock, req, timeout);
}

/* Destructor.
* Se desconecta del dispositivo y elimina todo los datos
*/
BTDongleDevice::~BTDongleDevice(void)
{
	list<BTSDPSessionData*>::iterator it;
	
	/* cerramos la conexion con el microcontrolador */
	close(sock);
	/* cerramos cada conexion */
	for (it = this->sdpList.begin(); it != this->sdpList.end(); ++it) {
		destroySDPService(*it);
		delete(*it);
	}
	this->sdpList.erase(this->sdpList.begin(), this->sdpList.end());
	
	
}