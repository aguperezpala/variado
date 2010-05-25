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
int parser_search_key(string &data,string &key, string &value)
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
static string *parse_word(string &data, uint32_t from, const char *charsTo)
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
static string *parser_get_comment(string &data, int &from, 
				   string &openComment, string &closeComment)
{
	string *result = NULL;
	int pos = 0, endPos = 0;
	
	if (from < data.size())
		return result;
	
	/* buscamos la posicion donde comienza el comentario */
	pos = data.find(openComment, from);
	if (pos < 0)
		return result;
	
	endPos = data.find(closeComment, pos + 1);
	if (endPos < 0)
		return result;
	
	from = endPos;
	result = new string(data.substr(pos, endPos-pos + closeComment.size()));
	
	return result;
}


/* Funcion que busca un valor numerico (entero )
 * RETURNS:
 * 	< 0	on error
 *	0	if success
 */
static int parser_search_int_key(string &data,string &key, int &value)
{
	string aux = "";
	
	if (parser_search_key(data,key,aux) < 0)
		return -1;
	
	value = atoi(aux.c_str());
	
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
static string *parser_read_all_file(char *fname)
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
	result = new String(buffer, fLength);
	delete[] buffer;
	
	return result;
}

/* Funcion que se encarga de parsear o encontrar el nombre del modulo
 * RETURNS:
 * 	NULL			on error
 * 	modName != NULL		if success
 * 	type = (none = 0, 1 = normal_module, 2 = class)
 */
static string *parse_module_name(string &data, int &type)
{
	string *result = NULL;
	int pos = 0;
	
	
	type = 0;
	/* ahora vamos a ver si encontramos el nombre de una clase o no
	 * class | Class 
	 */
	pos = data.find("class");
	if (pos < 0)
		pos = data.find("Class");
	if (pos < 0){
		result = parse_word(data, 0, PARSER_BLANKS);
		if(result != NULL) {
			/* sacamos el _H */
			type = 1;
			int p = result->find("_H");
			if ((int)p >= 0)
				result->erase(pos, 2);
		}
		
		/* no es una clase */
		return result;
	}
	
	/* si estamos aca es porque entonces si hay un "class o un Class",
	 * DEBERIAMOS verificar que no este dentro de un comentario ni 
	 * nada por el estilo...
	 */
	result = parse_word(data, pos, PARSER_BLANKS "{:");
	type = 2;
	return result;
}


/* Funcion que va a parsear todas las funciones de un archivo, extrayendo
 * tanto el FUNC_COMPLETED como el FUNC_WEIGHT, y el nombre.
 * RETURNS:
 * 	list<Function *>*	if not error
 * 	NULL			on error
 * NOTE: genera memoria para cada funcion y para la lista
 */
static list<Function *> *parser_functions(string &data)
{
	list<Function *> *result = NULL;
	string *comment = NULL;
	int pos = 0;
	
	if (data.size() == 0)
		return result;
	
	result = new list<Function *>();
	
	while (pos >= 0) {
		
		pos = data.find(FUNC_COMPLETED
		
	result
	
}



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
int parse_file(string &fname, Module &m)
{
	string *fData = NULL;
	string *modName = NULL;
	string sAux = "";
	int iAux = 0;
	
	
	fData = parser_read_all_file(fname.c_str());
	if(fData == NULL) {
		return -1;
	}
	
	/* comenzamos a parsear */
	/* obtenemos el peso */
	sAux = MODULE_WEIGHT;
	if (parser_search_key(*fData, sAux, iAux) < 0) {
		delete fData;
		return -1;
	}
	if (iAux >= 0)
		/*! no tar que el peso es >= 0 */
		m.setWeight(iAux);
	else 
		cerr << "error al obtener el peso del modulo: " << iAux << endl;
	
	iAux = -1;
	/* obtenemos el tipo y el nombre del modulo */
	modName = parse_module_name(*fData, iAux);
	if (modName != NULL) {
		/* tenemos un nombre */
		m.setName(*modName);
		delete modName;
		/* seteamos el tipo */
		if (iAux == 1)
			m.setType(MODULE_T_MODULE);
		else if (iAux == 2)
			m.setType(MODULE_T_CLASS);
		else {
			cerr << "error al obtener el tipo de modulo: " << iAux;
			cerr << endl;
		}
	} else {
		/* no tenemos nombre de modulo, seteamos el nombre del archivo */
		cerr << "no se pudo encontrar nombre del modulo\n";
		m.setName(fname);
	}
	
	/* ahora obtenemos la lista de funciones */
	
	
	
}
