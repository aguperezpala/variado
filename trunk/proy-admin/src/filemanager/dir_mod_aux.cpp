#include "dir_mod_aux.h"


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
void dirm_filter(vector<string> &ent, const char *begin, const char *end)
{
	string sBegin = " ";
	string sEnd = " ";
	vector<string>::iterator it;
	vector<string> result;
	int i = 0;
	
	result.clear();
	
	if (begin != NULL)
		sBegin.assign(begin);
	
	if (end != NULL)
		sEnd.assign(end);
	
	/* FIXME: fijarse que es medio ineficiente esto */
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
		
		if(i != 2)
			/* lo agregamos */
			result.push_back(*it);
	}
	ent = result;

}


/* Funcion que devuelve llena un vector con todos los directorios partiendo de
* uno raiz e ingresando recursivamente en todos.
* NOTE: los directorios van a tener la forma: rpath/xx/xx..
*/
void dirm_get_rdirs(vector<string> &dirs, string &rpath)
{
	// get the "root" directory's directories
	vector<string> fileList;
	string path = "";
	
	
	dirm_list_directories(rpath, fileList, NULL);
	dirm_filter(fileList, ".", NULL);
	
	if (rpath[rpath.size()-1] == '/')
		rpath.erase(rpath.size()-1,1);
	
	for (vector<string>::iterator i=fileList.begin(); i!=fileList.end(); ++i)
	{
		// test for . and .. directories (this and back)
		if (strcmp((*i).c_str(), ".") &&
			strcmp((*i).c_str(), ".."))
		{
			// i use stringstream here, not string = foo; string.append(bar);
			stringstream fullname;
			fullname << rpath << "/" << (*i);
			
			dirs.push_back(fullname.str());
			path = fullname.str();
			dirm_get_rdirs(dirs, path);
		}
	}
	
}