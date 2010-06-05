#include <QtGui>
#include <assert.h>
#include <math.h>

#include "function_gui.h"

GUIFunction::GUIFunction(QWidget *parent) : QDialog(parent)
{
	
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	

}


/* funcion que carga el gui desde una funcion
* REQUIRES:
* 	f	!= NULL
*/
void GUIFunction::loadFromFunction(Function *f)
{
	QString n = "";
	Qt::CheckState state;
	
	assert(f != NULL);
	
	n = f->getName().c_str();
	this->nameTxt->setText(n);
	
	if (f->getTested())
		state = Qt::Checked;
	else
		state = Qt::Unchecked;
	
	this->testedBox->setCheckState(state);
	
	this->weightBar->setValue(f->getWeight());
	this->completedBar->setValue(f->getCompleted());
}

/*! FIXME: deberiamos poder guardar la funcion con los cambios
* realizados y de esta forma modificar el modulo especifico..
*/

/* Funcion que genera desde el gui un nuevo function.
* RETURNS:
* 	new function 	if success
* 	NULL		otherwise
* NOTE no se va a usar.
*/
Function *GUIFunction::getFunction(void)
{
	return NULL;
}
