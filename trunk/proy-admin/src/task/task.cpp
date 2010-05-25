#include "task.h"

/* setea/obtiene el tiempo de creacion */
void Task::setCreatedTime(time_t &ct)
{
	memcpy(&this->createdTime, &ct, sizeof(ct));
}

/* setea/obtiene el tiempo de fecha limite */
void Task::setLimitTime(time_t &lt)
{
	memcpy(&this->limitTime, &lt, sizeof(lt));
}

void Task::Print(void)
{
	cout << "\nTask desc: " << this->desc;
	cout << "\nTask title: " << this->title;
	cout << "\nTask status: " << this->status;
	cout << "\nTask kind: " << this->kind;
	cout << "\nTask priority: " << this->priority;
	
	
}
