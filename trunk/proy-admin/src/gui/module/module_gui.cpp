#include <QtGui>
#include <assert.h>
#include <math.h>
#include <QTableWidgetItem>

#include "module_gui.h"


/* funcion que rellena la tabla de tareas por medio de una lista
* de Tasks */
void GUIModule::fillTasksTable(list<Task *> &l)
{
	list<Task *>::iterator it;
	QTableWidgetItem *newItem = NULL;
	QString aux = "";
	string stdaux = "";
	
	/*! notar que tiene que haber una biyeccion aca entre los campos de las
	 * tasks y la cantidad de columnas de la tabla */
	
	for(it = l.begin(); it != l.end(); ++it) {
		if(*it == NULL)
			continue;
		/* agregamos una nueva fila donde vamos agregar la tarea */
		this->tasksTable->insertRow(this->tasksTable->rowCount());
		
		/* sacamos el titulo */
		stdaux = (*it)->getTitle();
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,0,
					   newItem);
		
		/* sacamos la descripcion */
		stdaux = (*it)->getDescription();
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,1,
					   newItem);
					   
		/* sacamos la prioridad */
		(*it)->getStringPriority(stdaux);
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,2,
					   newItem);
					   
		/* sacamos el kind */
		(*it)->getStringType(stdaux);
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,3,
					   newItem);
		
		/* sacamos el state */
		(*it)->getStringStatus(stdaux);
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,4,
					   newItem);
		
		/* sacamos el kind */
		stdaux = "Created time FIXME";
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->tasksTable->setItem(this->tasksTable->rowCount()-1,5,
					   newItem);
		
	}
	
	
}

/* funcion que rellena la tabla de notas por medio de una lista
* de Notes */
void GUIModule::fillNotesTable(list<Note *> &l)
{
	list<Note *>::iterator it;
	QTableWidgetItem *newItem = NULL;
	QString aux = "";
	string stdaux = "";
	
	for(it = l.begin(); it != l.end(); ++it) {
		if(*it == NULL)
			continue;
		/* agregamos una nueva fila donde vamos agregar la nota */
		this->notesTable->insertRow(this->notesTable->rowCount());
		
		/* sacamos la nota */
		stdaux = (*it)->getNote();
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->notesTable->setItem(this->notesTable->rowCount()-1,0,
					   newItem);
		
	}
}

/* funcion que rellena la tabla de funciones por medio de una lista
* de Functions */
void GUIModule::fillFunctionsTable(list<Function *> &l)
{
	list<Function *>::iterator it;
	QTableWidgetItem *newItem = NULL;
	QString aux = "";
	string stdaux = "";
	int iaux = 0;
	
	/*! notar que tiene que haber una biyeccion aca entre los campos de las
	 * funciones y la cantidad de columnas de la tabla */
	
	for(it = l.begin(); it != l.end(); ++it) {
		if(*it == NULL)
			continue;
		/* agregamos una nueva fila donde vamos agregar la tarea */
		this->functionsTable->insertRow(this->functionsTable->rowCount());
		
		/* sacamos el titulo */
		stdaux = (*it)->getName();
		aux = stdaux.c_str();
		newItem = new QTableWidgetItem(aux);
		this->functionsTable->setItem(this->functionsTable->rowCount()-1,0,
					   newItem);
		
		/* sacamos el % completado */
		iaux = (*it)->getCompleted();
		aux = QString::number(iaux);
		newItem = new QTableWidgetItem(aux);
		this->functionsTable->setItem(this->functionsTable->rowCount()-1,1,
					   newItem);
					   
		/* sacamos el peso */
		iaux = (*it)->getWeight();
		aux = QString::number(iaux);
		newItem = new QTableWidgetItem(aux);
		this->functionsTable->setItem(this->functionsTable->rowCount()-1,2,
					   newItem);
					   
		/* sacamos si fue testeada o no */
		if ((*it)->getTested())
			aux = "True";
		else
			aux = "False";
		newItem = new QTableWidgetItem(aux);
		this->functionsTable->setItem(this->functionsTable->rowCount()-1,3,
					   newItem);
		
	}
}




GUIModule::GUIModule(QWidget *parent) : QDialog(parent)
{
	
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	//connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	
}


/* funcion que carga el gui desde un modulo
* REQUIRES:
* 	mod	!= NULL
*/
void GUIModule::loadFromModule(Module *mod)
{
	QString aux = "";
	string stdaux = "";
	
	assert(mod != NULL);
	
	
	aux = mod->getName().c_str();
	this->nameTxt->setText(aux);
	
	aux = mod->getFileName().c_str();
	this->fnameTxt->setText(aux);
	
	mod->getStringType(stdaux);
	aux = stdaux.c_str();
	this->typeTxt->setText(aux);
	
	if(mod->getTested())
		this->testedBox->setCheckState(Qt::Checked);
	else
		this->testedBox->setCheckState(Qt::Unchecked);
	
	if(mod->getCanCompile())
		this->canCompileBox->setCheckState(Qt::Checked);
	else
		this->canCompileBox->setCheckState(Qt::Unchecked);
	
	this->weightBar->setValue(mod->getWeight());
	this->completedBar->setValue((int)mod->getCompleted());
	
	/* cargamos las tareas */
	fillTasksTable(mod->getTasks());
	
	/* cargamos las notas */
	fillNotesTable(mod->getNotes());
	
	/* cargamos las funciones */
	fillFunctionsTable(mod->getFunctions());
	
}


