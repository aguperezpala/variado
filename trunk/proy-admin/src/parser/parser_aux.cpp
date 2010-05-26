#include "parser_aux.h"

/* convierte a UPPERCASE todo un string */
static void convert_upper(string &sValue)
{
	int i = 0;
	/* determinamos el valor de value */
	while (sValue[i])
	{
		sValue[i] = toupper(sValue[i]);
		i++;
	}
}

/* Funcion que busca un valor numerico (entero )
 * RETURNS:
 * 	< 0	on error
 *	0	if success
 */
int parser_search_int_key(string &data,string &key, int &value)
{
	string aux = "";
	
	if (parser_search_key(data,key,aux) < 0)
		return -1;
	
	value = atoi(aux.c_str());
	
	return 0;
}


/* Funcion que se encarga de parsear o encontrar el nombre del modulo
 * RETURNS:
 * 	NULL			on error
 * 	modName != NULL		if success
 * 	type = (none = 0, 1 = normal_module, 2 = class)
 */
string *parse_module_name(string &data, int &type)
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
	result = parse_word(data, pos+6, PARSER_BLANKS "{:");
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
list<Function *> *parser_functions(string &data)
{
	list<Function *> *result = NULL;
	Function *func = NULL;
	string *comment = NULL;
	string opCmt = PARSER_OPEN_COMMENT, cCmt = PARSER_CLOSE_COMMENT;
	string funCmp = FUNC_COMPLETED;
	string funWt = FUNC_WEIGHT;
	string funTest = FUNC_TESTED;
	string aux = "";
	string *funcName = NULL,blanks = PARSER_BLANKS;
	int pos = 0;
	uint32_t upos = 0;
	int completed = 0, weight = 0;
	bool tested = false;
	
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
		if ((parser_search_key(*comment,funTest, aux) < 0)) {
			delete comment;
			continue;
		}
		/* verificamos si fue testeada la funcion */
		convert_upper(aux);
		if((int)aux.find("TRUE") >= 0)
			tested = true;
		else 
			tested = false;
		
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
		func->setTested(tested);
		result->push_back(func);
	}
		
	return result;
	
}


