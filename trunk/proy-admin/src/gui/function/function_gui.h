

#ifndef GUIFUNCTION_H
#define GUIFUNCTION_H

#include <QUiLoader>
#include <QWidget>
#include <QColor>
#include <QDialog>
#include <QBrush>


QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

#include "ui_function_ui.h"
#include "function.h"

class GUIFunction : public QDialog, private Ui::GUIFunction
{
    Q_OBJECT

public:
	GUIFunction(QWidget *parent);
	
	/* funcion que carga el gui desde una funcion
	 * REQUIRES:
	 * 	f	!= NULL
	 */
	void loadFromFunction(Function *f);
	
	/*! FIXME: deberiamos poder guardar la funcion con los cambios
	 * realizados y de esta forma modificar el modulo especifico..
	 */
	
	/* Funcion que genera desde el gui un nuevo function.
	 * RETURNS:
	 * 	new function 	if success
	 * 	NULL		otherwise
	 * NOTE no se va a usar.
	 */
	Function *getFunction(void);
	
	
	~GUIFunction(){};
	
		

protected:

private:
	Function *actualF;
	
};

#endif
