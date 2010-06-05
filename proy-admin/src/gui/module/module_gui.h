

#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <QUiLoader>
#include <QWidget>
#include <QColor>
#include <QDialog>
#include <QBrush>


QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

#include "ui_module_ui.h"
#include "module.h"
#include "task_gui.h"
#include "note_gui.h"

class GUIModule: public QDialog, private Ui::GUIModule 
{
    Q_OBJECT

public:
	GUIModule(QWidget *parent);
	
	/* funcion que carga el gui desde un modulo
	 * REQUIRES:
	 * 	mod	!= NULL
	 */
	void loadFromModule(Module *mod);
	
	
	
	~GUIModule(){};
	
		

protected:

private:
	/* funcion que rellena la tabla de tareas por medio de una lista
	 * de Tasks */
	void fillTasksTable(list<Task *> &l);
	
	/* funcion que rellena la tabla de notas por medio de una lista
	* de Notes */
	void fillNotesTable(list<Note *> &l);
	
	/* funcion que rellena la tabla de funciones por medio de una lista
	* de Functions */
	void fillFunctionsTable(list<Function *> &l);
	
	
	Module *actualM;
	
};

#endif
