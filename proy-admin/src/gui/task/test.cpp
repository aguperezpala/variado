
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "task_gui.h"
#include "task.h"


GUITask *task;
GUITask *task2;
Task *t;

int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	Qt::WindowFlags flags = 0;
	string aux = "";
	
	t = new Task();
	aux = "Titulo 1";
	t->setTitle(aux);
	aux = "Esta es la descripcion deu na tarea bolo\nsi si buenisimo :D";
	t->setDescription(aux);
	t->setPriority(TASK_P_HIGH);
	t->setKind(TASK_k_IMPROVE);
	t->setStatus(TASK_S_CHECKED);
	
	
	task = new GUITask(NULL);
	task->loadFromTask(t);
	cout << "task->exec(): " << task->exec() << endl;
	delete t;
	
	t = task->getTask();
	task2 = new GUITask(NULL);
	task2->loadFromTask(t);
	cout << "task->exec(): " << task2->exec() << endl;
	delete t;
	
	//app.exec();
	
	delete task2;
	delete task;
	return 0;
}