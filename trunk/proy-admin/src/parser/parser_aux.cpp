#include "parser_aux.h"


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
static string *parser_read_all_file(const char *fname)
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
		int auxP = data.find("#ifndef");
		
		if (auxP < 0)
			return result;
		/* salteamos el #ifndef */
		auxP += 8;
		
		result = parse_word(data, auxP, PARSER_BLANKS);
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
	Function *func = NULL;
	string *comment = NULL;
	string opCmt = PARSER_OPEN_COMMENT, cCmt = PARSER_CLOSE_COMMENT;
	string funCmp = FUNC_COMPLETED;
	string funWt = FUNC_WEIGHT;
	string *funcName = NULL,blanks = PARSER_BLANKS;
	int pos = 0;
	uint32_t upos = 0;
	int completed = 0, weight = 0;
	
	if (data.size() == 0)
		return result;
	
	result = new list<Function *>();
	if (result == NULL)
		return result;
	
	while (pos >= 0) {
		comment = parser_get_comment(data, pos, opCmt, cCmt);
		if (comment == NULL)
			/* no hay mas comentarios... salimos */
			break;
		
		/* buscamos el FUNC_COMPLETED */
		if ((parser_search_int_key(*comment, funCmp, completed) < 0) ||
			(completed > 100) || (completed < 0)) {
			/* no es un comentario valido.. */
			delete comment;
			continue;
		}
		/* extraemos el peso de la funcion */
		if ((parser_search_int_key(*comment, funWt, weight) < 0) ||
			(weight < 0)) {
			/* no es un comentario valido.. */
			delete comment;
			continue;
		}
		delete comment;
		/* salteamos los blancos */
		pos = pos + 1;
		upos = pos;
		while (upos < data.size())
			if((int)blanks.find(data[upos]) >= 0)
				upos++;
			else
				break;
		/* extraemos el nombre de la funcion ahora */
		funcName = parse_word(data, upos, "{;");
		if ((funcName == NULL) || (funcName->size() > 400)){
			/* error no estamos tomando una funcion */
			cerr << "Error al intentar obtener el prototipo de ";
			cerr << "la funcion, no estan respetando el formato?" << endl;
			continue;
		}
		/* si estamos aca entonces podemos decir que tenemos una func */
		func = new Function(*funcName, completed, weight);
		delete funcName;
		if (func == NULL){
			cerr << "no hay memoria para crear la funcion\n";
			break;
		}
		result->push_back(func);
	}
		
	return result;
	
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
	string sAux = "", sValue = "";
	int iAux = 0;
	list<Function *>* fList = NULL;
	list<Function *>::iterator it;
	
	
	fData = parser_read_all_file(fname.c_str());
	if(fData == NULL) {
		return -1;
	}
	m.setFileName(fname);
	
	/* comenzamos a parsear */
	/* obtenemos el peso */
	sAux = MODULE_WEIGHT;
	if (parser_search_int_key(*fData, sAux, iAux) < 0) {
		delete fData;
		return -1;
	}
	if (iAux >= 0)
		/*! no tar que el peso es >= 0 */
		m.setWeight(iAux);
	else 
		cerr << "error al obtener el peso del modulo: " << iAux << endl;
	
	/* ahora vamos a ver si esta testeado o no */
	sAux = MODULE_TESTED;
	if(parser_search_key(*fData, sAux, sValue) < 0) {
		cerr << "No se pudo determinar si el modulo fue testeado o no\n";
		m.setTested(false);
	} else {
		int i = 0;
		/* determinamos el valor de value */
		while (sValue[i])
		{
			sValue[i] = toupper(sValue[i]);
			i++;
		}
		cout << "sValue: " << sValue << endl;
		if((int)sValue.find("TRUE") >= 0)
			m.setTested(true);
		else if((int)sValue.find("FALSE") >= 0)
			m.setTested(false);
		else
			cerr << "MODULE_TESTED no respeta el formato \n";
	}
	
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
			m.setType(MODULE_T_UNKNOWN);
			cerr << "error al obtener el tipo de modulo: " << iAux;
			cerr << endl;
		}
	} else {
		/* no tenemos nombre de modulo, seteamos el nombre del archivo */
		cerr << "no se pudo encontrar nombre del modulo\n";
		m.setName(fname);
		m.setType(MODULE_T_UNKNOWN);
	}
	
	/* ahora obtenemos la lista de funciones */
	fList = parser_functions(*fData);
	if (fList == NULL) {
		cerr << "No se pudieron obtener funciones del modulo\n";
		delete fData;
		return -2;
	}
	/* agregamos las funciones al modulo */
	for (it = (*fList).begin(); it != (*fList).end(); ++it) {
		assert(*it != NULL);
		m.addFunction(*it);
	}
	delete fData;
	delete fList;
	
	return 0;
}