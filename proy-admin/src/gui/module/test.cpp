
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "searcher.h"
#include "module_gui.h"
#include "module.h"


GUIModule *mod;



int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	Qt::WindowFlags flags = 0;
	string aux = "", fname;
	Module *m;
	Searcher searcher;
	Project pro;
	
	
	
	fname = "../../searcher_conf.conf";
	searcher.setProject(&pro);
	searcher.loadConfig(fname);
	
	aux = "../../parse_test_file.h";
	m = new Module();
	m->setFileName(aux);	
	cout << "m->actualize(): " << m->actualize() << endl;
	
	pro.addModule(m);
	cout << "searcher.searchForNotes() " << searcher.searchForNotes() << endl;
	//pro.Print();
	cout << "\n\n AGREGANDO LAS TAREAS: \n\n";
	cout << " searcher.searchForNotes() " << searcher.searchForTasks() << endl;
	
	mod = new GUIModule(NULL);
	mod->loadFromModule(m);
	mod->exec();
	delete m;
	delete mod;
	
	app.exec();
	
	return 0;
}