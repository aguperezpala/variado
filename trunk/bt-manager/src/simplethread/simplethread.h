#ifndef SIMPLETHREAD_H
#define SIMPLETHREAD_H

#include <pthread.h>
#include <stdbool.h>

using namespace::std;

class SimpleThread {
	
	public:
		/* Constructor: */
		SimpleThread();
		
		/* funcion que ejecuta el codigo del thread */
		void start();
		
		/* funcion que se reimplementa donde se ejecutara el nuevo 
		 * thread */
		virtual void run(void){};
		
		/* funcion que detiene el thread */
		void stop();
		
		/* funcion que determina si se esta ejecutando el codigo */
		bool isRunning(void){return this->runing;};
		
		
		/* Destructor */
		~SimpleThread(void);
		
	private:
		/* atributos */
		pthread_attr_t attr;
		pthread_t thread;
		bool runing;
		
		/* funcion auxiliar para castear y llamar al verdadero thread */
		static void *realThread(void *obj)
		{
			reinterpret_cast<SimpleThread *>(obj)->run();
			return NULL;
		};
		
		
};



#endif
