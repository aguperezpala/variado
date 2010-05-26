#include "project.h"


/* funcion que genera un string de notas respetando el formato 
* partiendo de la lista de notas noteList 
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseNoteFromList(string &result)
{
	list<Note *>::iterator it;
	string *note = NULL;
	
	result.clear();
	if (this->noteList.empty())
		return 0;
	
	result.append("<proj-not>");
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
		result.append("<proj-not>");
	}
	
	return 0;
}

/* funcion que llena la lista desde un string que respete el
*  formato de las notas
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseNoteToList(string &result)
{
	Note *note = NULL;
	string aux = "";
	int pos = 0;
	int size = (int) result.size() - 10 /*<proj-not>*/;
	
	
	while (pos < size) {
		if (getValue(result, pos, "<proj-not>", "<proj-not>", aux) < 0) 
			return -1;
		
		/* generamos una nota con el string */
		note = new Note();
		if (note == NULL || note->fromString(aux) < 0)
			return -1;
		
		
		/* la agregamos */
		this->noteList.push_back(note);
		/* ahora corremos el puntero pos */
		pos += (int) aux.size() + 10 /*<proj-not>*/;
		
	}
	
	return 0;
}


/* funcion que genera un string de tasks respetando el formato 
* partiendo de la lista de tareas taskList 
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseTasksFromList(string &result)
{
	list<Task *>::iterator it;
	string *task = NULL;
	
	result.clear();
	if (this->taskList.empty())
		return 0;
	result.append("<proj-tas>");
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
		result.append("<proj-tas>");
	}
	
	return 0;
}

/* funcion que parsea un string que respete el formato de tasks
* para llenar la lista
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseTasksToList(string &result)
{
	Task *task = NULL;
	string aux = "";
	int pos = 0;
	int size = (int) result.size() - 10 /*<proj-tas>*/;
	
	while (pos < size) {
		if (getValue(result, pos, "<proj-tas>", "<proj-tas>", aux) < 0)
			return -1;
		/* generamos una nota con el string */
		task = new Task();
		if (task == NULL || task->fromString(aux) < 0)
			return -1;
		
		/* la agregamos */
		this->taskList.push_back(task);
		/* ahora corremos el puntero pos */
		pos += (int) aux.size() + 10 /*<proj-tas>*/;
	}
	
	return 0;
}


/* funcion que genera un string de modules respetando el formato 
* partiendo de la lista de modulos modulesList 
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseModulesFromList(string &result)
{
	list<Module *>::iterator it;
	string modName = "";
	string *modData = NULL;
	string path = "";
	
	result.clear();
	if (this->moduleList.empty())
		return 0;
	
	result.append("<proj-mod>");
	
	/*MODULES = <proj-mod>modFileName<proj-mod>....*/
	for(it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		assert((*it) != NULL);
		
		/* agregamos el nombre del modulo */
		modName = (*it)->getName();
		modName.append(".mod");
		
		/*! aca tenemos que crear el archivo con nombre modName,
		 * e insertarle (*it)->toString(). */
		modData = (*it)->toString();
		if (modData == NULL)
			return -1;
		
		path = this->projDir;
		if (path[path.size()-1] != '/')
			path.append("/");
		
		path.append(modName);
		if (filea_create_w(path, *modData) < 0){
			delete modData;
			return -1;
		}
		
		delete modData;
		
		result.append(modName);
		result.append("<proj-mod>");
	}
	
	return 0;
}


/* funcion que parsea un string que respete el formato de modules
* para llenar la lista modulesList
* RETURNS:
* 	< 0 	on error
* 	0	if success
*/
int Project::parseModulesToList(string &result)
{
	Module *module = NULL;
	string aux = "";
	string *fData = NULL;
	int pos = 0;
	int size = (int) result.size() - 10 /*<proj-mod>*/;
	
	/*MODULES = <proj-mod>modFileName<proj-mod>....*/
	while (pos < size) {
		if (getValue(result, pos, "<proj-mod>", "<proj-mod>", aux) < 0)
			return -1;
		
		/* ahora cargamos el archivo del modulo */
		fData = parser_read_all_file(aux.c_str());
		if (fData == NULL)
			return -1;
		
		/* generamos una nota con el string */
		module = new Module();
		if (module == NULL) {
			delete fData;
			return -1;
		}
		if (module->fromString(*fData) < 0){
			delete fData;
			delete module;
			return -1;
		}
		
		
		/* la agregamos */
		this->moduleList.push_back(module);
		/* ahora corremos el puntero pos */
		pos += (int) aux.size() + 10 /*<proj-mod>*/;
	}
	
	return 0;
}


/*!	###		FUNCIONES PUBLICAS		### 	*/



/* funcion que agrega un modulo al proyecto
* REQUIRES:
* 	module != NULL
* NOTE: No debe ser eliminada la funcion una vez agregada.
*/
void Project::addModule(Module *module)
{
	assert(module != NULL);
	this->moduleList.push_back(module);
}

/* Funcion que permite agregar una nota relacionada al modulo
* REQUIRES:
* 	note	!= NULL
* NOTE: No debe ser liberada la nota una vez agregada 
*/
void Project::addNote(Note *note)
{
	assert(note != NULL);
	this->noteList.push_back(note);
}

/* Funcion que permite agregar una tarea relacionada al modulo
* REQUIRES:
* 	t 	!= NULL
* NOTE: No debe ser liberada la tarea una vez agregada 
*/
void Project::addTask(Task *t)
{
	assert(t != NULL);
	this->taskList.push_back(t);
}

/*! Funcion que devuelve el porcentaje de completado del modulo
* teniendo en cuenta los porcentajes de las funciones que
* tiene asociado el modulo
*/
float Project::getCompleted(void)
{
	float completed = 0;
	int totalW = 0;
	list<Module *>::iterator it;
	
	for(it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		assert(*it != NULL);
		completed += (float)(*it)->getWeight() * (*it)->getCompleted();
		totalW += (*it)->getWeight();
	}
	completed = completed / (float) totalW;
	
	
	return completed;
}


/*! Genera un proyecto desde un string respetando el formato
* asignado para guardar los proyectos
* Carga tanto las notas como las tareas y los modulos
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int Project::fromString(string &str)
{
	string aux = "";
	
	/*<proj-name>name<proj-dsc>description<proj-dir>projDir<proj-t>tested<proj-co>
	canCompile<proj-mods>MODULES<proj-notes>NOTES<proj-tasks>TASKS<project;>*/
	if (getValue(str, 0,"<proj-name>", "<proj-dsc>", this->name) < 0)
		return -1;
	if (getValue(str, 0,"<proj-dsc>", "<proj-dir>", this->desc) < 0)
		return -1;
	if (getValue(str, 0,"<proj-dir>", "<proj-t>", this->projDir) < 0)
		return -1;
	if (getValue(str, 0,"<proj-t>", "<proj-co>", aux) < 0)
		return -1;
	else {
		if (sizeof(this->tested) != aux.size())
			return -1;
		memcpy(&this->tested, aux.c_str(), aux.size());
	}
	if (getValue(str, 0,"<proj-co>", "<proj-mods>", aux) < 0)
		return -1;
	else {
		if (sizeof(this->canCompile) != aux.size())
			return -1;
		memcpy(&this->canCompile, aux.c_str(), aux.size());
	}
	if (getValue(str, 0,"<proj-mods>", "<proj-notes>", aux) < 0)
		return -1;
	/* parseamos los modulos */
	if (parseModulesToList(aux) < 0)
		return -1;
	aux.clear();
	
	if (getValue(str, 0,"<proj-notes>", "<proj-tasks>", aux) < 0)
		return -1;
	/* parseamos las notas */
	if (parseNoteToList(aux) < 0)
		return -1;
	aux.clear();
		
	if (getValue(str, 0,"<proj-tasks>", "<project;>", aux) < 0)
		return -1;
	/* parseamos las tasks */
	if (parseTasksToList(aux) < 0)
		return -1;
	aux.clear();
	
	/* ahora cargamos de archivo las cosas */
	return 0;
	
}

/*! Convierte un proyecto en un string listo para ser guardado
* en un archivo.
* Guarda todo, las notas como las tareas.. y los modulos
* RETURNS:
*	NULL		if error
*	strNote		otherwise
* NOTE: Genera memoria
*/
string *Project::toString(void)
{
	string *result = new string();
	string aux = "";
	char buff[20];
	
	if (result == NULL)
		return result;
	
	/*<proj-name>name<proj-dsc>description<proj-dir>projDir<proj-t>tested<proj-co>
	canCompile<proj-mods>MODULES<proj-notes>NOTES<proj-tasks>TASKS<project;>*/
	
	result->append("<proj-name>");
	result->append(this->name);
	result->append("<proj-dsc>");
	result->append(this->desc);
	result->append("<proj-dir>");
	result->append(this->projDir);	
	result->append("<proj-t>");
	memcpy(buff, &this->tested, sizeof(this->tested));
	result->append(buff, sizeof(this->tested));	
	result->append("<proj-co>");
	memcpy(buff, &this->canCompile, sizeof(this->canCompile));
	result->append(buff, sizeof(this->canCompile));	
	result->append("<proj-mods>");
	if (parseModulesFromList(aux) < 0) {
		delete result;
		return NULL;
	}
	result->append(aux);
	aux.clear();
	result->append("<proj-notes>");
	if (parseNoteFromList(aux) < 0) {
		delete result;
		return NULL;
	}
	result->append(aux);	
	result->append("<proj-tasks>");
	if (parseTasksFromList(aux) < 0) {
		delete result;
		return NULL;
	}
	result->append(aux);
	result->append("<project;>");
	
	return result;
}


/* destructor:
* NOTE: Libera toda la memoria relacionada con las listas
* y tareas, etc
*/
Project::~Project()
{
	list<Module *>::iterator fit;
	list<Note *>::iterator nit;
	list<Task *>::iterator tit;
	
	for(fit = this->moduleList.begin(); fit != this->moduleList.end(); ++fit) {
		assert(*fit != NULL);
		delete (*fit);
	}
	for(nit = this->noteList.begin(); nit != this->noteList.end(); ++nit) {
		assert(*nit != NULL);
		delete (*nit);
	}
	for(tit = this->taskList.begin(); tit != this->taskList.end(); ++tit) {
		assert(*tit != NULL);
		delete (*tit);
	}
	
}


/* DEBUG */
void Project::Print(void)
{
	list<Module *>::iterator fit;
	list<Note *>::iterator nit;
	list<Task *>::iterator tit;	
	
	cout << "\nProject Name: " << this->name;
	cout << "\nProject Dir: " << this->projDir;
	cout << "\nProject Completed" << getCompleted();
	cout << "\nCan compile: " << this->canCompile;
	cout << "\tTested: " << this->tested;
	
	cout << "\nModulos del proyecto \n";
	
	for(fit = this->moduleList.begin(); fit != this->moduleList.end(); ++fit) {
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



