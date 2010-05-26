#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <string>

#include "module.h"
#include "note.h"
#include "task.h"


using namespace std;

class Project {
	
	public:
		/* constructor */
		Project(){};
		
		/* set/get project dir */
		void setDir(string &d){this->projDir = d;};
		string &getDir(void){return this->projDir;};
		
		/* set/get name */
		void setName(string &n){this->name = n;};
		string &getName(void){return this->name;};
		
		/* set/get description */
		void setDesc(string &d){this->desc= d;};
		string &getDesc(void){return this->desc;};
		
		/* set/get canCompile */
		void setCanComiple(bool c){this->canCompile = c;};
		bool getCanComiple(void){return this->canCompile;};
		
		/* set/get tested*/
		void setTested(bool t){this->tested = t;};
		bool getTested(void){return this->tested;};
		
		
		/* Funcion que devuelve la lista de modulos del proyecto */
		list<Module *> &getModules(void){return this->moduleList;};
		
		/* funcion que agrega un modulo al proyecto
		* REQUIRES:
		* 	module != NULL
		* NOTE: No debe ser eliminada la funcion una vez agregada.
		*/
		void addModule(Module *module);
		
		/* Funcion que devuelve la lista de notas */
		list<Note *> &getNotes(void){return this->noteList;};
		
		/* Funcion que permite agregar una nota relacionada al modulo
		* REQUIRES:
		* 	note	!= NULL
		* NOTE: No debe ser liberada la nota una vez agregada 
		*/
		void addNote(Note *note);
		
		/* Funcion que devuelve la lista de tareas */
		list<Task *> &getTasks(void){return this->taskList;};
		
		/* Funcion que permite agregar una tarea relacionada al modulo
		* REQUIRES:
		* 	t 	!= NULL
		* NOTE: No debe ser liberada la tarea una vez agregada 
		*/
		void addTask(Task *t);
		
		/*! Funcion que devuelve el porcentaje de completado del proyecto
		* teniendo en cuenta los porcentajes de los pesos y cuanto
		* estan completados los modulos.
		*/
		float getCompleted(void);
		
		/* Destructor:
		 * NOTE: libera la memoria asociada a todos los modulos y 
		 * las demas cosas
		 */
		~Project();
		
		/* debug */
		void Print(void);
	
	private:
		string name;
		string desc;
		string projDir;
		bool tested;
		bool canCompile;
		list<Module *> moduleList;
		list<Task *> taskList;
		list<Note *> noteList;
		
	
};




#endif
