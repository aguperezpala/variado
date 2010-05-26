#include "file_aux.h"

/* Funcion que abre para escritura un archivo (reemplazandolo si ya existe,
* o creandolo si no existe), e inserta datos.
* NOTE: fn = nombre de archivo con el path incluido
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int filea_create_w(string &fn, string &data)
{
	ofstream outfile (fn.c_str(), ofstream::binary);
	
	
	if(!outfile.good())
		return -1;
	
	// write to outfile
	outfile.write (data.c_str(),data.size());
	
	if (outfile.fail()){
		outfile.close();
		return -1;
	} 
	
	outfile.close();
	return 0;
}



/* Funcion que lee un archivo completo y devuelve en un string el contenido
* del archivo.
* REQUIRES:
* 	fname != NULL
* RETURNS:
* 	string	!= NULL	if success
* 	NULL		othereise
* NOTE: Genera memoria
*/
string *parser_read_all_file(const char *fname)
{
	char *buffer;
	ifstream is;
	string *result = NULL;
	int fLength = 0;
	
	assert(fname != NULL);
	
	/* abrimos el archivo y lo leemos completo */
	is.open (fname, ios::binary );
	if (!is.good()) {
		cerr << "Error al abrir el archivo\n";
		return NULL;
	}
	
	// obtenemos el tamaño y lemos todo
	is.seekg (0, ios::end);
	fLength = is.tellg();
	is.seekg (0, ios::beg);
	
	// pedimos memoria
	buffer = new char [fLength];	
	// leemos
	is.read (buffer,fLength);
	if (is.fail()) {
		cerr << "error al leer el archivo\n";
		delete[] buffer;
		is.close();
		return NULL;
	}
	
	
	is.close();
	/*! FIXME: ineficiente! */
	result = new string(buffer, fLength);
	delete[] buffer;
	
	return result;
}



