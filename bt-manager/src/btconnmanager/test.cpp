#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <stdio.h>
#include <assert.h>
/* libs de bluetooth */
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>



#include "btconnection.h"
#include "btdmanager.h"
#include "btdongledevice.h"
#include "btconnmanager.h"
#include "tester.h"

sdp_record_t record = { 0 };
sdp_session_t * register_service(bdaddr_t &localDevice)
{
	uint32_t svc_uuid_int[] = { 0 , 0 , 0 , 0x0003 };
	uint8_t rfcomm_channel = 11;
	const char * service_name = "The Joystick Server" ;
	const char * svc_dsc = "Experimento para testear joysticks" ;
	const char * service_prov = "Joystick-Server" ;
	uuid_t root_uuid , l2cap_uuid , rfcomm_uuid , svc_uuid, svc_class_uuid ;
	sdp_list_t * l2cap_list = 0 ,
	* rfcomm_list = 0 ,
	* root_list = 0 ,
	* proto_list = 0 ,
	* access_proto_list = 0 ,
	* svc_class_list = 0 ,
	* profile_list = 0 ;
	
	sdp_data_t * channel = 0 ;
	sdp_profile_desc_t profile ;
	sdp_session_t * session = 0 ;

	
	// set the general service ID
	sdp_uuid128_create( &svc_uuid , &svc_uuid_int ) ;
	sdp_set_service_id ( &record , svc_uuid ) ;
	
	// set the service class
	sdp_uuid16_create(&svc_class_uuid , SERIAL_PORT_SVCLASS_ID ) ;
	svc_class_list = sdp_list_append( 0 , &svc_class_uuid ) ;
	sdp_set_service_classes(&record , svc_class_list ) ;
	
	// set the Bluetooth profile information
	sdp_uuid16_create(&profile.uuid , SERIAL_PORT_PROFILE_ID ) ;
	profile.version = 0x0100 ;
	profile_list = sdp_list_append( 0 , &profile ) ;
	sdp_set_profile_descs (&record , profile_list ) ;
	
	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid , PUBLIC_BROWSE_GROUP ) ;
	root_list = sdp_list_append ( 0 , &root_uuid ) ;
	sdp_set_browse_groups ( &record , root_list ) ;
	
	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid , L2CAP_UUID ) ;
	l2cap_list = sdp_list_append ( 0 , &l2cap_uuid ) ;
	proto_list = sdp_list_append ( 0 , l2cap_list ) ;
	
	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&rfcomm_uuid , RFCOMM_UUID ) ;
	channel = sdp_data_alloc ( SDP_UINT8 , &rfcomm_channel ) ;
	rfcomm_list = sdp_list_append ( 0 , &rfcomm_uuid ) ;
	sdp_list_append ( rfcomm_list , channel ) ;
	sdp_list_append ( proto_list , rfcomm_list ) ;
	access_proto_list = sdp_list_append ( 0 , proto_list ) ;
	sdp_set_access_protos ( &record , access_proto_list ) ;
	
	// set the name, provider, and description
	sdp_set_info_attr (&record , service_name , service_prov , svc_dsc ) ;
	
	// connect to the local SDP server, register the service record,
	
	// and disconnect
	session = sdp_connect(&localDevice , BDADDR_LOCAL , SDP_RETRY_IF_BUSY );
	//session = sdp_connect(&localDevice , BDADDR_LOCAL , SDP_RETRY_IF_BUSY );
	if (session == NULL){
		perror("Error al conectarse con el sdp\n");
		return NULL;
	}
	sdp_record_register ( session , &record , 0 ) ;
	// cleanup
	sdp_data_free (channel ) ;
	sdp_list_free (l2cap_list , 0 ) ;
	sdp_list_free (rfcomm_list , 0 ) ;
	sdp_list_free (root_list , 0 ) ;
	sdp_list_free (proto_list ,0);
	sdp_list_free (access_proto_list , 0 ) ;
	sdp_list_free (svc_class_list , 0 ) ;
	sdp_list_free (profile_list , 0 ) ;
	
	return session ;
}



/* funcion que obtiene el primer mac del un dogle del sistema 
 * RETURNS:
 * 	< 0	if error
 * 	0	on success
*/
static int get_some_dongle_mac(bdaddr_t &mac)
{
	BTDManager bdm;
	list<BTDongleDevice *> *dongles = NULL;
	
	dongles = bdm.getListDevices(0);
	if (dongles == NULL) {
		return -1;
	}
	
	/* simplemente obtenemos el primero */
	bacpy(&mac, dongles->front()->getMac());
	delete dongles;
	
	return 0;
}

/* Funcion que genera un server, le hace el bind y lo pone a escuchar, devuelve
 * el codigo de error */
int create_server(BTSimpleServer *server)
{
	assert(server != NULL);
	
	if (server->bindSocket() < 0)
		return -1;
	return server->startListen();
}



int main (void)
{
	BTConnManager btcm;
	BTConnection *con = NULL;
	bdaddr_t mac;
	char strCon[30] = {0};
	int result = 0;
	eventType_t evT;
	string sData = "";
	sdp_session_t * session = NULL;
	BTSimpleServer *server = NULL;
	bool fail = false;
	
	
	session = register_service(mac);
	/* obtenemos alguna mac */
	get_some_dongle_mac(mac);
	
	/* creamos el server */
	server = new BTSimpleServer(&mac, 7, 11);
	assert(server != NULL);
	if (create_server(server) < 0) {
		cerr << "error al crear el server \n";
		exit(1);
	}
	
	/* agregamos al manager */
	btcm.insertServer(server);
	
	while (!fail) {
		con = btcm.getConnEvent(evT, result);
		if (con != NULL)
			ba2str(con->getMacDest(), strCon);
		switch(evT) {
			case BTCM_EV_NEW_CONN:
				cout << "eventType: BTCM_EV_NEW_CONN\n";
				if (con == NULL){
					cout << "nueva conexion NULA \n";
					continue;
				}
				/* imprimimos la conexion */
				cout << "nueva conexion recibida: " << strCon << endl;
				
				/* mandamos la lista de servers */ 
				sData = "resp:18:00:1A:7D:01:54:6A,";
				//con->sendData(sData);
				con->getSendBuff() += sData;
				btcm.setFlags(con,BTCM_POLL_OUT_FLAGS);
				break;
				
			case BTCM_EV_OUT:
				cout << "eventType: BTCM_EV_OUT\n";
				assert(con != NULL);
				if (result < 0) {
					btcm.removeConnection(con);
					delete con;
				}
					
				break;
				
			case BTCM_EV_CLOSE_CONN:
				cout << "eventType: BTCM_EV_CLOSE_CONN\n";
				/* debemos eliminarla del arreglo */
				assert(con != NULL);
				cout << "recibimos: " << con->getRcvBuff() << endl;
				if((int)con->getRcvBuff().find("fail") >= 0)
					fail = true;
				btcm.removeConnection(con);
				delete con;
				break;
				
			case BTCM_EV_RCV:
				cout << "eventType: BTCM_EV_RCV\n";
				assert(con != NULL);
				cout << "Recibimos de: " << strCon << "\ndatos: ";
				if((int)con->getRcvBuff().find("fail") >= 0){
					fail = true;
					con->closeConnection();
					btcm.removeConnection(con);
					delete con;
				}
				cout << con->getRcvBuff() << endl;
				con->clearRecvBuffer();
				break;
				
			case BTCM_EV_SERVER_ERR:
				cout << "eventType: BTCM_EV_SERVER_ERR\n";
				/*! deberiamos eliminar el server... */
				fail = true;
				break;
				
			case BTCM_EV_ACCEPT_ERR:
				cout << "eventType: BTCM_EV_ACCEPT_ERR\n";
				/* error al aceptar?... */
				break;
				
			case BTCM_EV_ERR:
				cout << "eventType: BTCM_EV_ERR\n";
				/*...*/
				fail = true;
				break;
				
			default:
				assert(false);
				break;
		}
	}
	
	sdp_close(session);
	delete server;
	return 0;
}



