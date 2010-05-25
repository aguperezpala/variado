/*! Modulo encargado de manipular los directorios
 */
#ifndef DIR_MANAGER_H
#define DIR_MANAGER_H

#ifndef _BSD_SOURCE
	#define _BSD_SOURCE
#endif

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <string>
#include <list>

using namespace std;


/* Funcion que se encarga de listar los directorios de un path determinado
 * que contengan una cadena determinada (o NULL para listar todos)
 * RETURNS:
 * 	0	if success
 * 	< 0	on error
 */
int dirm_list_directories(string &dir, vector<string> &dirs, const char *match);


/* Funcion que se encarga de listar los archivos de un directorio
* que contengan una cadena determinada (o NULL para listar todos)
* RETURNS:
* 	0	if success
* 	< 0	on error
*/
int dirm_list_files(string &dir, vector<string> &files, const char *match);


/* Funcion auxiliar que sirve para filtrar directorios que comiencen
 * de determinada forma, o terminen de determinada forma.
 * Elimina todos aquellos que matcheen
 * REQUIRES:
 * 	vector<string>
 * 	begin	(como debe empezar) | NULL para no verificar esto
 *	end	(como debe terminar) | NULL para no verificar esto
 * 	
*/
void drim_filter(vector<string> &ent, const char *begin, const char *end);


#endif
