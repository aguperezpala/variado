#include "function.h"


/* Constructor para setear todo de una */
Function::Function(string &n, int comp, int w)
{
	this->name = n;
	this->completed = comp;
	this->weight = w;
}

/* debug */
void Function::Print(void)
{
	cout << "\nFuncion Name " << this->name;
	cout << "\nFunction completed" << this->completed;
	cout << "\nFunction weight " << this->weight;
	cout << endl;	
}
