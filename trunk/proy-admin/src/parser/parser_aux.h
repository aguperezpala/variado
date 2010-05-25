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

/*! Funcion que va a tomar un nombre de archivo y lo que va hacer es 
 * devolver un modulo con el nombre, el peso del modulo y sus funciones.
 * NOTE: Fijarse que debe respetarse el formato de los modulos.
 *
 * REQUIRES:
 * 	fname	!= NULL
 * RETURNS:
 * 	errCode
 * NOTE: Genera memoria para cada funcion (si no hay error)
 */
int parse_file(string &fname, Module &m);








#endif
