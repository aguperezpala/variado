#include "simplethread.h"

/* Constructor: */
SimpleThread::SimpleThread()
{
	pthread_attr_init(&this->attr);
	this->runing = false;
}

/* funcion que ejecuta el codigo del thread */
void SimpleThread::start()
{
	if(this->runing)
		return;
	this->runing = true;
	pthread_create(&this->thread, &this->attr, &SimpleThread::realThread, this);
}
	
	

/* funcion que detiene el thread */
void SimpleThread::stop()
{
	this->runing = false;
	pthread_join(this->thread, NULL);
}


/* Destructor */
SimpleThread::~SimpleThread(void)
{
	pthread_attr_destroy(&this->attr);
}