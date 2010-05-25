#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <string>


using namespace std;

class Project {
	
	public:
		/* constructor */
		Project(){};
		
		~Project(){};
	
	private:
		int completed;	/* % de completado de la funcion */
		int weight;	/* peso de la funcion en el modulo */
		string name;
	
};




#endif
