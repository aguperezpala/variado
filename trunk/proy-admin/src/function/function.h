#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>

class Function {
	
	public:
		/* constructor */
		Function(){};
		
		/* Constructor para setear todo de una */
		Function(string &n, int comp, int w);
		
		/* setea/obtiene el nombre de la funcion */
		void setName(string &n){this->name = n;};
		string &getName(void){return this->name;};
		
		/* setea/obtiene el porcentaje completado */
		void setCompleted(int c){this->completed = c;};
		int getCompleted(void){return this->completed;};
		
		/* setea/obtiene el peso */
		void setWeight(int w){this->weight= c;};
		int getWeight(void){return this->weight;};
		
		~Function(){};
	
	private:
		int completed;	/* % de completado de la funcion */
		int weight;	/* peso de la funcion en el modulo */
		string name;
	
};




#endif
