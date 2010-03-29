#include "btsdpsessiondata.h"
#include <iostream>

int main (void)
{
	BTSDPSessionData d;
	string name = "esto es nombre";
	string desc = "descripcionnnn";
	string sp = "servicio proveido es jjj";
	
	
	d.setDescServInfo(name,desc,sp);
	printf("\n\nname:%s\ndesc:%s\nsp:%s\n",d.serviceName, d.serviceDsc, 
		d.serviceProv);
	
	cout << "Esto es una prueba" << endl;
	return 0;
}