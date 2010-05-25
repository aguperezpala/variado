#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <perror.h>

#include "function.h"
#include "module.h"

/*! vamos a definir las palabras claves para buscar los pesos y esas cosas */
#define MODULE_WEIGHT	"MODULE_WEIGHT"
#define FUNC_COMPLETED	"FUNC_COMPLETED"
#define FUNC_WEIGHT	"FUNCTION_WEIGHT"
#define VALUE_SEPARATOR ";"
#define VALUE_ASSIGN	"="
#define PARSER_BLANKS	"\n\t "


using namespace std;

/*! Funcion que devuelve un valor determinado segun un nombre de una KEY
* determinada. 
* REQUIRES:
* 	data	where search
* 	key	to find
* 	value	to fill
* RETURNS:
* 	0 	if success
* 	< 0	otherwise
*/
int parser_search_key(string &data,string &key, string &value)








#endif
