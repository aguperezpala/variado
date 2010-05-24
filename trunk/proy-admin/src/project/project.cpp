#include "function.h"


/* Constructor para setear todo de una */
Function::Function(string &n, int comp, int w)
{
	this->name = n;
	this->completed = comp;
	this->weight = w;
}