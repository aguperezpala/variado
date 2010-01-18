#include <stdio.h>
#include <QApplication>
#include "gtimequeue.h"
#include "normalbox.h"
#include "timepointer.h"
#include "trigger.h"
#include "timemeters.h"
#include "tester.h"

QImage * bi = NULL;

/* testeo de imagen de fondo */
void testBackImg(GTimeQueue * gtq)
{
	gtq->setBackImg(bi);
	/* ahora seteamos una de fondo */
	bi = new QImage("/home/agustin/cordoba5.jpg");
	gtq->setBackImg(bi);
}

/* seteo de imagen a las cajas */
void setBackBoxImg(GTQNormalBox * b, const char * name)
{
	QImage * img = new QImage(name);
	b->setBackImg(img);
}
	
int main (int argc, char **argv)
{
	QApplication app(argc, argv, true);
	printf("vamos a empezar...\n");
	int i = 0;
	GTimeQueue *gtq = new GTimeQueue();
	GTQNormalBox * box = NULL, *box2 = NULL;
	GTQTrigger * trig1 = NULL, *trig2 = NULL;
	GTQTimeMeter *tm1 = NULL, *tm2 = NULL;
	QColor fColor(255,0,0);
	QColor bColor(150,150,150);
	QPalette gtqBackColor(Qt::black);
	QString label = "Caja 1";
	QColor metersColor(150,150,150);
	
	
	/* configuramos el gtq */
	/*gtq->setBackColor(gtqBackColor);*/
	
// 	testBackImg(gtq);
	
	box = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box->setStartMs(0);
	box->setDurationMs(5*60*60*1000); /* 5 hs*/
// 	setBackBoxImg(box, "/home/agustin/cara_naipe.jpg");
	
	label = "Caja 2";
	box2 = new GTQNormalBox(bColor,fColor, label, false, gtq->getBoxHeigth());
	box2->setStartMs(box->getDurationMs()+5);
	box2->setDurationMs(8*60*60*1000); /* 5 hs*/
	
	/* creamos los medidores */
	tm1 = new GTQTimeMeter();
	tm1->setColor(metersColor);
	tm1->setStart(0);
	tm1->setEnd(box->getDurationMs());
	
	tm2 = new GTQTimeMeter();
	tm2->setColor(metersColor);
	tm2->setStart(box->getDurationMs());
	tm2->setEnd(box->getDurationMs() + box2->getDurationMs());
	
	gtq->insertMeterObject(tm1);
	gtq->insertMeterObject(tm2);
	
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
	gtq->setPointerMs(box->getDurationMs() + 1000*60*17+1000*59);
	gtq->insertBoxObject(box);
	gtq->insertBoxObject(box2);
	
	//gtq->moveBoxObject(box2, 0);
	
	printf("vamos a ejecutar app...\n");
	
	app.exec();
	
	printf("vamos a terminar...\n");
	delete gtq;
	delete tm1; delete tm2;
	delete box;	delete box2;
	delete trig1; 	delete trig2;
	
	return 0;
}