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
	GTQNormalBox * box = NULL, *box2 = NULL, *box3 = NULL;
	QColor fColor(255,0,0);
	QColor bColor(0,0,0);
	QString label = "Caja 1";
	QRect rect(0,0,100,20);
	QRect rect2(0,0,100,40);
	QRect rect3(0,0,100,30);
	QPoint refPos(200,200);
	QPoint refPos2(200,200);
	QPoint refPos3(200,200);
	
	
	
	box = new GTQNormalBox(bColor,fColor, label, false, rect);
	box->setStartMs(refPos.x());
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	label = "Caja 2";
	box2 = new GTQNormalBox(bColor,fColor, label, false, rect2);
	box->setStartMs(refPos2.x()+5);
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	label = "Caja 3";
	box3 = new GTQNormalBox(bColor,fColor, label, false, rect3);
	box->setStartMs(refPos3.x()+10);
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
	
	gtq->append(box);
	gtq->append(box2);
	gtq->append(box3);
	
	printf("vamos a empezar...\n");
	gtq->show();
	gtq->repaint();
	
	printf("vamos a ejecutar app...\n");
	
	app.exec();
	
	printf("vamos a terminar...\n");
	delete gtq;
	return 0;
}