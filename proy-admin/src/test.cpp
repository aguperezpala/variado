#include <iostream>
#include <string>

#include "searcher.h"
#include "file_aux.h"
#include "project.h"
#include "note.h"
#include "task.h"
#include "function.h"
#include "module.h"
#include "parser.h"
#include "parser_aux.h"

int main(void)
{
	Module module, *m2;
	Task t;
	string fname = "parse_test_file.h";
	string desc = "esto es la nota numero no se que";
	string d = "<task-desc>descripcion<task-title>title<task-tc>1515<task-tl>timeLimit<task-st>0000<task-k>0001<task-p>0007<task;>";
	Project pro;
	string *str;
	Searcher searcher;
	
	
	
	m2 = new Module();
	m2->setFileName(fname);
	cout << "m2->actualize(): " << m2->actualize() << endl;
	
	
	pro.addModule(m2);
	fname = "searcher_conf.conf";
	searcher.setProject(&pro);
	searcher.loadConfig(fname);
	
	pro.Print();
	/* agregamos ahora las notas e imprimirmos */
	cout << "\n\n AGREGANDO LAS NOTAS: \n\n";
	cout << "searcher.searchForNotes() " << searcher.searchForNotes() << endl;
	pro.Print();
	cout << "\n\n AGREGANDO LAS TAREAS: \n\n";
	cout << " searcher.searchForNotes() " << searcher.searchForTasks() << endl;
	pro.Print();
	
	fname = ".project";
	pro.setDir(fname);
	str = pro.toString();
	if (str == NULL)
		cout << "error al convertir el proj en string\n";
	else {
		fname = "project.pro";
		cout << "guardando en disco: " << filea_create_w(fname, *str)<< endl;
		delete str;
	}	
	
	return 0;
}

