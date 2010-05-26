#include "function.h"


/* Constructor para setear todo de una */
Function::Function(string &n, int comp, int w)
{
	this->name = n;
	this->completed = comp;
	this->weight = w;
}

/*! Genera una funcion desde un string respetando el formato
* asignado para guardar las funciones
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int Function::fromString(string &str)
{	
	string value = "";
	
	
	if (getValue(str, 0,"<f-c>", "<f-w>", value) < 0)
		return -1;
	else {
		if (sizeof(this->completed) != value.size())
			return -1;
		memcpy(&this->completed, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<f-w>", "<f-n>", value) < 0)
		return -1;
	else {
		if (sizeof(this->weight) != value.size())
			return -1;
		/* debemos copiar lo que leimos en createdTime */
		memcpy(&this->weight, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<f-n>", "<f-t>", this->name) < 0)
		return -1;
	
	if (getValue(str, 0,"<f-t>", "<fun;>", value) < 0)
		return -1;
	else {
		if (sizeof(this->tested) != value.size())
			return -1;
			/* debemos copiar lo que leimos en createdTime */
			memcpy(&this->tested, value.c_str(), value.size());
	}
	
	return 0;
	/*<f-c>completed<f-w>weight<f-n>name<fun;>*/
	
}

/*! Convierte una funciones en un string listo para ser guardada
* en un archivo.
* RETURNS:
*	NULL		if error
*	funcStr		otherwise
* NOTE: Genera memoria
*/
string *Function::toString(void)
{
	string *result = new string();
	char buff[20] = {0};
	
	
	if (result == NULL)
		return result;
	
	result->append("<f-c>");
	memcpy(buff, &this->completed, sizeof(this->completed));
	result->append(buff, sizeof(this->completed));
	result->append("<f-w>");
	memcpy(buff, &this->weight, sizeof(this->weight));
	result->append(buff, sizeof(this->weight));
	result->append("<f-n>");
	result->append(this->name);
	result->append("<f-t>");
	memcpy(buff, &this->tested, sizeof(this->tested));
	result->append(buff, sizeof(this->tested));
	result->append("<fun;>");
	
	return result;
}

/* debug */
void Function::Print(void)
{
	cout << "\nFuncion Name " << this->name;
	cout << "\nFunction completed" << this->completed;
	cout << "\nFunction weight " << this->weight;
	cout << endl;	
}
