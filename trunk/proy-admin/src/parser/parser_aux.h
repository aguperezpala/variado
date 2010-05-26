#ifndef PARSER_AUX_H
#define PARSER_AUX_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "function.h"
#include "module.h"


#define PARSER_OPEN_COMMENT	"/*"
#define PARSER_CLOSE_COMMENT	"*/"

using namespace std;


/* Funcion que busca un valor numerico (entero )
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int parser_search_int_key(string &data,string &key, int &value);

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

/* Funcion que se encarga de parsear o encontrar el nombre del modulo
* RETURNS:
* 	NULL			on error
* 	modName != NULL		if success
* 	type = (none = 0, 1 = normal_module, 2 = class)
*/
string *parse_module_name(string &data, int &type);

/* Funcion que va a parsear todas las funciones de un archivo, extrayendo
* tanto el FUNC_COMPLETED como el FUNC_WEIGHT, y el nombre.
* RETURNS:
* 	list<Function *>*	if not error
* 	NULL			on error
* NOTE: genera memoria para cada funcion y para la lista
*/
list<Function *> *parser_functions(string &data);




#endif
