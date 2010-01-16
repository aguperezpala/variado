#include <stdio.h>
#include <QApplication>
#include "gtimequeue.h"
#include "normalbox.h"
#include "timepointer.h"
#include "trigger.h"
#include "tester.h"


int main (int argc, char **argv)
{
	QApplication app(argc, argv, true);
	printf("vamos a empezar...\n");
	int i = 0;
	GTimeQueue *gtq = new GTimeQueue();
	GTQNormalBox * box = NULL, *box2 = NULL;
	GTQTrigger * trig1 = NULL, *trig2 = NULL;
	QColor fColor(255,0,0);
	QColor bColor(0,0,0);
	QString label = "Caja 1";
	
	QPoint refPos(200,200);
	QPoint refPos2(9999000,200);
	QPoint refPos3(200,200);
	
	
	
	box = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box->setStartMs(0);
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	label = "Caja 2";
	box2 = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box2->setStartMs(box->getDurationMs()+refPos2.x()+5);
	box2->setDurationMs(8*60*60*1000); /* 5 hs*/
	
	/* creamos los triggers */
	trig1 = new GTQTrigger(GTQT_JUMP);
	trig1->setStart(0);
	trig1->setEnd(box->getDurationMs());
	trig1->setColor(fColor);
	
	trig2 = new GTQTrigger(GTQT_STOP);
	trig2->setStart(box->getDurationMs());
	trig2->setEnd(box->getDurationMs()+9999999);
	
	gtq->insertTriggerObject(trig1);
	gtq->insertTriggerObject(trig2);
	
// 	gtq->setScale(144000/2);
	
	
	printf("vamos a empezar...\n");
	gtq->show();
	gtq->setPointerMs(box->getDurationMs());
	gtq->insertBoxObject(box);
	gtq->insertBoxObject(box2);
	
	//gtq->moveBoxObject(box2, 0);
	
	printf("vamos a ejecutar app...\n");
	
	app.exec();
	
	printf("vamos a terminar...\n");
	delete gtq;
	delete box;	delete box2;
	delete trig1; 	delete trig2;
	
	return 0;
}