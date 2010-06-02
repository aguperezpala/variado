#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <string>

#include "module.h"
#include "note.h"
#include "task.h"
#include "parser.h"
#include "file_aux.h"


using namespace std;

class Project {
	
	public:
		/* constructor */
		Project();
		
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
		
		/*! Genera un proyecto desde un string respetando el formato
		* asignado para guardar los proyectos
		* Carga tanto las notas como las tareas y los modulos
		* RETURNS:
		* 	< 0	on error
		*	0	if success
		*/
		int fromString(string &str);
		
		/*! Convierte un proyecto en un string listo para ser guardado
		* en un archivo.
		* Guarda todo, las notas como las tareas.. y los modulos
		* RETURNS:
		*	NULL		if error
		*	strNote		otherwise
		* NOTE: Genera memoria
		*/
		string *toString(void);
		
		/* Destructor:
		 * NOTE: libera la memoria asociada a todos los modulos y 
		 * las demas cosas
		 */
		~Project();
		
		/* debug */
		void Print(void);
	
	private:
		/* funcion que genera un string de notas respetando el formato 
		* partiendo de la lista de notas noteList 
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseNoteFromList(string &result);
		
		/* funcion que llena la lista desde un string que respete el
		*  formato de las notas
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseNoteToList(string &result);
		
		/* funcion que genera un string de tasks respetando el formato 
		* partiendo de la lista de tareas taskList 
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseTasksFromList(string &result);
		
		/* funcion que parsea un string que respete el formato de tasks
		* para llenar la lista
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseTasksToList(string &result);
		
		/* funcion que genera un string de modules respetando el formato 
		* partiendo de la lista de modulos modulesList 
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseModulesFromList(string &result);
		
		/* funcion que parsea un string que respete el formato de modules
		* para llenar la lista modulesList
		* RETURNS:
		* 	< 0 	on error
		* 	0	if success
		*/
		int parseModulesToList(string &result);
		
		
		
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
