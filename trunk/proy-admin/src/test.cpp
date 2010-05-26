#include <iostream>
#include <string>

#include "note.h"
#include "task.h"
#include "function.h"
#include "module.h"
#include "parser.h"
#include "parser_aux.h"

int main(void)
{
	Module module, m2;
	Task t;
	string fname = "parse_test_file.h";
	string desc = "esto es la nota numero no se que";
	string d = "<task-desc>descripcion<task-title>title<task-tc>1515<task-tl>timeLimit<task-st>0000<task-k>0001<task-p>0007<task;>";
	string *str;
	int i = 0;
	Note *note = NULL;
	
	
	
	for (i = 0; i < 10; i++)
	{
		note = new Note(desc);
		module.addNote(note);
	}
	
	module.setFileName(fname);
	m2.setFileName(fname);
	if (module.actualize()<0)
		cout << "ERROR AL ACTUALIZAR MODULO\n";
	
	str = module.toString();
	/*cout << "Module str:\n" << *str << endl;*/
	
	m2.fromString(*str);
	if (m2.actualize() <0 )
		cout << "ERROR ACTUALIZANDO m2\n";
	cout << "\n\n**********************************\n\nstr:\n" << *str <<
	"\n\n**********************************\n\n";
	delete str;
	
	cout << "Print module:\n";
	module.Print();
	cout << "\nPrint m2:\n";
	m2.Print();
	/*
	cout << "t.fromString(d); " << t.fromString(d) << endl;
	t.setPriority(2);
	t.setKind(3);
	t.setStatus(2);
	
	str = t.toString();
	
	cout << "str.size(): " << str->size() << "\nt.toString:\n " << *str << endl ;
	cout << "t.fromString(d); " << t.fromString(*str) << endl;
	delete str;
	
	cout << "AROA VAMOS A IMPRIMIR PRINT \n\n";
	t.Print();
	*/
// 	cout << "parse_file(fname, Module &m): " << parse_file(fname,module);
// 	module.Print();
// 	
	return 0;
}

