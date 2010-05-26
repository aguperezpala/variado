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


/*! Genera una task desde un string respetando el formato
* asignado para guardar las task
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int Task::fromString(string &str)
{
	string value = "";

	
	if (getValue(str, 0,"<task-desc>", "<task-title>", this->desc) < 0)
		return -1;
	
	if (getValue(str, 0,"<task-title>", "<task-tc>", this->title) < 0)
		return -1;
	
	if (getValue(str, 0,"<task-tc>", "<task-tl>", value) < 0)
		return -1;
	else {
		if (sizeof(this->createdTime) != value.size())
			return -1;
		memcpy(&this->createdTime, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<task-tl>", "<task-st>", value) < 0)
		return -1;
	else {
		if (sizeof(this->limitTime) != value.size())
			return -1;
		/* debemos copiar lo que leimos en createdTime */
		memcpy(&this->limitTime, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<task-st>", "<task-k>", value) < 0)
		return -1;
	else {
		if (sizeof(this->status) != value.size())
			return -1;
		/* debemos copiar lo que leimos en createdTime */
		memcpy(&this->status, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<task-k>", "<task-p>", value) < 0)
		return -1;
	else {
		if (sizeof(this->kind) != value.size())
			return -1;
		memcpy(&this->kind, value.c_str(), value.size());
	}
	
	if (getValue(str, 0,"<task-p>", "<task;>", value) < 0)
		return -1;
	else {
		if (sizeof(this->priority) != value.size())
			return -1;
		memcpy(&this->priority, value.c_str(), value.size());
	}
	
	return 0;
}

/*! Convierte una task en un string listo para ser guardada
* en un archivo.
* RETURNS:
*	NULL		if error
*	strTask		otherwise
* NOTE: Genera memoria
*/
string *Task::toString(void)
{
	string *result = new string();
	char buff[20] = {0};
	
	
	if (result == NULL)
		return result;
	/*<task-desc>descripcion<task-title>title<task-tc>createdTime<task-tl>timeLimit
	<task-st>status<task-k>kind<task-p>priority<task;>*/
	result->append("<task-desc>");
	result->append(this->desc);
	result->append("<task-title>");
	result->append(this->title);
	result->append("<task-tc>");
	memcpy(buff, &this->createdTime, sizeof(this->createdTime));
	result->append(buff, sizeof(this->createdTime));
	result->append("<task-tl>");
	memcpy(buff, &this->limitTime, sizeof(this->limitTime));
	result->append(buff, sizeof(this->limitTime));	
	result->append("<task-st>");
	memcpy(buff, &this->status, sizeof(this->status));
	result->append(buff, sizeof(this->status));
	result->append("<task-k>");
	memcpy(buff, &this->kind, sizeof(this->kind));
	result->append(buff, sizeof(this->kind));
	result->append("<task-p>");
	memcpy(buff, &this->priority, sizeof(this->priority));
	result->append(buff, sizeof(this->priority));
	result->append("<task;>");
	
	return result;
}

/* vamos a comparar 2 task segun su desc (porque nos conviene) */
bool Task::operator==(Task &other)
{
	if(this->desc.compare(other.getDescription()) == 0)
		return true;
	return false;
}


void Task::Print(void)
{
	cout << "\nTask desc: " << this->desc;
	cout << "\nTask title: " << this->title;
	cout << "\nTask status: " << this->status;
	cout << "\nTask kind: " << this->kind;
	cout << "\nTask priority: " << this->priority;
	
	
}
