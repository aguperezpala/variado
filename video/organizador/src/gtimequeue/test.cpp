#include <stdio.h>
#include <QApplication>
#include "gtimequeue.h"
#include "normalbox.h"
#include "tester.h"


int main (int argc, char **argv)
{
	QApplication app(argc, argv, true);
	printf("vamos a empezar...\n");
	GTimeQueue *gtq = new GTimeQueue();
	GTQNormalBox * box = NULL;
	QColor fColor(255,0,0);
	QColor bColor(0,0,0);
	QString label = "Prueba caja";
	QRect rect(0,0,100,20);
	QPoint refPos(200,200);
	
	
	box = new GTQNormalBox(bColor,fColor, label, true, rect);
	box->setStartMs(refPos.x());
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	gtq->append(box);
	
	printf("vamos a empezar...\n");
	gtq->show();
	gtq->repaint();
	
	printf("vamos a ejecutar app...\n");
	
	app.exec();
	
	printf("vamos a terminar...\n");
	delete gtq;
	return 0;
}