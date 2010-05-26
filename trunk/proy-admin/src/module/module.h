#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <list>
#include <string>
#include <assert.h>

#include "function.h"
#include "task.h"
#include "note.h"
#include "parser_aux.h"


using namespace std;


/* definimos los tipos de modulos */
enum {
	MODULE_T_CLASS,		/* es una clase */
	MODULE_T_MODULE,	/* es un modulo */
	MODULE_T_UNKNOWN	/* que lo que */
};

class Module {
	
	public:
		/* constructor */
		Module(){};
		
		/* Constructor para setear todo de una */
		Module(string &n, int w, string& fname);
		
		/* set/get fname */
		void setFileName(string &fn){this->fname = fn;};
		string &getFileName(void){return this->fname;};
		
		/* set/get name */
		void setName(string &n){this->name = n;};
		string &getName(void){return this->name;};
		
		/* set/get canCompile */
		void setCanComiple(bool c){this->canCompile = c;};
		bool getCanComiple(void){return this->canCompile;};
		
		/* set/get tested*/
		void setTested(bool t){this->tested = t;};
		bool getTested(void){return this->tested;};
		
		/* set/get weight */
		void setWeight(int w){this->weight = w;};
		int getWeight(void){return this->weight;};
		
		/* set/get type*/
		void setType(int t){this->type= t;};
		int getType(void){return this->type;};
		
		/* Funcion que devuelve la lista de funciones del modulo */
		list<Function *> &getFunctions(void){return this->funcList;};
		
		/* funcion que agrega una funcion al modulo 
		 * REQUIRES:
		 * 	func	!= NULL
		 * NOTE: No debe ser eliminada la funcion una vez agregada.
		 */
		void addFunction(Function *func);
		
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
		
		/*! Funcion que devuelve el porcentaje de completado del modulo
		 * teniendo en cuenta los porcentajes de las funciones que
		 * tiene asociado el modulo
		 */
		float getCompleted(void);
		
		
		/* Funcion que actualiza el modulo (funciones, peso, etc) desde
		* el archivo al cual tiene asignado (fname)
		* RETURNS:
		* 	< 0	on error
		* 	 0	if success
		*/
		int actualize(void);
		
		/*! Genera un modulo desde un string respetando el formato
		* asignado para guardar los modulos
		* Carga tanto las notas como las tareas y las funciones
		* RETURNS:
		* 	< 0	on error
		*	0	if success
		*/
		int fromString(string &str);
		
		/*! Convierte un modulo en un string listo para ser guardado
		* en un archivo.
		* Guarda todo, las notas como las tareas..
		* RETURNS:
		*	NULL		if error
		*	strNote		otherwise
		* NOTE: Genera memoria
		*/
		string *toString(void);
		
		/* destructor:
		 * NOTE: Libera toda la memoria relacionada con las listas
		 * y tareas, etc
		 */
		~Module();
		
		/* DEBUG */
		void Print(void);
	
	private:
		/*			Funciones			*/
		
		/* Funcion que lee el archivo asignado al modulo y extrae
		 * el nombre del modulo, funciones, etc, todo menos las notas
		 * y las tasks
		 * RETURNS:
		 * 	< 0	if error
		 *	0	on success
		 */
		int loadFromFile(void);
		
		/* Funcion que libera todas Funciones asociadas */
		void freeFunctions(void);
		
		/* funcion que libera todas las notas asociadas */
		void freeNotes(void);
		
		/* funcion que libera todas las tasks asociads */
		void freeTasks(void);
		
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
		
		
		/*			Atributos			*/
		int weight;	/* peso del modulo en el proyecto */
		string name;
		string fname;	/* file name */
		list<Function *> funcList;
		list<Task *> taskList;
		list<Note *> noteList;
		bool canCompile;
		bool tested;
		int type;
		
	
};




#endif
