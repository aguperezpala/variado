

#ifndef GUITASK_H
#define GUITASK_H

#include <QUiLoader>
#include <QWidget>
#include <QColor>
#include <QDialog>
#include <QBrush>


QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

#include "ui_task_ui.h"
#include "task.h"

class GUITask: public QDialog, private Ui::GUITask 
{
    Q_OBJECT

public:
	GUITask(QWidget *parent);
	
	/* funcion que carga el gui desde un task 
	 * REQUIRES:
	 * 	task != NULL
	 */
	void loadFromTask(Task *task);
	
	/* Funcion que guarda los datos en el task pasado.
	 * REQUIRES:
	 * 	task != NULL
	 */
	void saveTask(Task *task);
	
	/* Funcion que genera desde el gui un nuevo task.
	 * RETURNS:
	 * 	new Task	if success
	 * 	NULL		otherwise
	 */
	Task *getTask(void);
	
	
	~GUITask(){};
	
		

protected:

private:
	Task *actualT;
	
};

#endif
