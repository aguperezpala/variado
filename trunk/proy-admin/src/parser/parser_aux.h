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
