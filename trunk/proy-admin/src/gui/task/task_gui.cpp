#include <QtGui>
#include <assert.h>
#include <math.h>

#include "task_gui.h"

GUITask::GUITask(QWidget *parent) : QDialog(parent)
{
	
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

}


/* funcion que carga el gui desde un task 
* REQUIRES:
* 	task != NULL
*/
void GUITask::loadFromTask(Task *task)
{
	QString aux = "";
	assert(task != NULL);
	
	this->actualT = task;
	
	/* seteamos la prioridad-status-kind */
	this->priorityBox->setCurrentIndex(task->getPriority());
	this->stateBox->setCurrentIndex(task->getStatus());
	this->kindBox->setCurrentIndex(task->getKind());
	
	aux = task->getTitle().c_str();
	this->titleEdit->setText(aux);
	
	aux = task->getDescription().c_str();
	this->descEdit->setText(aux);
	/*! FIXME setear limit time */
	
	
}

/* Funcion que guarda los datos en el task pasado.
* REQUIRES:
* 	task != NULL
*/
void GUITask::saveTask(Task *task)
{
	string aux = "";
	
	assert(task != NULL);
	
	
	task->setPriority(this->priorityBox->currentIndex());
	task->setStatus(this->stateBox->currentIndex());
	task->setKind(this->kindBox->currentIndex());
	
	aux = this->titleEdit->text().toStdString();
	task->setTitle(aux);
	aux = this->descEdit->toPlainText().toStdString();
	task->setDescription(aux);
	/*! FIXME: falta la fecha */
}

/* Funcion que genera desde el gui un nuevo task.
* RETURNS:
* 	new Task	if success
* 	NULL		otherwise
*/
Task *GUITask::getTask(void)
{
	Task *result = new Task();
	string aux = "";
	
	if (result == NULL)
		return NULL;
	
	result->setPriority(this->priorityBox->currentIndex());
	result->setStatus(this->stateBox->currentIndex());
	result->setKind(this->kindBox->currentIndex());
	
	aux = this->titleEdit->text().toStdString();
	result->setTitle(aux);
	aux = this->descEdit->toPlainText().toStdString();
	result->setDescription(aux);
	
	/*! FIXME: arreglar el tiempo limite */
	
	return result;
	
}