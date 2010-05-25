#include "parser.h"

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
int parser_search_key(string &data, string &key, string &value)
{
	int pos = data.find(key);
	int assignPos = 0;
	int sepPos = 0;
	int i = 0;
	string blanks = PARSER_BLANKS;
	
	if (pos < 0)
		return pos;
	/* si lo encontramos, ahora buscamos el valor y lo extraemos */
	assignPos = data.find(VALUE_ASSIGN, pos);
	if (assignPos < 0) {
		cerr << "no se encontro assignPos " << key << " \n";
		return -1;
	}
	
	/* debemos ver que no hay ningun otro caracter entre medio */
	i = pos + key.size();
	while (i < assignPos) {
		if((int)blanks.find(data[i]) >= 0)
			i++;
		else
			break;
	}
	
	if(i < assignPos)
		/* hay otra cosa rara...*/
		return -1;
	
	
	sepPos = data.find(VALUE_SEPARATOR, assignPos); 
	if (sepPos < 0) {
		cerr << "no se encontro sepPos " << key << " \n";
		return -1;
	}
	/* extraemos en teoria el valor y devolvemos */
	value = data.substr(assignPos + 1, sepPos - assignPos - 1);
	
	
	return 0;
}

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
string *parse_word(string &data, uint32_t from, const char *charsTo)
{
	string *result = NULL;
	string aux = charsTo;
	uint32_t to = 0;
	uint32_t size = data.size();
	
	assert(charsTo != NULL);
	assert(from >= 0);
	assert(from <= data.size());
	
	while (from < size)
		if((int)aux.find(data[from]) >= 0)
			from++;
		else
			break;
		
	if (from == size)
		/* no hay palabra */
		return NULL;
	
	/* aca en from tenemos la posicion tal que es un caracter que no
	* pertenece a charsTo. ahora debemos buscar la posicion final
	*/
	to = from;
	while (to < size)
		if((int)aux.find(data[to]) < 0)
			to++;
		else
			break;
	
	result = new string(data.substr(from, to-from));
	
	return result;
}

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
				   string &openComment, string &closeComment)
{
	string *result = NULL;
	int pos = 0, endPos = 0;
	
	
	if (from >= (int)data.size()){
		from = -1;
		return result;
	}
	
	/* buscamos la posicion donde comienza el comentario */
	pos = data.find(openComment, from);
	from = -1;
	if (pos < 0)
		return result;
	
	endPos = data.find(closeComment, pos + 1);
	if (endPos < 0)
		return result;
	
	from = endPos + closeComment.size();
	result = new string(data.substr(pos, endPos-pos + closeComment.size()));
	
	return result;
}


