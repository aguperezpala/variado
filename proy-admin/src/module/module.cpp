#include "module.h"


/* Funcion que libera todas Funciones asociadas */
void Module::freeFunctions(void)
{
	list<Function *>::iterator fit;
	
	
	if (this->funcList.empty())
		return;
	
	for(fit = this->funcList.begin(); fit != this->funcList.end(); ++fit) {
		assert(*fit != NULL);
		delete (*fit);
	}
	

}

/* funcion que libera todas las notas asociadas */
void Module::freeNotes(void)
{
	list<Note *>::iterator nit;
	
	if(this->noteList.empty())
		return;
	
	for(nit = this->noteList.begin(); nit != this->noteList.end(); ++nit) {
		assert(*nit != NULL);
		delete (*nit);
	}
}

/* funcion que libera todas las tasks asociads */
void Module::freeTasks(void)
{
	list<Task *>::iterator tit;
	
	if (this->taskList.empty())
		return;
	
	for(tit = this->taskList.begin(); tit != this->taskList.end(); ++tit) {
		assert(*tit != NULL);
		delete (*tit);
	}
}



/* Funcion que lee el archivo asignado al modulo y extrae
* el nombre del modulo, funciones, etc, todo menos las notas
* y las tasks
* RETURNS:
* 	< 0	if error
*	0	on success
*/
int Module::loadFromFile(void)
{
	string *modName = NULL;
	string sAux = "", sValue = "";
	int iAux = 0;
	list<Function *>* fList = NULL;
	list<Function *>::iterator it;
	string *fData = NULL;
	
	/* liberamos toda la posible memoria que estemos usando */
	freeFunctions();
	
	
	fData = parser_read_all_file(this->fname.c_str());
	if (fData == NULL)
		return -1;
	/* comenzamos a parsear */
	/* obtenemos el peso */
	sAux = MODULE_WEIGHT;
	if (parser_search_int_key(*fData, sAux, iAux) < 0) {
		delete fData;
		return -1;
	}
	
	if (iAux >= 0)
		/*! no tar que el peso es >= 0 */
		this->weight = iAux;
	else 
		cerr << "error al obtener el peso del modulo: " << iAux << endl;
	
	/* ahora vamos a ver si esta testeado o no */
	sAux = MODULE_TESTED;
	if(parser_search_key(*fData, sAux, sValue) < 0) {
		cerr << "No se pudo determinar si el modulo fue testeado o no\n";
		this->tested = false;
	} else {
		int i = 0;
		/* determinamos el valor de value */
		while (sValue[i])
		{
			sValue[i] = toupper(sValue[i]);
			i++;
		}
		if((int)sValue.find("TRUE") >= 0)
			this->tested = true;
		else if((int)sValue.find("FALSE") >= 0)
			this->tested = false;
		else
			cerr << "MODULE_TESTED no respeta el formato \n";
	}
	
	iAux = -1;
	/* obtenemos el tipo y el nombre del modulo */
	modName = parse_module_name(*fData, iAux);
	if (modName != NULL) {
		/* tenemos un nombre */
		this->name = *modName;
		delete modName;
		/* seteamos el tipo */
		if (iAux == 1)
			this->type = MODULE_T_MODULE;
		else if (iAux == 2)
			this->type = MODULE_T_CLASS;
		else {
			this->type = MODULE_T_UNKNOWN;
			cerr << "error al obtener el tipo de modulo: " << iAux;
			cerr << endl;
		}
	} else {
		/* no tenemos nombre de modulo, seteamos el nombre del archivo */
		cerr << "no se pudo encontrar nombre del modulo\n";		
		this->type = MODULE_T_UNKNOWN;
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
		this->funcList.push_back(*it);
	}
	
	delete fData;
	delete fList;
	
	return 0;
	
}


/* funcion que genera un string de notas respetando el formato 
* partiendo de la lista de notas noteList 
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Module::parseNoteFromList(string &result)
{
	list<Note *>::iterator it;
	string *note = NULL;
	
	result.clear();
	if (this->noteList.empty())
		return 0;
	
	result.append("<mod-not>");
	/*NOTAS=<mod-not>nota1<mod-not>nota2<mod-not>....<mod-not>notaN<mod-not>*/
	for(it = this->noteList.begin(); it != this->noteList.end(); ++it) {
		assert((*it) != NULL);
		note = (*it)->toString();
		if (note == NULL) {
			return -1;
		}
		/* agregamos la nota */
		result.append(*note);
		delete note;
		result.append("<mod-not>");
	}
	
	return 0;
}

/* funcion que llena la lista desde un string que respete el
*  formato de las notas
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Module::parseNoteToList(string &result)
{
	Note *note = NULL;
	string aux = "";
	int pos = 0;
	int size = (int) result.size() - 9 /*<mod-not>*/;
	
	
	while (pos < size) {
		if (getValue(result, pos, "<mod-not>", "<mod-not>", aux) < 0) 
			return -1;
		
		/* generamos una nota con el string */
		note = new Note();
		if (note == NULL || note->fromString(aux) < 0)
			return -1;
		
		
		/* la agregamos */
		this->noteList.push_back(note);
		/* ahora corremos el puntero pos */
		pos += (int) aux.size() + 9 /*<mod-not>*/;
		
	}
	
	return 0;
}


/* funcion que genera un string de tasks respetando el formato 
* partiendo de la lista de tareas taskList 
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Module::parseTasksFromList(string &result)
{
	list<Task *>::iterator it;
	string *task = NULL;
	
	result.clear();
	if (this->taskList.empty())
		return 0;
	result.append("<mod-tas>");
	/*TASKS=<mod-tas>task1<mod-tas>task2<mod-tas>....<mod-tas>taskN<mod-tas>*/
	for(it = this->taskList.begin(); it != this->taskList.end(); ++it) {
		assert((*it) != NULL);
		task = (*it)->toString();
		if (task == NULL) {
			return -1;
		}
		/* agregamos la nota */
		result.append(*task);
		delete task;
		result.append("<mod-tas>");
	}
	
	return 0;
}

/* funcion que parsea un string que respete el formato de tasks
* para llenar la lista
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Module::parseTasksToList(string &result)
{
	Task *task = NULL;
	string aux = "";
	int pos = 0;
	int size = (int) result.size() - 9 /*<mod-tas>*/;
	
	while (pos < size) {
		if (getValue(result, pos, "<mod-tas>", "<mod-tas>", aux) < 0)
			return -1;
		/* generamos una nota con el string */
		task = new Task();
		if (task == NULL || task->fromString(aux) < 0)
			return -1;
		
		/* la agregamos */
		this->taskList.push_back(task);
		/* ahora corremos el puntero pos */
		pos += (int) aux.size() + 9 /*<mod-tas>*/;
	}
	
	return 0;
}


/*!	###		FUNCIONES PUBLICAS		###	*/

/* Constructor para setear todo de una */
Module::Module(string &n, int w, string& fn)
{
	this->name = n;
	this->weight = w;
	this->fname = fn;
	this->canCompile = false;
	this->tested = false;
}


/* funcion que agrega una funcion al modulo 
* REQUIRES:
* 	func	!= NULL
* NOTE: No debe ser eliminada la funcion una vez agregada.
*/
void Module::addFunction(Function *func)
{
	assert(func != NULL);
	this->funcList.push_back(func);
}

/* Funcion que permite agregar una nota relacionada al modulo
* REQUIRES:
* 	note	!= NULL
* NOTE: No debe ser liberada la nota una vez agregada 
*/
void Module::addNote(Note *note)
{
	assert(note != NULL);
	this->noteList.push_back(note);
}

/* Funcion que permite agregar una tarea relacionada al modulo
* REQUIRES:
* 	t 	!= NULL
* NOTE: No debe ser liberada la tarea una vez agregada 
*/
void Module::addTask(Task *t)
{
	assert(t != NULL);
	this->taskList.push_back(t);
}

/*! Funcion que devuelve el porcentaje de completado del modulo
* teniendo en cuenta los porcentajes de las funciones que
* tiene asociado el modulo
*/
float Module::getCompleted(void)
{
	float completed = 0;
	int totalW = 0;
	list<Function *>::iterator it;
	
	for(it = this->funcList.begin(); it != this->funcList.end(); ++it) {
		assert(*it != NULL);
		completed += (*it)->getWeight() * (*it)->getCompleted();
		totalW += (*it)->getWeight();
	}
	completed = completed / (float) totalW;
	
	
	return completed;
}

/*! Genera un modulo desde un string respetando el formato
* asignado para guardar los modulos
* Carga tanto las notas como las tareas y las funciones
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int Module::fromString(string &str)
{
	string aux = "";
	
	/*<mod-fn>file-name<mod-notes>NOTAS<mod-tasks>TASKS<module;>*/
	if (getValue(str, 0,"<mod-fn>", "<mod-notes>", this->fname) < 0)
		return -1;
	if (getValue(str, 0,"<mod-notes>", "<mod-tasks>", aux) < 0)
		return -1;
	/* parseamos las notas */
	if (parseNoteToList(aux) < 0)
		return -1;
	aux.clear();
	
	if (getValue(str, 0,"<mod-tasks>", "<module;>", aux) < 0)
		return -1;
	/* parseamos las tasks */
	if (parseTasksToList(aux) < 0)
		return -1;
	aux.clear();
	
	/* ahora cargamos de archivo las cosas */
	return loadFromFile();
}

/*! Convierte un modulo en un string listo para ser guardado
* en un archivo.
* Guarda todo, las notas como las tareas..
* RETURNS:
*	NULL		if error
*	strNote		otherwise
* NOTE: Genera memoria
*/
string *Module::toString(void)
{
	string *result = new string();
	string aux = "";
	
	if (result == NULL)
		return result;
	
	/*<mod-fn>file-name<mod-notes>NOTAS<mod-tasks>TASKS<module;>*/
	result->append("<mod-fn>");
	result->append(this->fname);
	result->append("<mod-notes>");
	if (parseNoteFromList(aux) < 0) {
		delete result;
		return NULL;
	}
	result->append(aux);
	result->append("<mod-tasks>");
	if (parseTasksFromList(aux) < 0) {
		delete result;
		return NULL;
	}
	result->append(aux);
	result->append("<module;>");
	
	return result;
}


/* Funcion que actualiza el modulo (funciones, peso, etc) desde
* el archivo al cual tiene asignado (fname)
* RETURNS:
* 	< 0	on error
* 	 0	if success
*/
int Module::actualize(void)
{	
	return loadFromFile();
}

/* destructor:
* NOTE: Libera toda la memoria relacionada con las listas
* y tareas, etc
*/
Module::~Module()
{
	freeFunctions();
	freeNotes();
	freeTasks();
}


/* DEBUG */
void Module::Print(void)
{
	list<Function *>::iterator fit;
	list<Note *>::iterator nit;
	list<Task *>::iterator tit;
	
	
	cout << "\nModule Name " << this->name;
	cout << "\nModule File Name " << this->fname;
	cout << "\nModule weight " << this->weight;
	cout << "\nCan compile: " << this->canCompile;
	cout << "\tTested: " << this->tested;
	cout << "\tModule Type " << this->type;
	
	cout << "\nFunciones del modulo \n";
	
	for(fit = this->funcList.begin(); fit != this->funcList.end(); ++fit) {
		assert(*fit != NULL);
		(*fit)->Print();
		
	}
	cout << "\n\n\nNotas: \n";
	for(nit = this->noteList.begin(); nit != this->noteList.end(); ++nit) {
		assert(*nit != NULL);
		(*nit)->Print();
	}
	cout << "\n\n\nTareas: \n";
	for(tit = this->taskList.begin(); tit != this->taskList.end(); ++tit) {
		assert(*tit != NULL);
		(*tit)->Print();
	}
	
}