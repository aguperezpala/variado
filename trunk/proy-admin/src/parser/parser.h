#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <assert.h>


/*! vamos a definir las palabras claves para buscar los pesos y esas cosas */
#define MODULE_WEIGHT	"MODULE_WEIGHT"
#define FUNC_COMPLETED	"FUNC_COMPLETED"
#define FUNC_TESTED	"FUNC_TESTED"
#define FUNC_WEIGHT	"FUNCTION_WEIGHT"
#define MODULE_TESTED	"MODULE_TESTED"

#define VALUE_SEPARATOR ";"
#define VALUE_ASSIGN	"="
#define PARSER_BLANKS	"\n\t "


using namespace std;

/*! Funcion que devuelve un valor determinado segun un nombre de una KEY
* determinada. 
* NOTE: Usa VALUE_SEPARATARO VALUE_ASSIGN & PARSER_BLANKS
* REQUIRES:
* 	data	where search
* 	key	to find
* 	value	to fill
* RETURNS:
* 	0 	if success
* 	< 0	otherwise
*/
int parser_search_key(string &data,string &key, string &value);

/*! funcion que parsea un value entre 2 elementos
* RETURNS:
*  	< 0	on error
*  	0	on success
*/
int getValue(string &d, int from, const char *beg, const char *end, string &value);

/*! Funcion que extrae una palabra desde una posicion determinada (from) 
*  saltiandose todos los caracteres pertenecientes a charsTo y luego
*  tomando todos aquellos caracteres hasta encontrar nuevamente otro € chartsTo
* REQUIRES:
* 	from	<= data.size()
*	charsTo	!= NULL
* RETURNS:
* 	word	!= NULL if success
* 	NULL	otherwise
* NOTE: genera memoria
*/
string *parse_word(string &data, uint32_t from, const char *charsTo);


/*! Funcion que parsea un comentario devolviendolo en un string, buscando
* desde una posicion determinada, con 2 strings necesarios, uno para 
* especificar como comienza un comentario, y el otro determinando como
* termina el comentario.
* NOTE: devuelve el comentario sin los caracteres de comentarios
* REQUIRES:
* 	from <= data.size()
* 	openComment != NULL
* 	closeComment != NULL
* RETURNS:
* 	NULL		if cant find or error
* 	comment		otherwise
* 	from		devuelve la posicion donde termina el comentario
*/
string *parser_get_comment(string &data, int &from, 
			    string &openComment, string &closeComment);


/*! funcion que saltea los caracteres cs y devuelve la posicion luego de saltear
 * los caracteres especificados
 * REQUIRES:
 * 	cs != NULL
 * RETURNS:
 * 	< 0	on error
 * 	pos	otherwise
 */
int parser_jump_chars(string &d, int from, const char* cs);

#endif
