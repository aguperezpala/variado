#include <stdio.h>
#include <QApplication>
#include "gtimequeue.h"
#include "normalbox.h"
#include "timepointer.h"
#include "tester.h"


int main (int argc, char **argv)
{
	QApplication app(argc, argv, true);
	printf("vamos a empezar...\n");
	GTimeQueue *gtq = new GTimeQueue();
	GTQNormalBox * box = NULL, *box2 = NULL;
	QColor fColor(255,0,0);
	QColor bColor(0,0,0);
	QString label = "Caja 1";
	
	QPoint refPos(200,200);
	QPoint refPos2(9999000,200);
	QPoint refPos3(200,200);
	
	
	
	box = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box->setStartMs(refPos.x());
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	label = "Caja 2";
	box2 = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box2->setStartMs(box->getDurationMs()+refPos2.x()+5);
	box2->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	
	gtq->insertBoxObject(box);
	gtq->insertBoxObject(box2);
	
	printf("vamos a empezar...\n");
	gtq->show();
	gtq->repaint();
	
	printf("vamos a ejecutar app...\n");
	
	app.exec();
	
	printf("vamos a terminar...\n");
	delete gtq;
	return 0;
}