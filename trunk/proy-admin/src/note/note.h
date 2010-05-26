#ifndef NOTE_H
#define NOTE_H

#include <iostream>
#include <string>

using namespace std;

class Note {
	
	public:
		/* constructor */
		Note(){};
		
		/* Constructor para setear todo de una */
		Note(string &n){this->note = n;};
		
		/*! Genera una nota desde un string respetando el formato
		 * asignado para guardar las notas
		 * RETURNS:
		 * 	< 0	on error
		 *	0	if success
		 */
		int fromString(string &str);
		
		/*! Convierte una nota en un string listo para ser guardada
		 * en un archivo.
		 * RETURNS:
		 *	NULL		if error
		 *	strNote		otherwise
		 * NOTE: Genera memoria
		 */
		string *toString(void);
		
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
