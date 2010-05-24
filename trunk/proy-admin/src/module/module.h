#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <string>

#include "function.h"
#include "task.h"
#include "note.h"

class Module {
	
	public:
		/* constructor */
		Module(){};
		
		/* Constructor para setear todo de una */
		Module(string &n, int w);
		
		
		~Module(){};
	
	private:
		int completed;	/* % de completado de la funcion */
		int weight;	/* peso de la funcion en el modulo */
		string name;
	
};




#endif
