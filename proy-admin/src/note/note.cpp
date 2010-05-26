#include "note.h"


/*! Genera una nota desde un string respetando el formato
* asignado para guardar las notas
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int Note::fromString(string &str)
{
	cout << "estamos parseando una nota: "<< str << endl;
	this->note = str;
	return 0;
}

/*! Convierte una nota en un string listo para ser guardada
* en un archivo.
* RETURNS:
*	NULL		if error
*	strNote		otherwise
* NOTE: Genera memoria
*/
string *Note::toString(void)
{	
	return new string(this->note);
}