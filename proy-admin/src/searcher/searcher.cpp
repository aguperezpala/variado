#include "searcher.h"

/* funcion que crea una tarea segun el indice 
* RETURNS:
* 	NULL	on error
* 	task	if success
*/
Task *Searcher::createTask(int index, string &t)
{
	Task *task = new Task();
	
	if (task == NULL)
		return task;
	
	task->setPriority(TASK_P_MEDIUM);
	/* notar que tenemos el mismo orden que las tasks */
	task->setKind(index);
	task->setDescription(t);
	
	return task;
}


/* funcion que dada una cadena la parsea y agrega
* tanto a la lista de strings la cadena y el indice 
* correspondiente
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int Searcher::parseStr(string &line)
{
	string *str = parse_word(line, 0, PARSER_BLANKS);
	int i = 0;
	bool found = false;
	
	if (str == NULL)
		return -1;
	
	/* hacemos esto medio choro */
	while((int)SEARCHER_STR[i].size() > 1){
		if ((int)line.find(SEARCHER_STR[i]) >= 0) {
			/* encontramos uno */
			found = true;
			break;
		}
		i++;
	}
	
	if (!found) {
		delete str;
		return -1;
	}
	
	this->strList.push_back(*str);
	this->typeList.push_back(i);
	delete str;
	
	return 0;
}


/* Funcion que dado un string, busca en el mismo todas las 
* posibles notas, las parsea y las devuelve en una lista
* de tareas.
* RETURNS:
* 	NULL		si no hay tareas o error
* 	list<Note *> *	caso contrario
*/
list<Note *> *Searcher::getNotesFromStr(string &str)
{
	list<Note *> *result = NULL;
	Note *note = NULL;
	string pattern = "";
	string finNote = ";";
	list<string> *strList = NULL;
	list<string>::iterator it;
	int i = 0;
	
	
	if ((int)str.size() == 0)
		return result;
	
	/* vemos que debemos buscar */
	while (i < (int)this->typeList.size()){
		if(this->typeList[i] == SEARCHER_NOTE)
			break;
		i++;
	}
	
	if(i == (int)this->typeList.size())
		return NULL;
	
	pattern = this->strList[i];
	i = 0;
	
	/* obtenemos todas las notas en un string */
	strList = parser_get_comments(str, 0, (int) str.size(), pattern, finNote);
	if(strList == NULL)
		return result;
	
	/* creamos la lista */
	result = new list<Note *>();
	if (result == NULL){
		delete strList;
		return result;
	}
	
	for (it = strList->begin(); it != strList->end(); ++it) {
		if((int) (*it).size() == 0)
			continue;
		note = new Note(*it);
		if (note == NULL)
			continue;
		
		/* agregamos a la lista */
		result->push_back(note);
	}
	
	delete strList;
	
	if(result->size() == 0) {
		delete result;
		result = NULL;
	}
		
	return result;
}

/* Funcion que dado un string, busca en el mismo todas las 
* posibles tareas, las parsea y las devuelve en una lista
* de tareas.
* RETURNS:
* 	NULL		si no hay tareas o error
* 	list<Task *> *	caso contrario
*//*! Es ineficiente pero bueno, asi es mejor */
list<Task *> *Searcher::getTasksFromStr(string &str)
{
	list<Task *> *result = NULL;
	Task *task = NULL;
	string pattern = "";
	string finTask = ";";
	int i = 0, j = 0, from = 0;
	list<string> *strList = NULL;
	list<string>::iterator it;
	
	if ((int)str.size() == 0)
		return result;
	
	/* vemos que debemos buscar */
	while (i < (int)this->typeList.size()){
		if(this->typeList[i] == SEARCHER_NOTE)
			break;
		i++;
	}
	
	
	/* creamos la lista */
	result = new list<Task *>();
	if (result == NULL)
		return result;
	
	
	/* Ahora debemos buscar para todo j != i */
	while (j < (int)this->typeList.size()){
		if (j == i) {
			/*! lo salteamos */
			j++;
			continue;
		}
		from = 0;
		pattern = this->strList[j];
		
		strList =  parser_get_comments(str, 0, (int) str.size(), pattern,
						finTask);
		if (strList == NULL){
			j++;
			continue;
		}
		
		for(it = strList->begin(); it != strList->end(); ++it) {
			if((int) (*it).size() == 0)
				continue;
			
			task = createTask(j, *it);
			
			if(task != NULL)
				/* agregamos a la lista */
				result->push_back(task);
		}
		
		delete strList;
		j++;
	}
	
	return result;
	
}



/* Funcion que carga la configuracion del searcher */
void Searcher::loadConfig(string &confPath)
{
	string *fData = parser_read_all_file(confPath.c_str());
	int bp = 0, ep = 1;
	
	if(fData == NULL)
		return;
	
	while (ep > 0) {
		string aux;
		ep = (*fData).find("\n", bp);
		aux = (*fData).substr(bp,ep-bp);
		if (parseStr(aux) < 0)
			break;
		if (ep < 0)
			break;
		bp = ep + 1;
	}
	delete fData;
}

/* Funcion que setea un proyecto en el cual va a buscar 
* REQUIRES:
*	p 	!= NULL
*/
void Searcher::setProject(Project *p)
{
	assert(p != NULL);
	this->pro = p;
}

/* Funcion que agrega la lista de notas que no se encuentren
* ya en los modulos a los respectivos....
* REQUIRES:
* 	se haya seteado un proyecto
* RETURNS:
* 	number of notes finded
* 	< 0 	on error
*/
int Searcher::searchForNotes(void)
{
	list<Module *>::iterator it;
	list<Note *> *notesList = NULL;
	list<Note *>::iterator nit;
	string *fData = NULL;
	int result = 0;
	
	
	assert(this->pro != NULL);
	
	for(it = this->pro->getModules().begin(); 
		it != this->pro->getModules().end(); ++it){
		/* para cada moudlo lo que hacemos es obtener el nombre de
		 * archivo, abrirlo, y parsear todas las notas que encontremos
		 */
		assert((*it) != NULL);
		fData = parser_read_all_file((*it)->getFileName().c_str());
		if (fData == NULL)
			continue;
		
		/* tenemos los datos => parseamos la lista y la agregamos */ 
		notesList = getNotesFromStr(*fData);
		delete fData;
		if (notesList != NULL) {
			/* agregamos todas las notas al modulo */
			for (nit = notesList->begin(); nit != notesList->end();
				++nit) {
				result++;
				assert(*nit != NULL);
				if((*it)->existNote((*nit)))
					delete (*nit);
				else
					(*it)->addNote(*nit);
			}
			delete notesList;
		}
	}
	
	return result;
}

/* Funcion que genera las tareas determinadas segun lo que se
* configuro en el archivo y las agrega a cada modulo
* respectivamente.
* REQUIRES:
* 	se haya seteado un proyecto
* RETURNS:
* 	number of Tasks founded
* 	< 0	on error
*/
int Searcher::searchForTasks(void)
{
	list<Module *>::iterator it;
	list<Task *> *tasksList = NULL;
	list<Task *>::iterator nit;
	string *fData = NULL;
	int result = 0;
	
	
	assert(this->pro != NULL);
	
	for(it = this->pro->getModules().begin(); 
	it != this->pro->getModules().end(); ++it){
		/* para cada moudlo lo que hacemos es obtener el nombre de
		* archivo, abrirlo, y parsear todas las tareas que encontremos
		*/
		assert((*it) != NULL);
		fData = parser_read_all_file((*it)->getFileName().c_str());
		if (fData == NULL)
			continue;
		/* tenemos los datos => parseamos la lista y la agregamos */ 
		tasksList = getTasksFromStr(*fData);
		delete fData;
		if (tasksList != NULL) {
			/* agregamos todas las notas al modulo */
			for (nit = tasksList->begin(); nit != tasksList->end();
			++nit) {
				result++;
				assert(*nit != NULL);
				if ((*it)->existTask(*nit))
					delete (*nit);
				else
					(*it)->addTask(*nit);
			}
			delete tasksList;
		}
	}
	
	return result;
}
