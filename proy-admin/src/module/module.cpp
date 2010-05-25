#include "module.h"



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

/* destructor:
* NOTE: Libera toda la memoria relacionada con las listas
* y tareas, etc
*/
Module::~Module()
{
	list<Function *>::iterator fit;
	list<Note *>::iterator nit;
	list<Task *>::iterator tit;
	
	for(fit = this->funcList.begin(); fit != this->funcList.end(); ++fit) {
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