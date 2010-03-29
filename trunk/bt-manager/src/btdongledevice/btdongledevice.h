/*! Representa a un usb dongle bluetooth.
* Enviar comandos de configuracion (setear nombre, tipo de authenticacion, 
* encriptacion,  seguridad y otras cosas).
* Enviar requests (al dispositivo).
* Chequear informacion de estado.
* Realizar scaneo de dispositivos.
* Manejar eventos.
* Manejar SDP_SERVER
* NOTE: deberiamos revisar si es mejor mantener siempre abierta la conexion
* (socket) al dispositivo, o cerrarla y abrirla cuando sea necesaria..
 */  
#ifndef BTDONGLEDEVICE_H
#define BTDONGLEDEVICE_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
/* libs propias */
#include "../btsdpsessiondata/btsdpsessiondata.h"


using namespace::std;



class BTDongleDevice {
	
	public:
		/*! Ambos constructores establecen automaticamente una conexion
		 * con el dipositivo automaticamente */
		
		/* Constructor: Toma un dev_id */
		BTDongleDevice(uint16_t dev_id);
		
		/* Constructor opcional, tomando una bdaddr_t del device */
		BTDongleDevice(const bdaddr_t *devMac);
		
		/* Funcion que establece la conexion con el dispositivo
		 * RETURNS:
		 * 	codeError
		 */
		int connectToDevice(void);
		
		/* Funcion que devuelve el ID del dispositivo */
		uint16_t getID(void);
		
		/* Funcion que devuelve la MAC del dispositivo 
		 * RETURNS:
		 * 	bdaddr_t *	
		 * 	NULL 		if error
		 */
		const bdaddr_t * getMac(void);
		
		/* Funcion que devuelve el socket de la conexion establecida
		 * con el device.
		 */
		int getSocket(void);
		
		/* Funcion que devuelve un string con informacion (parseada) 
		 * sobre el dispositivo. 
		 * NOTE: genera memoria
		 */
		string * getInfo(void);
		
		/* Devuelve el estado en el que se encuentra el dispositivo 
		 * FIXME: verificar donde se puede obtener el estado 
		 */
		int getStatus(void);
		
		/* Funcion que devuelve el nombre del dispositivo.
		 * NOTE: Eliminar luego el nombre (genera memoria).
		 */
		string * getDevName(void);
		
		/* Funcion que setea el nombre al dispositivo local. 
		 * REQUIRES:
		 * 	name != NULL
		 */
		void setDevName(const string *name);
		
		/* Funcion que obtiene una lista de conexiones establecidas con
		 * el dispositivo. Son las conexiones de bajo "nivel" (physical
		 * links).
		 * RETURNS:
		 * 	list<hci_conn_info *> *		on success
		 *	NULL				if no conn or error.
		 * NOTE: se genera memoria para cada hci_conn_info y para
		 * 	 la lista. (veer hci,h para la struct hci_conn_info).
		 */
		list<struct hci_conn_info *> * getPhysicalConnections(void);
		
		/* Funcion que va a forzar el cierre de una conexion con un
		 * dispositivo determinado.
		 * REQUIRES:
		 * 	baDst		!= NULL
		 * 	reason		(Reason close code)
		 * RETURENS;
		 * 	>= 0 	on success
		 * 	<  0	on error
		 */
		int closePhysicalConnection(bdaddr_t * baDst, uint8_t reason);
		int closePhysicalConnection(struct hci_conn_info *dst, 
					     uint8_t reason);
		int closePhysicalConnection(uint16_t handle, uint8_t reason);
		
		
		/* Funcion que hace un escaneo de dispositivos devolviendo
		 * una lista de bdaddr_t * (de macs) las cuales tienen
		 * que ser liberadas una por una incluyendo la lista.
		 * RETURN:
		 * 	list * of mac	if no error
		 * 	NULL		on error
		 * NOTE: genera memoria para cada bdaddr_t y la lista.
		 */
		list<bdaddr_t *> * scanNearbyDevices(void);
		
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
		list<string*> * getFriendlyNames(list<bdaddr_t *> *macList);
		
		/*! 			SDP SERVER			*/
		
		/* Funcion que devuelve una lista con las sessiones al servidor
		 * SDP del dongle actual.
		 * RETURNS:
		 * 	list<const BTSDPSessionData*> *list
		 * 	NULL	on error
		*/
		const list<const BTSDPSessionData*>& getSDPSessions(void);
		
		/* Funcion que elimina un servicio (SDP) determinado,
		 * REQUIRES:
		 * 	btS 	!= NULL
		 * 	btS € this.btSDPSessionList
		 * RETURNS:
		 * 	true 	on success (==> free(btS))
		 * 	false	otherwise (==> !free(btS))
		 * NOTE: btS es eliminado luego de esta llamada.
		 */
		bool removeSDPSession(BTSDPSessionData *btS);
		
		/* Funcion que inserta un nuevo servicio al SDP local, si y solo
		 * si este no se encuentra ya corriendo en el server.
		 * (Comparamos los uuid)
		 * REQUIRES:
		 * 	btS 	!= NULL
		 * RETURNS:
		 * 	>= 0	on succes
		 * 	< 0	on error
		 * NOTE: No debe ser eliminado btS luego de haber llamado a esta
		 * 	 funcion.
		 */
		int addSDPSession(BTSDPSessionData *btS);
		 
		
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
		int sendCmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param);
		
		/* Funcion que realiza un request al dipositivo.
		 * RETURNS:
		 * 	codeErr (ver hci.h/hci_lib.h)
		 */
		int sendReq(struct hci_request *req, int timeout);
		
		/* Destructor.
		 * Se desconecta del dispositivo y elimina todo los datos
		 */
		~BTDongleDevice(void);
		
	
	private:
		/* Funciones */
		
		/* Funcion de inicializacion */
		void initalize(void);
		
		/* funcion que produce una conexion al dipositivo */
		int connectToDev(uint16_t dev_id);
		
		/* Funcion que transforma un struct hci_dev_info en un string
		 * generando memoria...
		 * RETURNS:
		 * 	string with info	if no error
		 * 	NULL			on error
		 */
		string * devInfoToStr(struct hci_dev_info * devInfo);
		
		/* funcion que cierra una conexion segun su identificador. y
		 * con un codigo de razon de cierre de conexion.
		 * RETURNS:
		 * 	< 0 	on error
		 * 	0	on success (ver hci_disconnect -hci_lib.h-) 
		 */
		 int disconnectPhyCon(uint16_t cHanndle, uint8_t reason);
		
		 /* Funcion que se conecta con el servidor SDP y establece
		  * un nuevo servicio devolviendo un codigo de error
		  * REQUIRES:
		  * 	sdpData 	!= NULL
		  * RETURNS:
		  * 	< 0		on error
		  * 	== 0		if success
		  */
		 int createSDPService(BTSDPSessionData *sdpData);
		 
		 /* Libera una conexion con el SDP server.
		  * REQUIRES:
		  * 	sdpData != NULL
		  * NOTE: no libera memoria del sdpData.
		  */
		 void destroySDPService(BTSDPSessionData *sdpData);
		
		/* Argumentos */
		
		/* socket para la conexion con el dispositivo */
		int sock;
		bdaddr_t mac;
		uint16_t devID;
		list<BTSDPSessionData*> sdpList;
		
};



#endif
