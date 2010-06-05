

#ifndef GUINOTE_H
#define GUINOTE_H

#include <QUiLoader>
#include <QDialog>
#include <assert.h>


QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

#include "ui_note_ui.h"
#include "note.h"

class GUINote : public QDialog, private Ui::GUINote 
{
    Q_OBJECT

public:
	GUINote(QWidget *parent);
	
	/* funcion que carga el gui desde una nota ya creada
	 * REQUIRES:
	 * 	note != NULL
	 */
	void loadFromNote(Note *note);
	
	/* Funcion que guarda la nota segun el gui
	 * REQUIRES:
	 * 	note != NULL
	 */
	void saveNote(Note *note);
	
	/* Funcion que genera desde el gui un nuevo note.
	 * RETURNS:
	 * 	new Note	if success
	 * 	NULL		otherwise
	 */
	Note *getNote(void);
	
	
	~GUINote(){};
	
		

protected:

private:
	Note *actualN;
	
};

#endif
