#include "btsdpsessiondata.h"


void BTSDPSessionData::initialize(void)
{
	this->rfcomm_channel = 11;
	this->serviceName = NULL;
	this->serviceDsc = NULL;
	this->serviceProv = NULL;
	this->l2cap_list = NULL;
	this->rfcomm_list = NULL;
	this->root_list = NULL;
	this->proto_list = NULL;
	this->access_proto_list = NULL;
	this->svc_class_list = NULL;
	this->profile_list = NULL;
	this->channel = NULL;
	memset(&this->profile,'\0', sizeof(this->profile));
	this->session = NULL;
	memset(&this->record, '\0', sizeof(this->record));
	
}


/* constructor */
BTSDPSessionData::BTSDPSessionData(uint32_t* uuid128, uint8_t rfcommChannel)
{
	initialize();
	sdp_uuid128_create(&this->svc_uuid, uuid128);
	this->rfcomm_channel = rfcommChannel;
}

/* Constructor que por defecto setea los siguietnes valores:
	* uuid128 = { 0 , 0 , 0 , 0x0003 }
	* rfcommChannel = 11
	*/
BTSDPSessionData::BTSDPSessionData(void)
{
	initialize();
	uint32_t svc_uuid_int[] = { 0 , 0 , 0 , 0x0003 };
	sdp_uuid128_create(&this->svc_uuid, svc_uuid_int);
	this->rfcomm_channel = 11;
}

/* Funcion que setea el nombre del servicio, la descripcion del
	* mismo y que servicio provee.
	* REQUIRES:
	* 	ns = Name service
	* 	ds = Description Service
	* 	sp = Service provided
	* NOTE: deben ser borrados los nombres luego
	*/
void BTSDPSessionData::setDescServInfo(const string& ns, const string& ds, 
			const string& sp)
{
	if (this->serviceName)
		free(this->serviceName);
	
	this->serviceName = (char *) malloc(ns.size()*sizeof(char) + 1);
	if (!this->serviceName){
		debugp("no hay memoria\n")
		return;
	}
	
	if (this->serviceDsc)
		free(this->serviceDsc);
	this->serviceDsc = (char *) malloc(ds.size()*sizeof(char) + 1);
	if (!this->serviceDsc){
		free(this->serviceName);
		debugp("no hay memoria\n")
		return;
	}
	
	if (this->serviceProv)
		free(this->serviceProv);
	this->serviceProv = (char *) malloc(sp.size()*sizeof(char) + 1);
	if (!this->serviceProv) {
		free(this->serviceName);
		free(this->serviceDsc);
		debugp("no hay memoria\n")
		return;
	}
	memset(this->serviceName, '\0', ns.size() + 1);
	memcpy(this->serviceName, ns.c_str(), ns.size());
	
	memset(this->serviceDsc, '\0', ds.size() + 1);
	memcpy(this->serviceDsc, ds.c_str(), ds.size());
	
	memset(this->serviceProv, '\0', sp.size() + 1);
	memcpy(this->serviceProv, sp.c_str(), sp.size());
	
}



BTSDPSessionData::~BTSDPSessionData(void)
{
	if (this->serviceName)
		free(this->serviceName);
	if (this->serviceDsc)
		free(this->serviceDsc);
	if (this->serviceProv)
		free(this->serviceProv);
	
}

