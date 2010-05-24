#include "task.h"

/* setea/obtiene el tiempo de creacion */
void Task::setCreatedTime(time_t &ct)
{
	memcpy(&this->createdTime, &ct, sizeof(ct));
}

/* setea/obtiene el tiempo de fecha limite */
void Task::setLimitTime(time_t &lt)
{
	memcpy(&this->limitTime, &lt, sizeof(ct));
}


