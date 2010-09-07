/*! modulo que se encarga de buscar cadenas determinadas y generar distintos
 * tipos de objetos dependiendo de que es lo que se determina en el archivo
 * de configuracion.
 * NOTE: debe respetar las reglas de asignacion (leer FORMATO.txt)
 */
#ifndef SEARCHER_H
#define SEARCHER_H

#include <iostream>
#include <string>
#include <assert.h>

#include "dir_mod_aux.h"
#include "file_aux.h"
#include "parser.h"
#include "parser_aux.h"
#include "project.h"
#include "note.h"
#include "task.h"
#include "module.h"

using namespace std;

static const string SEARCHER_STR[] = {
	"SEARCHER_TASK_K_BUGFIX",
	"SEARCHER_TASK_K_REVISION",
	"SEARCHER_TASK_K_TODO",
	"SEARCHER_TASK_k_IMPROVE",
	"SEARCHER_TASK_K_NORMAL",
	"SEARCHER_NOTE",
	""
};

enum {
	SEARCHER_TASK_K_BUGFIX,		/* es una tarea para reparar un bug */
	SEARCHER_TASK_K_REVISION,	/* es una tarea para revisar algo */
	SEARCHER_TASK_K_TODO,		/* tarea normal */
	SEARCHER_TASK_k_IMPROVE,	/* tarea de mejoramiento */
	SEARCHER_TASK_K_NORMAL,		/* tarea normal..? wtf XD */
	SEARCHER_NOTE
};

class Searcher {
	
	public:
		/* constructor */
		Searcher(){};
		
		/* Funcion que carga la configuracion del searcher */
		void loadConfig(string &confPath);
		
		/* Funcion que setea un proyecto en el cual va a buscar 
		 * REQUIRES:
		 *	p 	!= NULL
		 */
		void setProject(Project *p);
		
		/* Funcion que agrega la lista de notas que no se encuentren
		 * ya en los modulos a los respectivos....
		 * REQUIRES:
		 * 	se haya seteado un proyecto
		 * RETURNS:
		 * 	number of notes finded
		 */
		int searchForNotes(void);
		
		/* Funcion que genera las tareas determinadas segun lo que se
		 * configuro en el archivo y las agrega a cada modulo
		 * respectivamente.
		 * REQUIRES:
		 * 	se haya seteado un proyecto
		 * RETURNS:
		 * 	number of Tasks founded
		 */
		int searchForTasks(void);
		
		
		~Searcher(){};
		
		
	private:
		/* funcion que crea una tarea segun el indice 
		 * RETURNS:
		 * 	NULL	on error
		 * 	task	if success
		 */
		Task *createTask(int index, string &t);
		
		/* funcion que dada una cadena la parsea y agrega
		 * tanto a la lista de strings la cadena y el indice 
		 * correspondiente
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if success
		 */
		int parseStr(string &line);
		
		
		/* Funcion que dado un string, busca en el mismo todas las 
		 * posibles notas, las parsea y las devuelve en una lista
		 * de tareas.
		 * RETURNS:
		 * 	NULL		si no hay tareas o error
		 * 	list<Note *> *	caso contrario
		 */
		list<Note *> *getNotesFromStr(string &str);
		
		/* Funcion que dado un string, busca en el mismo todas las 
		* posibles tareas, las parsea y las devuelve en una lista
		* de tareas.
		* RETURNS:
		* 	NULL		si no hay tareas o error
		* 	list<Task *> *	caso contrario
		*/
		list<Task *> *getTasksFromStr(string &str);
		
		Project *pro;
		/* lista que representa las cadenas a buscar */
		vector<string> strList; 
		/* lista que representa tipo (matching uno a uno con las str) */
		vector<int> typeList;
		
	
};




#endif
