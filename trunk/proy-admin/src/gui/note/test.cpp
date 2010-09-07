
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "note_gui.h"
#include "note.h"


GUINote *note;
GUINote *note2;
Note *n;

int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	Qt::WindowFlags flags = 0;
	string aux = "";
	
	n = new Note();
	aux = "Esta es la descripcion deu na tarea bolo\nsi si buenisimo :D";
	n->setNote(aux);
	
	
	note = new GUINote(NULL);
	note2 = new GUINote(NULL);
	note2->loadFromNote(n);
	cout << "note2->exec(): " << note2->exec() << endl;
	note2->saveNote(n);
	note->loadFromNote(n);
	cout << "note->exec(): " << note->exec() << endl;
	
	n->Print();
	
	app.exec();
	
	delete note;
	return 0;
}