
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "function_gui.h"
#include "function.h"


GUIFunction *fun;
Function *f;

int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	Qt::WindowFlags flags = 0;
	string aux = "";
	
	f = new Function();
	aux = "void esto_es_una_prueba(Integer x);";
	f->setName(aux);
	f->setTested(true);
	f->setWeight(23);
	f->setCompleted(78);
	
	
	fun = new GUIFunction(NULL);
	fun->loadFromFunction(f);
	cout << "function->exec(): " << fun->exec() << endl;
	app.exec();
	
	delete f;
	delete fun;
	return 0;
}