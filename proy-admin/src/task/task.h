#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include <string.h>
#include <time.h>

/* Enumeramos las prioridades */
enum {
	TASK_P_LOW,	
	TASK_P_MEDIUM,	
	TASK_P_HIGH
};
/* enumeramos los tipos */
enum {
	TASK_K_BUGFIX,		/* es una tarea para reparar un bug */
	TASK_K_REVISION,	/* es una tarea para revisar algo */
	TASK_K_TODO,		/* tarea normal */
	TASK_k_IMPROVE,		/* tarea de mejoramiento */
	TASK_K_NORMAL		/* tarea normal..? wtf XD */
};
/* enumeramos el estado en el que se puede encontrar la tarea */
enum {
	TASK_S_DONE,		/* tarea realizada */
	TASK_S_CHECKED		/* tarea vista/chequeada */
};

class Task {
	
	public:
		/* constructor */
		Task(){};
				
		/* setea/obtiene la descripcion */
		void setDescription(string &d){this->desc = d;};
		string &getDescription(void){return this->desc;};
		
		/* setea/obtiene titulo */
		void setTitle(string &t){this->title = d;};
		string &getTitle(void){return this->title;};
		
		/* setea/obtiene el tiempo de creacion */
		void setCreatedTime(time_t &ct);
		time_t &getCreatedTime(void){return this->createdTime;};
		
		/* setea/obtiene el tiempo de fecha limite */
		void setLimitTime(time_t &lt);
		time_t &getLimitTime(void){return this->limitTime;};
		
		/* setea/obtiene el estado */
		void setStatus(int s){this->status = s;};
		int getStatus(void){return this->status;};
		
		/* setea/obtiene el tipo */
		void setKind(int k){this->kind= k;};
		int getKind(void){return this->kind;};
		
		/* setea/obtiene la prioridad */
		void setPriority(int p){this->priority= p;};
		int getPriority(void){return this->priority;};
		
		~Task(){};
	
	private:
		string desc;
		string title;
		time_t createdTime;
		time_t limitTime;
		int status;
		int kind;
		int priority;
	
};




#endif
