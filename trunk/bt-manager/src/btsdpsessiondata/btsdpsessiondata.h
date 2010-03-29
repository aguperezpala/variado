/*! Representa a un usb dongle bluetooth.
 * Clase encargada de mantener los datos especificos para mantener una 
 * determinada conexion con el servidor SDP
 * Simplemente es un contenedor de datos
 */  
#ifndef BTSDPSESSIONDATA_H
#define BTSDPSESSIONDATA_H

#include <iostream>
#include <string>
#include <list>
/* libs de bluetooth */
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include <stdlib.h>
#include "../debug.h"


using namespace::std;

class BTSDPSessionData {
	
	public:
		/* constructor */
		BTSDPSessionData(uint32_t* uuid128, uint8_t rfcommChannel);
		
		/* Constructor que por defecto setea los siguietnes valores:
		 * uuid128 = { 0 , 0 , 0 , 0x0003 }
		 * rfcommChannel = 11
		 */
		BTSDPSessionData(void);
		
		/* Funcion que setea el nombre del servicio, la descripcion del
		 * mismo y que servicio provee.
		 * REQUIRES:
		 * 	ns = Name service
		 * 	ds = Description Service
		 * 	sp = Service provided
		 * NOTE: deben ser borrados los nombres luego
		 */
		void setDescServInfo(const string& ns, const string& ds, 
				      const string& sp);

		
		
		~BTSDPSessionData(void);
	
	/*! vamos hacer todos los argumentos publicos! */
	/* 			Argumentos 				*/
		uint8_t rfcomm_channel;
		char *serviceName;
		char *serviceDsc;
		char *serviceProv;
		uuid_t root_uuid , l2cap_uuid , rfcomm_uuid , svc_uuid, svc_class_uuid ;
		sdp_list_t * l2cap_list,
		* rfcomm_list,
		* root_list,
		* proto_list,
		* access_proto_list,
		* svc_class_list,
		* profile_list;
		sdp_data_t * channel;
		sdp_profile_desc_t profile;
		sdp_session_t * session ;
		sdp_record_t *record;
	
	private:
		void initialize(void);
};



#endif
