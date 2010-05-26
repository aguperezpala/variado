#include "project.h"


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



