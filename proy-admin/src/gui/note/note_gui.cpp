#include <QtGui>
#include <assert.h>
#include <math.h>

#include "note_gui.h"



GUINote::GUINote(QWidget *parent)  : QDialog(parent)
{
	
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	
}

/* funcion que carga el gui desde una nota ya creada
* REQUIRES:
* 	note != NULL
*/
void GUINote::loadFromNote(Note *note)
{
	QString aux = "";
	
	assert(note != NULL);
	
	aux = note->getNote().c_str();
	this->noteEdit->setPlainText(aux);
}

/* Funcion que guarda la nota segun el gui
* REQUIRES:
* 	note != NULL
*/
void GUINote::saveNote(Note *note)
{
	string aux = "";
	
	assert(note != NULL);
	
	aux = this->noteEdit->toPlainText().toStdString();
	note->setNote(aux);
}

/* Funcion que genera desde el gui un nuevo note.
* RETURNS:
* 	new Note	if success
* 	NULL		otherwise
*/
Note *GUINote::getNote(void)
{
	Note *note = new Note();
	string aux = "";
	
	if (note == NULL)
		return note;
	
	aux = this->noteEdit->toPlainText().toStdString();
	note->setNote(aux);
	
	return note;
}

