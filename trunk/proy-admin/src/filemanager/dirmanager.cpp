#include "dirmanager.h"


/* Funcion que se encarga de listar los directorios de un path determinado
* que contengan una cadena determinada (o NULL para listar todos)
* RETURNS:
* 	0	if success
* 	< 0	on error
*/
int dirm_list_directories(string &dir, vector<string> &dirs, const char *match)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	
	
	if((dp  = opendir(dir.c_str())) == NULL) {
		return -1;
	}
	if(match == NULL){
		while ((dirp = readdir(dp)) != NULL) {
			if(dirp->d_type & DT_DIR)
				dirs.push_back(string(dirp->d_name));
		}
	} else {
		while ((dirp = readdir(dp)) != NULL) {
			if(dirp->d_type & DT_DIR && 
				(strstr(match, dirp->d_name) != NULL))
				dirs.push_back(string(dirp->d_name));
		}
	}
		
	closedir(dp);
	
	return 0;
}



/* Funcion que se encarga de listar los archivos de un directorio
* que contengan una cadena determinada (o NULL para listar todos)
* RETURNS:
* 	0	if success
* 	< 0	on error
*/
int dirm_list_files(string &dir, vector<string> &files, const char *match)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	
	
	if((dp  = opendir(dir.c_str())) == NULL) {
		return -1;
	}
	if(match == NULL){
		while ((dirp = readdir(dp)) != NULL) {
				files.push_back(string(dirp->d_name));
		}
	} else {
		while ((dirp = readdir(dp)) != NULL) {
			if(strstr(dirp->d_name, match) != NULL)
				files.push_back(string(dirp->d_name));
		}
	}
		
	closedir(dp);
	
	return 0;
}


/* Funcion auxiliar que sirve para filtrar directorios que comiencen
* de determinada forma, o terminen de determinada forma.
* REQUIRES:
* 	vector<string>
* 	begin	(como debe empezar) | NULL para no verificar esto
*	end	(como debe terminar) | NULL para no verificar esto
* 	
*/
void drim_filter(vector<string> &ent, const char *begin, const char *end)
{
	string sBegin = "";
	string sEnd = "";
	vector<string>::iterator it;
	int i = 0;
	
	if (begin != NULL)
		sBegin.assign(begin);
	
	if (end != NULL)
		sEnd.assign(end);
	
	for (it = ent.begin(); it != ent.end(); ++it) {
		i = 0;
		if (begin != NULL){
			if(((int)(*it).find(sBegin) == 0))
				i++;
		} else
			i++;
		
		if (end != NULL){
			if (((int)(*it).find(sEnd) == (int)((*it).size() - 
				sEnd.size())))
				i++;
		} else
			i++;
		
		if ((int) i == 2 ){
			cout << "borrando " << *it << endl;
			ent.erase(it);
		}
	}

}
