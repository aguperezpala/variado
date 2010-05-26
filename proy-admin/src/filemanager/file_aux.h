/*! Modulo encargado de manipular los archivos
 */
#ifndef FILE_MOD_AUX_H
#define FILE_MOD_AUX_H

#ifndef _BSD_SOURCE
	#define _BSD_SOURCE
#endif

#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;


/* Funcion que abre para escritura un archivo (reemplazandolo si ya existe,
 * o creandolo si no existe), e inserta datos.
 * NOTE: fn = nombre de archivo con el path incluido
 * RETURNS:
 * 	< 0	on error
 *	0	if success
 */
int filea_create_w(string &fn, string &data);


/* Funcion que lee un archivo completo y devuelve en un string el contenido
* del archivo.
* REQUIRES:
* 	fname != NULL
* RETURNS:
* 	string	!= NULL	if success
* 	NULL		othereise
* NOTE: Genera memoria
*/
string *parser_read_all_file(const char *fname);



#endif
