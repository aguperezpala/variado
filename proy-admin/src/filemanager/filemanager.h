#ifndef NOTE_H
#define NOTE_H

#include <iostream>
#include <string>

using namespace std;

class Note {
	
	public:
		/* constructor */
		Note();
		
		/* Constructor para setear todo de una */
		Note(string &n){this->note = n;};
		
		/* setea/obtiene el nombre de la funcion */
		void setNote(string &n){this->note = n;};
		string &getNote(void){return this->note;};
		
		
		~Note(){};
		
		/*debug */
		void Print(void){cout << note << endl;};
	
	private:
		string note;
	
};




#endif
